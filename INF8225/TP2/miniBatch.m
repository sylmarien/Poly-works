%*************************************************************************%
%FICHIER:     miniBatch.m                                                 %
%DESCRIPTION: Construit un modèle par apprentissage via la méthode des    %
%               mini-batchs (stochastic gradient descent)                 %
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

Theta = rand(4,101)-.5;
X = documents;
X = [X ; ones(1,16242)];
taux_dapprentissage = 0.0005;
seuilConvergence = 15;
K = 20; % Nombre de batches pour la division en mini-batches

% Définition des subsets de données
[XA, XV, XT, YA, YV, YT] = create_train_valid_test_splits(X, Y);

learningSize = size(XA,2);
validationSize = size(XV,2);
testingSize = size(XT,2);

%% Valeurs initiales pour la boucle

% DeltaTheta initial
deltaTheta = 0;

% Création des batches
[XA, YA, batches] = createMiniBatches(XA, YA, K, Theta);

% Calcul du log vraisemblance initial
logVraisemblance = 0;
for k=1:K
    pYgivenXk = batches(k).pYgivenX;
    Xk = batches(k).X;
    fullYk = full(batches(k).Y);
    Zxk = batches(k).Zx;
    leftTerm = fullYk' .* (Theta * Xk);
    logVraisemblance = logVraisemblance + sum(leftTerm(:)) - sum(log(Zxk(:)));
end

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
    % Faire l'apprentissage partiel pour chaque mini-batch
        for k=1:K
            Xk = batches(k).X;
            Yk = batches(k).Y;
            pYgivenXk = batches(k).pYgivenX;
            
            Bk = size(Xk, 2);
            
            % Calcul du gradient pour le mini-batch
            
            sumObservations = Yk' * Xk';
            sumLikelihood = pYgivenXk * Xk';

            G = sumObservations - sumLikelihood;

            % Mise à jour des paramètres
            Theta = Theta + taux_dapprentissage * G;
            
            % Calcul de P(Y|X)
            pYgivenX = exp(Theta*Xk);
            Zx = zeros(1, Bk);
            for i = 1:Bk
                Zx(i) = sum(pYgivenX(:,i));
                pYgivenX(:,i) = pYgivenX(:,i) / Zx(i);
            end %end for
            batches(k).pYgivenX = pYgivenX;
            batches(k).Zx = Zx;
            
        end %end for
        
    % Calcul du log vraisemblance conditionnel
        % Mise en mémoire de la précédente
        oldLogVraisemblance = logVraisemblance;
    
        % Calcul du log vraisemblance
        logVraisemblance = 0;
        for k=1:K
            pYgivenXk = batches(k).pYgivenX;
            Xk = batches(k).X;
            fullYk = full(batches(k).Y);
            Zxk = batches(k).Zx;
            leftTerm = fullYk' .* (Theta * Xk);
            logVraisemblance = logVraisemblance + sum(leftTerm(:)) - sum(log(Zxk(:)));
        end
    
        % Sauvegarde dans le vecteur de stockage
        logVraisemblanceVector = [logVraisemblanceVector logVraisemblance];
        
        % Comparaison pour la condition de terminaison de l'apprentissage
        diff = abs(oldLogVraisemblance - logVraisemblance);
        %fprintf('Différence de log-vraisemblance = %f\n', diff);
        if (diff < seuilConvergence)
            converged = 1;
            fprintf('Converged\n');
        end %end if
    
    % Calculer la précision sur l’ensemble d’apprentissage et
    % l’ensemble de validation après chaque itération
    
        % Précision sur l'ensemble d'apprentissage
            % Obtention du résultat par le modèle
            result = Theta * XA;
            resultVect = zeros(size(YA,1),1);
            for i=1:learningSize
                [~, index] = max(result(:,i));
                resultVect(i) = index;
            end %end for

            % Calcul de la précision par rapport aux données
            H = (YAvect == resultVect);
            PrecisionIter = sum(H(:))/length(XA);

            % Ajout de la précision sur l'ensemble de validation
            learningPrecisionVector = [learningPrecisionVector PrecisionIter];
            %fprintf('Précision sur l''ensemble d''apprentissage  = %f\n', PrecisionIter);
    
        % Précision sur l'ensemble de validation
            % Obtention du résultat par le modèle
            result = Theta * XV;
            resultVect = zeros(size(YV,1),1);
            for i=1:validationSize
                [~, index] = max(result(:,i));
                resultVect(i) = index;
            end %end for

            % Calcul de la précision par rapport aux données
            H = (YVvect == resultVect);
            PrecisionIter = sum(H(:))/length(XV);

            % Ajout de la précision sur l'ensemble de validation
            validationPrecisionsVector = [validationPrecisionsVector PrecisionIter];
            %fprintf('Précision sur l''ensemble de validation  = %f\n', PrecisionIter);
            
end %end while

%% Calcul de la précision sur l'ensemble de test

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
% save(['experiments/experimentMiniBatch_K' int2str(K) '_taux' num2str(taux_dapprentissage,'%.4f') '_seuil' int2str(seuilConvergence) '.mat'])