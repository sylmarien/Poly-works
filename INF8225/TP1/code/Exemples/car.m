close all;
clear all;

%% Definir les valeurs
B=1; F=2; G=3; D=4; FT=5;
names=cell(1,FT);

% Aliases pour 1 (Faux ou 0 binaire) et 2 (Vrai ou 1 binaire)
Faux = 1;
Vrai = 2;

names{B}='Battery';
names{F}='Fuel';
names{G}='Gauge';
names{D}='Distance';
names{FT}='FillTank';

%% Définitions du réseau baysien
dgm=zeros(5,5);
% Lien de B vers G (etc pour les autres lignes)
dgm(B,G) = 1;
dgm(F,G) = 1;
dgm(G,[D,FT]) = 1;

%% Definition des distributions de probabilités conditionnelles
% reshape permet de transformer les vecteurs en matrices de taille voulue
% (Généralement : carrée, cubique, 'hypercubique', etc)

% P(B = 0) = 0.1
% P(B = 1) = 0.9
CPDs{B}=tabularCpdCreate(reshape([0.1, 0.9], 2, 1));

% P(F = 0) = 0.1
% P(F = 1) = 0.9
CPDs{F}=tabularCpdCreate(reshape([0.1, 0.9], 2, 1));

% Ordre dans le vecteur : compte sur 3 bits où :
% - 1er bit code l'état de la variable dont on obtient la probabilité
% - autres bits codent les états des variables de conditionnement (ordre
%   donné par leur indice défini au début du programme)
% p(G = 0 | B = 0, F = 0) = 0.9
% p(G = 0 | B = 0, F = 1) = 0.8
% p(G = 0 | B = 1, F = 0) = 0.8
% p(G = 0 | B = 1, F = 1) = 0.2
% p(G = 1 | B = 0, F = 0) = 0.1
% p(G = 1 | B = 0, F = 1) = 0.2
% p(G = 1 | B = 1, F = 0) = 0.2
% p(G = 1 | B = 1, F = 1) = 0.8
CPDs{G}=tabularCpdCreate(reshape([0.9, 0.8, 0.8, 0.2, 0.1, 0.2, 0.2, 0.8], 2, 2, 2));

% p(D = 0 | G = 0) = 0.95
% p(D = 0 | G = 1) = 0.7
% p(D = 1 | G = 0) = 0.05
% p(D = 1 | G = 1) = 0.3
CPDs{D}=tabularCpdCreate(reshape([0.95, 0.7, 0.05, 0.3], 2, 2));

% P(FT = 0 | G = 0) = 0.2
% P(FT = 0 | G = 1) = 0.6
% P(FT = 1 | G = 0) = 0.8
% P(FT = 1 | G = 1) = 0.4
CPDs{FT}=tabularCpdCreate(reshape([0.2, 0.6, 0.8, 0.4], 2, 2));

%% Création et affichage du diagramme
dgm=dgmCreate(dgm, CPDs, 'nodeNames', names, 'infEngine', 'jtree');

drawNetwork('-adjMatrix', dgm.G, '-nodeLabels', names, '-layout', Treelayout);

%% Compilation du réseau baysien
% 2ème argument : les variables aléatoires dans l'ordre de leurs indices
joint = dgmInferQuery(dgm, [B F G D FT]);

% arguments de ind2subv :
% - vecteur v avec à chaque indice i le nombre d'états
%   possibles pour la variable d'indice i
% - le nombre de combinaisons possibles (produit des v(i))
lab = cellfun(@(x) {sprintf('%d ',x)}, num2cell(ind2subv([2 2 2 2 2],1:32),2));

% Affichage en histogramme
figure;
bar(joint.T(:))
set(gca,'xtick',1:32);
xticklabelRot(lab, 90, 10, 0.01);
title('Distribution jointe des probas')

%% Requêtes
    %% Requête 1 : G = 0
    
    % "On met le noeud G à 1" => G=0
    clamped=sparsevec(G, Faux, 5);
    
    pFGivenG=tabularFactorCondition(joint, F, clamped);
    fprintf('p(F=0|G=0)=%f\n', pFGivenG.T(Faux));
    % Resultat
    %P(F=0|G=0)=0.257143

    %% Requête 2 : B = 0
    clamped=sparsevec([G, B], [Faux, Faux], 5);
    pFGivenGandB=tabularFactorCondition(joint, F, clamped);
    fprintf('P(F=0|G=0,B=0)=%f\n', pFGivenGandB.T(Faux));
    % Resultat
    %P(F=0|G=0,B=0)=0.11111