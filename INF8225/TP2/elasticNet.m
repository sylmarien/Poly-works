%*************************************************************************%
%FICHIER:     gradientDescent.m                                           %
%DESCRIPTION: Construit un modèle par apprentissage via la méthode de la  %
%               descente du gradient                                      %
%                                                                         %
%AUTEUR:      Maxime Schmitt 1719088                                      %
%CREATION:    Lundi 15 février 2016                                       %
%                                                                         %
%*************************************************************************%

clear; clc;

%% Variables
load 20news_w100;

n = 4; % Nombre de classes différentes
m = size(newsgroups,2); % Nombres de posts de l'ensemble de données
o = ones(1,m);
i = 1:m; %
j = newsgroups; % vecteur tel que : j(k) = classe du post numéro k
Y = sparse(i,j,o,m,n); %

Theta = rand(4,201)-.5;
X = documents;
% Ajout des 100 caractéristiques binaires dans X
binaries = randi([0 1], 100, size(X,2));
X = [X ; binaries; ones(1,16242)];
taux_dapprentissage = 0.00005;
seuilConvergence = 15;

% Coefficients lambda du terme de régularisation
% lambda1 = lambda2 = 0 : pas de régularisation
% on choisit pour chaque lambdai une valeur parmi [0.01 0.1 1 10]
lambda1 = 1;
lambda2 = 1;

% Définition des subsets de données
[XA, XV, XT, YA, YV, YT] = create_train_valid_test_splits(X, Y);

learningSize = size(XA,2);
validationSize = size(XV,2);
testingSize = size(XT,2);

%% Valeurs initiales pour la boucle

% Calcul de P(Y|X)
pYgivenX = exp(Theta*XA);

% Normalisation des distributions
Zx = zeros(1,length(XA));
for i = 1:length(XA)
    Zx(i) = sum(pYgivenX(:,i));
    pYgivenX(:,i) = pYgivenX(:,i) / Zx(i);
end

[ RL1, dRL1, RL2, dRL2 ] = regularizationTerms( Theta );

% Calcul du log vraisemblance initial
fullYA = full(YA);
leftTerm = fullYA' .* (Theta * XA);
logVraisemblance = sum(leftTerm(:)) - sum(log(Zx(:))) + lambda1 * RL1 + lambda2 * RL2;

% Vecteur des précisions de l'ensemble d'apprentissage et de validation
learningPrecisionVector = [];
validationPrecisionsVector = [];

% Vecteur des log-vraisemblances
logVraisemblanceVector = logVraisemblance;

% Vecteurs des résultats pour les ensembles de tests pour calculer la précision
% Ensemble d'apprentissage
YAvect = zeros(size(YA,1),1);
for i=1:learningSize
    [~, index] = max(YA(i,:));
    YAvect(i) = index;
end

% Ensemble de validation
YVvect = zeros(size(YV,1),1);
for i=1:validationSize
    [~, index] = max(YV(i,:));
    YVvect(i) = index;
end

% Ensemble de test
YTvect = zeros(size(YT,1),1);
for i=1:testingSize
    [~, index] = max(YT(i,:));
    YTvect(i) = index;
end

%% Boucle de convergence

converged = 0;
while ~converged    
    % Calcul de la mise à jour pour les paramètres

    % Calcul des termes de régularisation
    [ RL1, dRL1, RL2, dRL2 ] = regularizationTerms( Theta );
    
        % Calcul du gradient
        
            sumObservations = YA' * XA';
            sumLikelihood = pYgivenX * XA';

            G = sumObservations - sumLikelihood + lambda1 * dRL1 + lambda2 * dRL2;

            % Mise à jour des paramètres
            Theta = Theta + taux_dapprentissage * G;
        
    % Calcul du log vraisemblance conditionnel
    
        % Calcul de P(Y|X)
        pYgivenX = exp(Theta*XA);
        
        % Normalisation
        for i = 1:length(XA)
            Zx(i) = sum(pYgivenX(:,i));
            pYgivenX(:,i) = pYgivenX(:,i) / Zx(i);
        end
        
        % Calcul du log vraisemblance et mise en mémoire de la précédente
        oldLogVraisemblance = logVraisemblance;
        leftTerm = fullYA' .* (Theta * XA);
        logVraisemblance = sum(leftTerm(:)) - sum(log(Zx(:))) + lambda1 * RL1 + lambda2 * RL2;
        logVraisemblanceVector = [logVraisemblanceVector logVraisemblance];
        
        % Comparaison pour la condition de terminaison de l'apprentissage
        diff = abs(oldLogVraisemblance - logVraisemblance)
        if (diff < seuilConvergence)
            converged = 1;
            fprintf('Converged\n');
        end
    
    % Calculer la précision sur l’ensemble d’apprentissage et
    % l’ensemble de validation après chaque itération
    
        % Précision sur l'ensemble d'apprentissage
            % Obtention du résultat par le modèle
            result = Theta * XA;
            resultVect = zeros(size(YA,1),1);
            for i=1:learningSize
                [~, index] = max(result(:,i));
                resultVect(i) = index;
            end

            % Calcul de la précision par rapport aux données
            H = (YAvect == resultVect);
            PrecisionIter = sum(H(:))/length(XA)

            % Ajout de la précision sur l'ensemble de validation
            learningPrecisionVector = [learningPrecisionVector PrecisionIter];
    
        % Précision sur l'ensemble de validation
            % Obtention du résultat par le modèle
            result = Theta * XV;
            resultVect = zeros(size(YV,1),1);
            for i=1:validationSize
                [~, index] = max(result(:,i));
                resultVect(i) = index;
            end

            % Calcul de la précision par rapport aux données
            H = (YVvect == resultVect);
            PrecisionIter = sum(H(:))/length(XV)

            % Ajout de la précision sur l'ensemble de validation
            validationPrecisionsVector = [validationPrecisionsVector PrecisionIter];
end

%% Calcul de la précision sur l’ensemble du test

% Obtention du résultat par le modèle
result = Theta * XT;
resultVect = zeros(size(YT,1),1);
for i=1:testingSize
    [~, index] = max(result(:,i));
    resultVect(i) = index;
end

% Calcul de la précision par rapport aux données
H = (YTvect == resultVect);
testingPrecision = sum(H(:))/length(XT);
fprintf('Précision sur l''ensemble de test  = %f\n', testingPrecision);

%% Sauvegarde des données
% save(['experiments/experimentBElasticNet_taux' num2str(taux_dapprentissage,'%.4f') '_seuil' int2str(seuilConvergence) '.mat'...
%     '_lambda1-' num2str(lambda1, '%.4f') '_lambda2-' num2str(lambda2, '%.4f')])

