%*************************************************************************%
% Fichier : serialBlocking.m                                              %
% Description : Implémentation d'un exemple mettant en évidence le        %
%               concept de serial blocking                                %
% Auteur : Maxime SCHMITT 1719088                                         %
% Date : 01/02/16                                                         %
%*************************************************************************%

close all;
clear all;
clc;

%% Definir les valeurs
E=1;
R=2;
V=3;
names=cell(1,V);

% Aliases pour 1 (Faux ou 0 binaire) et 2 (Vrai ou 1 binaire)
Faux = 1;
Vrai = 2;

names{E}='EtudieCours';
names{R}='ReussitExamen';
names{V}='ValideCours';

%% Définitions du réseau baysien
dgm=zeros(3,3);
dgm(E, R) = 1;
dgm(R, V) = 1;

%% Definition des distributions de probabilités conditionnelles

% P(E = 0) = 0.3
% P(E = 1) = 0.7
CPDs{E}=tabularCpdCreate(reshape([0.3, 0.7], 2, 1));

% P(R = 0 | E = 0) = 0.9
% P(R = 0 | E = 1) = 0.2
% P(R = 1 | E = 0) = 0.1
% P(R = 1 | E = 1) = 0.8
CPDs{R}=tabularCpdCreate(reshape([0.9, 0.2, 0.1, 0.8], 2, 2));

% P(V = 0 | R = 0) = 0.8
% P(V = 0 | R = 1) = 0.1
% P(V = 1 | R = 0) = 0.2
% P(V = 1 | R = 1) = 0.9
CPDs{V}=tabularCpdCreate(reshape([0.8, 0.1, 0.2 0.9], 2, 2));

%% Création et affichage du diagramme
dgm=dgmCreate(dgm, CPDs, 'nodeNames', names, 'infEngine', 'jtree');

drawNetwork('-adjMatrix', dgm.G, '-nodeLabels', names, '-layout', Treelayout);

%% Compilation du réseau baysien
joint = dgmInferQuery(dgm, [E R V]);
lab = cellfun(@(x) {sprintf('%d ',x)}, num2cell(ind2subv([2 2 2],1:8),2));

% Affichage en histogramme
figure;
bar(joint.T(:))
set(gca,'xtick',1:8);
xticklabelRot(lab, 90, 10, 0.01)
title('Distribution jointe des probabilités')

%% Requêtes
    %% Probabilités de E et V à R fixé : R est vrai
    
    % R est vrai
    clamped=sparsevec(R, Vrai, 3);
    
    pE=tabularFactorCondition(joint, E, clamped);
    fprintf('P(E=1|R=1) = %f\n', pE.T(Vrai));
    pV=tabularFactorCondition(joint, V, clamped);
    fprintf('P(V=1|R=1) = %f\n', pV.T(Vrai));

    %% Nouvelle probabilité de V quand E est vrai
    
    % E et R sont vrais
    clamped=sparsevec([E R], [Vrai Vrai], 3);
    
    pV=tabularFactorCondition(joint, V, clamped);
    fprintf('P(V=1|E=1,R=1) = %f\n', pV.T(Vrai));
    