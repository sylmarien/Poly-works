%*************************************************************************%
% Fichier : mkDetecterCambriolageDgm.m                                    %
% Description : Implémentation d'un réseau bayésien représentant une      %
%               situation de détection et notification de cambriolage     %
% Auteur : Maxime SCHMITT 1719088                                         %
% Date : 01/02/16                                                         %
%*************************************************************************%

close all;
clear all;
clc;

%% Definir les valeurs
C=1;
T=2;
A=3;
M=4;
J=5;
names=cell(1,J);

% Aliases pour 1 (Faux ou 0 binaire) et 2 (Vrai ou 1 binaire)
Faux = 1;
Vrai = 2;

names{C}='Cambriolage';
names{T}='Tremblement';
names{A}='Alarme';
names{M}='MarieAppelle';
names{J}='JeanAppelle';

%% Définitions du réseau baysien
dgm=zeros(5,5);
dgm(C, A) = 1;
dgm(T, [A J]) = 1;
dgm(A, [M J]) = 1;

%% Definition des distributions de probabilités conditionnelles

% P(C = 0) = 0.999
% P(C = 1) = 0.001
CPDs{C}=tabularCpdCreate(reshape([0.999, 0.001], 2, 1));

% P(T = 0) = 0.998
% P(T = 1) = 0.002
CPDs{T}=tabularCpdCreate(reshape([0.998, 0.002], 2, 1));

% P(A = 0 | C = 0, T = 0) = 0.999
% P(A = 0 | C = 0, T = 1) = 0.71
% P(A = 0 | C = 1, T = 0) = 0.06
% P(A = 0 | C = 1, T = 1) = 0.05
% P(A = 1 | C = 0, T = 0) = 0.001
% P(A = 1 | C = 0, T = 1) = 0.29
% P(A = 1 | C = 1, T = 0) = 0.94
% P(A = 1 | C = 1, T = 1) = 0.95
CPDs{A}=tabularCpdCreate(reshape([0.999, 0.71, 0.06, 0.05, 0.001, 0.29, 0.94, 0.95], 2, 2, 2));

% P(M = 0 | A = 0) = 0.95
% P(M = 0 | A = 1) = 0.1
% P(M = 1 | A = 0) = 0.05
% P(M = 1 | A = 1) = 0.9
CPDs{M}=tabularCpdCreate(reshape([0.95, 0.1, 0.05, 0.9], 2, 2));

% P(J = 0 | T = 0, A = 0) = 0.999
% P(J = 0 | T = 0, A = 1) = 0.5
% P(J = 0 | T = 1, A = 0) = 0.999
% P(J = 0 | T = 1, A = 1) = 0.95
% P(J = 1 | T = 0, A = 0) = 0.001
% P(J = 1 | T = 0, A = 1) = 0.5
% P(J = 1 | T = 1, A = 0) = 0.001
% P(J = 1 | T = 1, A = 1) = 0.05
CPDs{J}=tabularCpdCreate(reshape([0.999, 0.5, 0.999, 0.95, 0.001, 0.5, 0.001, 0.05], 2, 2, 2));

%% Création et affichage du diagramme
dgm=dgmCreate(dgm, CPDs, 'nodeNames', names, 'infEngine', 'jtree');

drawNetwork('-adjMatrix', dgm.G, '-nodeLabels', names, '-layout', Treelayout);

%% Compilation du réseau baysien
joint = dgmInferQuery(dgm, [C T A M J]);
lab = cellfun(@(x) {sprintf('%d ',x)}, num2cell(ind2subv([2 2 2 2 2],1:32),2));

% Affichage en histogramme
figure;
bar(joint.T(:))
set(gca,'xtick',1:32);
xticklabelRot(lab, 90, 10, 0.01)
title('Distribution jointe des probabilités')

%% Requêtes
    %% Requêtes de la question c)
    
    % P(Cambriolage = V| MarieAppelle = V, JeanAppelle = F ) 
    clamped=sparsevec([M J], [Vrai Faux], 5);
    
    pC=tabularFactorCondition(joint, C, clamped);
    fprintf('P(Cambriolage = V| MarieAppelle = V, JeanAppelle = F) = %f\n', pC.T(Vrai));
    
    % P(Cambriolage = V| MarieAppelle = F, JeanAppelle = V ) 
    clamped=sparsevec([M J], [Faux Vrai], 5);
    
    pC=tabularFactorCondition(joint, C, clamped);
    fprintf('P(Cambriolage = V| MarieAppelle = F, JeanAppelle = V) = %f\n', pC.T(Vrai));
    
    % P(Cambriolage = V| MarieAppelle = V, JeanAppelle = V ) 
    clamped=sparsevec([M J], [Vrai Vrai], 5);
    
    pC=tabularFactorCondition(joint, C, clamped);
    fprintf('P(Cambriolage = V| MarieAppelle = V, JeanAppelle = V) = %f\n', pC.T(Vrai));
    
    % P(Cambriolage = V| MarieAppelle = F, JeanAppelle = F ) 
    clamped=sparsevec([M J], [Faux Faux], 5);
    
    pC=tabularFactorCondition(joint, C, clamped);
    fprintf('P(Cambriolage = V| MarieAppelle = F, JeanAppelle = F) = %f\n', pC.T(Vrai));
    
    % P(Cambriolage = V| MarieAppelle = V ) 
    clamped=sparsevec(M, Vrai, 5);
    
    pC=tabularFactorCondition(joint, C, clamped);
    fprintf('P(Cambriolage = V| MarieAppelle = V) = %f\n', pC.T(Vrai));
    
    % P(Cambriolage = V| JeanAppelle = V ) 
    clamped=sparsevec(J, Vrai, 5);
    
    pC=tabularFactorCondition(joint, C, clamped);
    fprintf('P(Cambriolage = V| JeanAppelle = V )=%f\n', pC.T(Vrai));

    %% Requêtes de la question d) (Probabilités marginales inconditionnelles)
    
    % P(C)
    pC=tabularFactorCondition(joint, C);
    fprintf('P(Cambriolage = V) = %f\n', pC.T(Vrai));
    
    % P(T)
    pT=tabularFactorCondition(joint, T);
    fprintf('P(Tremblement = V) = %f\n', pT.T(Vrai));
    
    % P(A)
    pA=tabularFactorCondition(joint, A);
    fprintf('P(Alarme = V) = %f\n', pA.T(Vrai));
    
    % P(M)
    pM=tabularFactorCondition(joint, M);
    fprintf('P(MarieAppelle = V) = %f\n', pM.T(Vrai));
    
    % P(J)
    pJ=tabularFactorCondition(joint, J);
    fprintf('P(JeanAppelle = V) = %f\n', pJ.T(Vrai));