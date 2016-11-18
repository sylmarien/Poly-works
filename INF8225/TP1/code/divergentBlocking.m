%*************************************************************************%
% Fichier : divergentBlocking.m                                           %
% Description : Implémentation d'un exemple mettant en évidence le        %
%               concept de divergent blocking                             %
% Auteur : Maxime SCHMITT 1719088                                         %
% Date : 01/02/16                                                         %
%*************************************************************************%

close all;
clear all;
clc;

%% Definir les valeurs
D=1;
R=2;
P=3;
names=cell(1,P);

% Aliases pour 1 (Faux ou 0 binaire) et 2 (Vrai ou 1 binaire)
Faux = 1;
Vrai = 2;

names{D}='DouleurCrane';
names{R}='SeRepose';
names{P}='PrendMedicament';

%% Définitions du réseau baysien
dgm=zeros(3,3);
dgm(D, [R P]) = 1;

%% Definition des distributions de probabilités conditionnelles

% P(D = 0) = 0.8
% P(D = 1) = 0.2
CPDs{D}=tabularCpdCreate(reshape([0.8, 0.2], 2, 1));

% P(R = 0 | D = 0) = 0.8
% P(R = 0 | D = 1) = 0.4
% P(R = 1 | D = 0) = 0.2
% P(R = 1 | D = 1) = 0.6
CPDs{R}=tabularCpdCreate(reshape([0.8, 0.4, 0.2, 0.6], 2, 2));

% P(P = 0 | D = 0) = 0.9
% P(P = 0 | D = 1) = 0.3
% P(P = 1 | D = 0) = 0.1
% P(P = 1 | D = 1) = 0.7
CPDs{P}=tabularCpdCreate(reshape([0.9, 0.3, 0.1, 0.7], 2, 2));

%% Création et affichage du diagramme
dgm=dgmCreate(dgm, CPDs, 'nodeNames', names, 'infEngine', 'jtree');

drawNetwork('-adjMatrix', dgm.G, '-nodeLabels', names, '-layout', Treelayout);

%% Compilation du réseau baysien
joint = dgmInferQuery(dgm, [D R P]);
lab = cellfun(@(x) {sprintf('%d ',x)}, num2cell(ind2subv([2 2 2],1:8),2));

% Affichage en histogramme
figure;
bar(joint.T(:))
set(gca,'xtick',1:8);
xticklabelRot(lab, 90, 10, 0.01)
title('Distribution jointe des probabilités')

%% Requêtes
    %% Probabilités de R et P à D fixé : D est vrai
    
    % D est vrai
    clamped=sparsevec(D, Vrai, 3);
    
    pR=tabularFactorCondition(joint, R, clamped);
    fprintf('P(R=1|D=1) = %f\n', pR.T(Vrai));
    pP=tabularFactorCondition(joint, P, clamped);
    fprintf('P(P=1|D=1) = %f\n', pP.T(Vrai));

    %% Nouvelle probabilité de P quand R est vrai
    
    % D et R sont vrais
    clamped=sparsevec([D R], [Vrai Vrai], 3);
    
    pP=tabularFactorCondition(joint, P, clamped);
    fprintf('P(P=1|D=1,R=1) = %f\n', pP.T(Vrai));
    