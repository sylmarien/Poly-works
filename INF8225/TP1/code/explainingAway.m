%*************************************************************************%
% Fichier : explainingAway.m                                              %
% Description : Implémentation d'un exemple mettant en évidence le        %
%               concept d'explaining away                                 %
% Auteur : Maxime SCHMITT 1719088                                         %
% Date : 01/02/16                                                         %
%*************************************************************************%

close all;
clear all;
clc;

%% Definir les valeurs
M=1;
F=2;
A=3;
names=cell(1,A);

% Aliases pour 1 (Faux ou 0 binaire) et 2 (Vrai ou 1 binaire)
Faux = 1;
Vrai = 2;

names{M}='Malade';
names{F}='ProblemeFamilial';
names{A}='Absent';

%% Définitions du réseau baysien
dgm=zeros(3,3);
dgm([M, F], A) = 1;

%% Definition des distributions de probabilités conditionnelles

% P(M = 0) = 0.7
% P(M = 1) = 0.3
CPDs{M}=tabularCpdCreate(reshape([0.7, 0.3], 2, 1));

% P(F = 0) = 0.9
% P(F = 1) = 0.1
CPDs{F}=tabularCpdCreate(reshape([0.9, 0.1], 2, 1));

% P(A = 0 | M = 0, F = 0) = 0.9
% P(A = 0 | M = 0, F = 1) = 0.4
% P(A = 0 | M = 1, F = 0) = 0.5
% P(A = 0 | M = 1, F = 1) = 0.1
% P(A = 1 | M = 0, F = 0) = 0.1
% P(A = 1 | M = 0, F = 1) = 0.6
% P(A = 1 | M = 1, F = 0) = 0.5
% P(A = 1 | M = 1, F = 1) = 0.9
CPDs{A}=tabularCpdCreate(reshape([0.9 0.4 0.5 0.1 0.1 0.6 0.5 0.9], 2, 2, 2));

%% Création et affichage du diagramme
dgm=dgmCreate(dgm, CPDs, 'nodeNames', names, 'infEngine', 'jtree');

drawNetwork('-adjMatrix', dgm.G, '-nodeLabels', names, '-layout', Treelayout);

%% Compilation du réseau baysien
joint = dgmInferQuery(dgm, [M F A]);
lab = cellfun(@(x) {sprintf('%d ',x)}, num2cell(ind2subv([2 2 2],1:8),2));

% Affichage en histogramme
figure;
bar(joint.T(:))
set(gca,'xtick',1:8);
xticklabelRot(lab, 90, 10, 0.01)
title('Distribution jointe des probabilités')

%% Requêtes
    %% Probabilités de M et F à A fixé : A est vrai
    
    % A est vrai
    clamped=sparsevec(A, Vrai, 3);
    
    pF=tabularFactorCondition(joint, F, clamped);
    fprintf('P(F=1|A=1) = %f\n', pF.T(Vrai));
    pM=tabularFactorCondition(joint, M, clamped);
    fprintf('P(M=1|A=1) = %f\n', pM.T(Vrai));

    %% Nouvelle probabilité de M quand F est vrai
    
    % F et A sont vrais
    clamped=sparsevec([F A], [Vrai Vrai], 3);
    
    pM=tabularFactorCondition(joint, M, clamped);
    fprintf('P(M=1|F=1,A=1) = %f\n', pM.T(Vrai));
    