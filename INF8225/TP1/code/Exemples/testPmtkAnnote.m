close all;
clear all;

%% Cell definition
B = 1;
F = 2;
G = 3;
D = 4;
FT = 5;

names = cell(1, 5);

names{B} = 'Battery';
names{F} = 'Fuel';
names{G} = 'Gauge';
names{D} = 'Distance';
names{FT} = 'FillTank';

%% Cell interaction definition
dgm = zeros(5,5);
dgm(B, G) = 1;
dgm(F, G) = 1;
dgm(G, [D, FT]) = 1;

%% Conditionnal probas

% p(B = 0) = 0.1;
% p(B = 1) = 0.9;
CPDs{B} = tabularCpdCreate(reshape([0.1, 0.9], 2, 1));

% p(F = 0) = 0.1;
% p(F = 1) = 0.9;
CPDs{F} = tabularCpdCreate(reshape([0.1, 0.9], 2, 1));

% ordre du tableau : 1er bit = la cellule qu'on regarde
% les autres, probas conditionnelles dans l'ordre croissant des indices
% p(G = 0 | B = 0, F = 0) = 0.9;
% p(G = 0 | B = 0, F = 1) = 0.8;
% p(G = 0 | B = 1, F = 0) = 0.8;
% p(G = 0 | B = 1, F = 1) = 0.2;
% p(G = 1 | B = 0, F = 0) = 0.1;
% p(G = 1 | B = 0, F = 1) = 0.2;
% p(G = 1 | B = 1, F = 0) = 0.2;
% p(G = 1 | B = 1, F = 1) = 0.8;
CPDs{G} = tabularCpdCreate(reshape([0.9, 0.8, 0.8, 0.2, 0.1, 0.2, 0.2, 0.8], 2, 2, 2));

% p(D = 0 | G = 0) = 0.95;
% p(D = 0 | G = 1) = 0.7;
% p(D = 1 | G = 0) = 0.05;
% p(D = 1 | G = 1) = 0.3;
CPDs{D} = tabularCpdCreate(reshape([0.95 0.7 .05 0.3], 2, 2));


% p(FT = 0 | G = 0) = 0.2;
% p(FT = 0 | G = 1) = 0.6;
% p(FT = 1 | G = 0) = 0.8;
% p(FT = 1 | G = 1) = 0.4;
CPDs{FT} = tabularCpdCreate(reshape([0.2 0.6 0.8 0.4], 2, 2));

%% Diagram creation
dgm = dgmCreate(dgm, CPDs, 'nodeNames', names, 'infEngine', 'jtree');

drawNetwork('-adjMatrix', dgm.G, '-nodeLabels', names,...
        '-layout', Treelayout);

%% Probas disjointes
joint = dgmInferQuery(dgm, [B F G D FT]);
lab = cellfun(@(x) {sprintf('%d ',x)}, num2cell(ind2subv([2 2 2 2 2],1:32),2));
figure;
bar(joint.T(:))
set(gca,'xtick',1:32);
xticklabelRot(lab, 90, 10, 0.01)
title('Distribution jointe des probas')

%% Requests

    %% Requete 1 : G = 0

    % "On met le noeud G à 1" => G=0
    clamped = sparsevec(G, 1, 5);

    pFGivenG = tabularFactorCondition(joint, F, clamped);
   fprintf('p(F = 0 | G = 0) = %f\n', pFGivenG.T(1));

    %% Requete 2 : G = 1, B = 1
    clamped = sparsevec([G, B], [2, 2], 5);
    pFGivenGandB = tabularFactorCondition(joint, F, clamped);
   fprintf('p(F = 0 | G = 1, B = 1) = %f\n', pFGivenGandB.T(1));



