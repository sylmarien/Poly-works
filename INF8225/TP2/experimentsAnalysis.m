%*************************************************************************%
%FICHIER:     experimentsAnalysis.m                                       %
%DESCRIPTION: Script pour générer les différents graphes pour les         %
%               expériences                                               %
%                                                                         %
%AUTEUR:      Maxime Schmitt 1719088                                      %
%CREATION:    Lundi 15 février 2016                                       %
%                                                                         %
%*************************************************************************%

clear; clc; close all;
% Création des dossiers nécessaires
mkdir('figures','singleLogLikelihood');
mkdir('figures', 'singlePrecisions');

%% Chargement des expériences dont on veut générer les courbes

% Expériences avec le batch simple
batchExperiments = dir('experiments/experimentBatch*.mat');
batchN = length(batchExperiments);
batchStruct = struct('learningPrecisionVector', {}, 'logVraisemblanceVector', {}, 'validationPrecisionsVector', {}, 'taux_dapprentissage', {}, 'seuilConvergence', {});
for exper=1:batchN
    load(['experiments/' batchExperiments(exper).name]);
    batchStruct(exper).learningPrecisionVector = learningPrecisionVector;
    batchStruct(exper).logVraisemblanceVector = logVraisemblanceVector;
    batchStruct(exper).validationPrecisionsVector = validationPrecisionsVector;
    batchStruct(exper).taux_dapprentissage = taux_dapprentissage;
    batchStruct(exper).seuilConvergence = seuilConvergence;
end

% Expériences avec les mini-batches (une seule construction de mini-batches
% au début)
miniBatchExperiments = dir('experiments/experimentMiniBatch_K*.mat');
miniBatchStruct = struct('learningPrecisionVector', {}, 'logVraisemblanceVector', {}, 'validationPrecisionsVector', {}, 'taux_dapprentissage', {}, 'seuilConvergence', {}, 'K', {});
miniBatchN = length(miniBatchExperiments);
for exper=1:miniBatchN
    load(['experiments/' miniBatchExperiments(exper).name]);
    miniBatchStruct(exper).learningPrecisionVector = learningPrecisionVector;
    miniBatchStruct(exper).logVraisemblanceVector = logVraisemblanceVector;
    miniBatchStruct(exper).validationPrecisionsVector = validationPrecisionsVector;
    miniBatchStruct(exper).taux_dapprentissage = taux_dapprentissage;
    miniBatchStruct(exper).seuilConvergence = seuilConvergence;
    miniBatchStruct(exper).K = K;
end

% Expériences avec les mini-batches (une construction par tour de boucle)
multipleMiniBatchExperiments = dir('experiments/experimentMiniBatchMultipleShuffle*.mat');
multipleMiniBatchStruct = struct('learningPrecisionVector', {}, 'logVraisemblanceVector', {}, 'validationPrecisionsVector', {}, 'taux_dapprentissage', {}, 'seuilConvergence', {}, 'K', {});
multipleMiniBatchN = length(multipleMiniBatchExperiments);
for exper=1:multipleMiniBatchN
    load(['experiments/' multipleMiniBatchExperiments(exper).name]);
    multipleMiniBatchStruct(exper).learningPrecisionVector = learningPrecisionVector;
    multipleMiniBatchStruct(exper).logVraisemblanceVector = logVraisemblanceVector;
    multipleMiniBatchStruct(exper).validationPrecisionsVector = validationPrecisionsVector;
    multipleMiniBatchStruct(exper).taux_dapprentissage = taux_dapprentissage;
    multipleMiniBatchStruct(exper).seuilConvergence = seuilConvergence;
    multipleMiniBatchStruct(exper).K = K;
end

%% Chargement des epxériences dont on veut générer les histogrammes

% Expériences avec les variables binaires aléatoires supplémentaires
% (lambda1,lambda2) != (0,0) => Expérience avec Elastic Net
elasticNetExperiments = dir('experiments/experimentElasticNet*.mat');
elasticNetStruct = struct('learningPrecisionVector', {}, 'logVraisemblanceVector', {}, 'validationPrecisionsVector', {}, 'taux_dapprentissage', {}, 'seuilConvergence', {}, 'K', {}, 'lambda1', {}, 'lambda2', {});
elasticNetN = length(elasticNetExperiments);
for exper=1:elasticNetN
    load(['experiments/' elasticNetExperiments(exper).name]);
    elasticNetStruct(exper).learningPrecisionVector = learningPrecisionVector;
    elasticNetStruct(exper).logVraisemblanceVector = logVraisemblanceVector;
    elasticNetStruct(exper).validationPrecisionsVector = validationPrecisionsVector;
    elasticNetStruct(exper).taux_dapprentissage = taux_dapprentissage;
    elasticNetStruct(exper).seuilConvergence = seuilConvergence;
    elasticNetStruct(exper).K = K;
    elasticNetStruct(exper).Theta = Theta;
    elasticNetStruct(exper).lambda1 = lambda1;
    elasticNetStruct(exper).lambda2 = lambda2;
end

%% Tracé des courbes

% Tracé des courbes pour les expériences de batch
for taux=[0.0005 0.001 0.005]
    figure; hold on
    title(['Batch Learning Precision and Validation Precision per iteration for LearningRate=' num2str(taux)]);
    xlabel('Iterations');
    ylabel('Precision');
    for i=1:batchN
        % Si le taux est le bon, on trace la courbe de l'expérience
        if(batchStruct(i).taux_dapprentissage == taux)
            plot(1:length(batchStruct(i).learningPrecisionVector), batchStruct(i).learningPrecisionVector, '-b',...
                1:length(batchStruct(i).validationPrecisionsVector), batchStruct(i).validationPrecisionsVector, '-r');
        end
    end
    legend('Learning set', 'Validation set', 'Location', 'southeast');
    % Sauvegarde de la figure
    saveas(gcf,['figures/singlePrecisions/batchPrecisions_taux' num2str(taux) '.png']);
    
    figure; hold on
    title(['Batch Log-Likelihood per iteration for LearningRate=' num2str(taux)]);
    xlabel('Iterations');
    ylabel('log-likelihood');
    for i=1:batchN
        % Si le taux est le bon, on trace la courbe de l'expérience
        if(batchStruct(i).taux_dapprentissage == taux)
            plot(1:length(batchStruct(i).logVraisemblanceVector), batchStruct(i).logVraisemblanceVector);
        end
    end
    legend('log-Likelihood', 'Location', 'southeast');
    % Sauvegarde de la figure
    saveas(gcf,['figures/singleLogLikelihood/batchLogLikelihood.png']);
end

% Tracé des précisions sur la même figure
figure; hold on
title('Batch Learning Precision and Validation Precision per iteration');
xlabel('Iterations');
ylabel('Precision');
tauxi = [];
for i=1:batchN
    % On ne trace pas le cas taux=0.005 avec les autres car c'est un cas
    % pathologique
    if(batchStruct(i).taux_dapprentissage ~= 0.005)
        plot(1:length(batchStruct(i).learningPrecisionVector), batchStruct(i).learningPrecisionVector,...
            1:length(batchStruct(i).validationPrecisionsVector), batchStruct(i).validationPrecisionsVector);
        tauxi = [tauxi batchStruct(i).taux_dapprentissage];
    end
end
legend(['Learning set for taux=' num2str(tauxi(1))], ['Validation set for taux=' num2str(tauxi(1))],...
    ['Learning set for taux=' num2str(tauxi(2))], ['Validation set for taux=' num2str(tauxi(2))], 'Location', 'southeast');
% Sauvegarde de la figure
saveas(gcf,['figures/batchPrecisions.png']);

% Tracé des log Vraisemblance sur la même figure
figure; hold on
title('Batch Log-Likelihood per iteration');
xlabel('Iterations');
ylabel('log-likelihood');
tauxi = [];
plotColor = {'-b', '-r'};
cptColor = 1;
for i=1:batchN
    % On ne trace pas le cas taux=0.005 avec les autres car c'est un cas
    % pathologique
    if (batchStruct(i).taux_dapprentissage ~= 0.005)
        plot(1:length(batchStruct(i).logVraisemblanceVector), batchStruct(i).logVraisemblanceVector, plotColor{cptColor});
        tauxi = [tauxi batchStruct(i).taux_dapprentissage];
        cptColor = cptColor + 1;
    end
end
legend(['taux = ' num2str(tauxi(1))], ['taux = ' num2str(tauxi(2))], 'Location', 'southeast');
% Sauvegarde de la figure
saveas(gcf,'figures/batchLogLikelihood.png');

close all;



% % Tracé des courbes pour les expériences de mini-batches (une seule
% % construction) pour K = 20
% for taux=[0.0005 0.001]
%     figure; hold on
%     title(['Simple Mini-Batches Learning Precision and Validation Precision per iteration for LearningRate=' num2str(taux) ' & K=' num2str(20)]);
%     xlabel('Iterations');
%     ylabel('Precision');
%     for i=1:miniBatchN
%         % Si le taux est le bon, on trace la courbe de l'expérience
%         % Et si K = 20 on prend la courbe en compte
%         if(miniBatchStruct(i).taux_dapprentissage == taux && miniBatchStruct(i).K == 20)
%             plot(1:length(miniBatchStruct(i).learningPrecisionVector), miniBatchStruct(i).learningPrecisionVector, '-b',...
%                 1:length(miniBatchStruct(i).validationPrecisionsVector), miniBatchStruct(i).validationPrecisionsVector, '-r');
%         end
%     end
%     legend('Learning set', 'Validation set', 'Location', 'southeast');
%     % Sauvegarde de la figure
%     saveas(gcf,['figures/singlePrecisions/miniBatchPrecisions_taux' num2str(taux) '_K' num2str(20) '.png']);
% 
%     % Tracé des log Vraisemblance sur des courbes séparées
%     figure; hold on
%     title(['Simple Mini-Batches Log-Likelihood per iteration for LearningRate=' num2str(taux) ' & K=' num2str(20)]);
%     xlabel('Iterations');
%     ylabel('log-likelihood');
%     for i=1:miniBatchN
%         % Si le taux est le bon, on trace la courbe de l'expérience
%         % Et si K = 20 on prend la courbe en compte
%         if(miniBatchStruct(i).taux_dapprentissage == taux && miniBatchStruct(i).K == 20)
%             plot(1:length(miniBatchStruct(i).logVraisemblanceVector), miniBatchStruct(i).logVraisemblanceVector);
%         end
%     end
%     legend('log-Likelihood', 'Location', 'southeast');
%     % Sauvegarde de la figure
%     saveas(gcf,['figures/singleLogLikelihood/miniBatchLogLikelihood_taux' num2str(taux) '_K' num2str(20) '.png']);
% end
% 
% % Tracé des précisions sur la même figure
% figure; hold on
% title(['Simple Mini-Batches Learning Precision and Validation Precision per iteration for K=' num2str(20)]);
% xlabel('Iterations');
% ylabel('Precision');
% tauxi = [];
% for i=1:miniBatchN
%     % Si le taux est le bon, on trace la courbe de l'expérience
%     % Et si K = 20 on prend la courbe en compte
%     if(miniBatchStruct(i).K == 20)
%         plot(1:length(miniBatchStruct(i).learningPrecisionVector), miniBatchStruct(i).learningPrecisionVector,...
%             1:length(miniBatchStruct(i).validationPrecisionsVector), miniBatchStruct(i).validationPrecisionsVector);
%         tauxi = [tauxi miniBatchStruct(i).taux_dapprentissage];
%     end
% end
% legend(['Learning set for taux=' num2str(tauxi(1))], ['Validation set for taux=' num2str(tauxi(1))],...
%     ['Learning set for taux=' num2str(tauxi(2))], ['Validation set for taux=' num2str(tauxi(2))], 'Location', 'southeast');
% % Sauvegarde de la figure
% saveas(gcf,['figures/miniBatchPrecisions_taux_K' num2str(20) '.png']);
% 
% % Tracé des log Vraisemblance sur la même figure
% figure; hold on
% title(['Simple Mini-Batches Log-Likelihood per iteration for K=' num2str(20)]);
% xlabel('Iterations');
% ylabel('log-likelihood');
% tauxi = [];
% plotColor = {'-b', '-r'};
% cptColor = 1;
% for i=1:miniBatchN
%     % Si K = 20 on prend la courbe en compte
%     if(miniBatchStruct(i).K == 20)
%         plot(1:length(miniBatchStruct(i).logVraisemblanceVector), miniBatchStruct(i).logVraisemblanceVector, plotColor{cptColor});
%         tauxi = [tauxi miniBatchStruct(i).taux_dapprentissage];
%         cptColor = cptColor + 1;
%     end
% end
% legend(['taux = ' num2str(tauxi(1))], ['taux = ' num2str(tauxi(2))], 'Location', 'southeast');
% % Sauvegarde de la figure
% saveas(gcf,['figures/miniBatchLogLikelihood_K' num2str(20) '.png']);
% 
% close all;



% Tracé des courbes pour les expériences de mini-batches (multiples
% construction) pour K = 20
for taux=[0.0005 0.001]
    figure; hold on
    title(['Mini-Batches Learning Precision and Validation Precision per iteration for LearningRate=' num2str(taux) ' & K=' num2str(20)]);
    xlabel('Iterations');
    ylabel('Precision');
    for i=1:multipleMiniBatchN
        % Si le taux est le bon, on trace la courbe de l'expérience
        % Et si K = 20 on prend la courbe en compte
        if(multipleMiniBatchStruct(i).taux_dapprentissage == taux && multipleMiniBatchStruct(i).K == 20)
            plot(1:length(multipleMiniBatchStruct(i).learningPrecisionVector), multipleMiniBatchStruct(i).learningPrecisionVector, '-b',...
                1:length(multipleMiniBatchStruct(i).validationPrecisionsVector), multipleMiniBatchStruct(i).validationPrecisionsVector, '-r');
        end
    end
    legend('Learning set', 'Validation set', 'Location', 'southeast');
    % Sauvegarde de la figure
    saveas(gcf,['figures/singlePrecisions/multipleMiniBatchPrecisions_taux' num2str(taux) '_K' num2str(20) '.png']);

    % Tracé des log Vraisemblance sur des courbes séparées
    figure; hold on
    title(['Mini-Batches Log-Likelihood per iteration for LearningRate=' num2str(taux) ' & K=' num2str(20)]);
    xlabel('Iterations');
    ylabel('log-likelihood');
    for i=1:multipleMiniBatchN
        % Si le taux est le bon, on trace la courbe de l'expérience
        % Et si K = 20 on prend la courbe en compte
        if(multipleMiniBatchStruct(i).taux_dapprentissage == taux && multipleMiniBatchStruct(i).K == 20)
            plot(1:length(multipleMiniBatchStruct(i).logVraisemblanceVector), multipleMiniBatchStruct(i).logVraisemblanceVector);
        end
    end
    legend('log-Likelihood', 'Location', 'southeast');
    % Sauvegarde de la figure
    saveas(gcf,['figures/singleLogLikelihood/multipleMiniBatchLogLikelihood_taux' num2str(taux) '_K' num2str(20) '.png']);
end

% Tracé des précisions sur la même figure
figure; hold on
title(['Mini-Batches Learning Precision and Validation Precision per iteration for K=' num2str(20)]);
xlabel('Iterations');
ylabel('Precision');
tauxi = [];
for i=1:multipleMiniBatchN
    % Si le taux est le bon, on trace la courbe de l'expérience
    % Et si K = 20 on prend la courbe en compte
    if(multipleMiniBatchStruct(i).K == 20)
        plot(1:length(multipleMiniBatchStruct(i).learningPrecisionVector), multipleMiniBatchStruct(i).learningPrecisionVector,...
            1:length(multipleMiniBatchStruct(i).validationPrecisionsVector), multipleMiniBatchStruct(i).validationPrecisionsVector);
        tauxi = [tauxi multipleMiniBatchStruct(i).taux_dapprentissage];
    end
end
legend(['Learning set for taux=' num2str(tauxi(1))], ['Validation set for taux=' num2str(tauxi(1))],...
    ['Learning set for taux=' num2str(tauxi(2))], ['Validation set for taux=' num2str(tauxi(2))], 'Location', 'southeast');
% Sauvegarde de la figure
saveas(gcf,['figures/multipleMiniBatchPrecisions_taux_K' num2str(20) '.png']);

% Tracé des log Vraisemblance sur la même figure
figure; hold on
title(['Mini-Batches Log-Likelihood per iteration for K=' num2str(20)]);
xlabel('Iterations');
ylabel('log-likelihood');
tauxi = [];
plotColor = {'-b', '-r'};
cptColor = 1;
for i=1:multipleMiniBatchN
    % Si K = 20 on prend la courbe en compte
    if(multipleMiniBatchStruct(i).K == 20)
        plot(1:length(multipleMiniBatchStruct(i).logVraisemblanceVector), multipleMiniBatchStruct(i).logVraisemblanceVector, plotColor{cptColor});
        tauxi = [tauxi multipleMiniBatchStruct(i).taux_dapprentissage];
        cptColor = cptColor + 1;
    end
end
legend(['taux = ' num2str(tauxi(1))], ['taux = ' num2str(tauxi(2))], 'Location', 'southeast');
% Sauvegarde de la figure
saveas(gcf,['figures/multipleMiniBatchLogLikelihood_K' num2str(20) '.png']);

close all;


% Tracé des courbes comparatives entre les méthodes (avec K = 20 pour les
% mini-batches) pour un taux d'apprentissage de 0.0005
taux = 0.0005;
figure; hold on
title(['Learning Precision and Validation Precision per iteration for LearningRate=' num2str(taux) '& K=' num2str(20)]);
xlabel('Iterations');
ylabel('Precision');
for i=1:batchN
    % Si le taux est le bon, on trace la courbe de l'expérience
    if(batchStruct(i).taux_dapprentissage == taux)
        plot(1:length(batchStruct(i).learningPrecisionVector), batchStruct(i).learningPrecisionVector,...
            1:length(batchStruct(i).validationPrecisionsVector), batchStruct(i).validationPrecisionsVector);
    end
end
for i=1:multipleMiniBatchN
    % Si le taux est le bon et K = 20, on trace la courbe de l'expérience
    if(multipleMiniBatchStruct(i).taux_dapprentissage == taux && multipleMiniBatchStruct(i).K == 20)
        plot(1:length(multipleMiniBatchStruct(i).learningPrecisionVector), multipleMiniBatchStruct(i).learningPrecisionVector,...
            1:length(multipleMiniBatchStruct(i).validationPrecisionsVector), multipleMiniBatchStruct(i).validationPrecisionsVector);
    end
end
legend('Batch Learning set', 'Batch Validation set',...
    'Mini-Batch Learning set', 'Mini-Batch Validation set', 'Location', 'southeast');
% Sauvegarde de la figure
saveas(gcf,['figures/comparaisonPrecisions_taux' num2str(taux) '_K' int2str(20) '.png']);

figure; hold on
title(['Mini-Batches Log-Likelihood per iteration for LearningRate=' num2str(taux) '& K=' int2str(20)]);
xlabel('Iterations');
ylabel('log-likelihood');
for i=1:batchN
    % Si le taux est le bon, on trace la courbe de l'expérience
    if(batchStruct(i).taux_dapprentissage == taux)
        plot(1:length(batchStruct(i).logVraisemblanceVector), batchStruct(i).logVraisemblanceVector);
    end
end
for i=1:multipleMiniBatchN
    % Si le taux est le bon et K = 20, on trace la courbe de l'expérience
    if(multipleMiniBatchStruct(i).taux_dapprentissage == taux && multipleMiniBatchStruct(i).K == 20)
        plot(1:length(multipleMiniBatchStruct(i).logVraisemblanceVector), multipleMiniBatchStruct(i).logVraisemblanceVector);
    end
end
legend('Batch', 'Mini-Batch', 'Location', 'southeast');
% Sauvegarde de la figure
saveas(gcf,['figures/comparaisonLogLikelihood_taux' num2str(taux) '_K' int2str(20) '.png']);

close all;


% Tracé des courbes comparatives de l'effet de la valeur de K pour les
% expériences de mini-batches (multiples constructions)
taux = 0.0005;
figure; hold on
title(['Mini-Batches Validation Precision per iteration with various K values for LearningRate=' num2str(0.0005)]);
xlabel('Iterations');
ylabel('Precision');
Ki = [];
plotColor = {'-b', '-r', '-g'};
cptColor = 1;
for i=1:multipleMiniBatchN
    % Si le taux est le bon, on trace la courbe de l'expérience
    if(multipleMiniBatchStruct(i).taux_dapprentissage == taux)
        plot(1:length(multipleMiniBatchStruct(i).validationPrecisionsVector), multipleMiniBatchStruct(i).validationPrecisionsVector, plotColor{cptColor});
        Ki = [Ki multipleMiniBatchStruct(i).K];
        cptColor = cptColor + 1;
    end
end
legend(['K = ' int2str(Ki(1))], ['K = ' int2str(Ki(2))], ['K = ' int2str(Ki(3))], 'Location', 'southeast');
% Sauvegarde de la figure
saveas(gcf,['figures/multipleMiniBatchPrecisions_VariousK_taux' num2str(taux) '.png']);

figure; hold on
title(['Mini-Batches Log-Likelihood per iteration with various K values for LearningRate=' num2str(taux)]);
xlabel('Iterations');
ylabel('log-likelihood');
Ki = [];
plotColor = {'-b', '-r', '-g'};
cptColor = 1;
for i=1:multipleMiniBatchN
    % Si le taux est le bon, on trace la courbe de l'expérience
    if(multipleMiniBatchStruct(i).taux_dapprentissage == taux)
        plot(1:length(multipleMiniBatchStruct(i).logVraisemblanceVector), multipleMiniBatchStruct(i).logVraisemblanceVector, plotColor{cptColor});
        Ki = [Ki multipleMiniBatchStruct(i).K];
        cptColor = cptColor + 1;
    end
end
legend(['K = ' int2str(Ki(1))], ['K = ' int2str(Ki(2))], ['K = ' int2str(Ki(3))], 'Location', 'southeast');
% Sauvegarde de la figure
saveas(gcf,['figures/multipleMiniBatchLogLikelihood_VariousK_taux' num2str(taux) '.png']);

close all;


%% Sélection du couple (lambda1,lambda2) de meilleure performance (sur l'ensemble de validation)
% On cherche le meilleur couple lambda1 lamdba2 (different de (0,0)) sur
% l'ensemble de validation
batchIndex = 1;
if (elasticNetStruct(1).lambda1 ~= 0 || elasticNetStruct(1).lambda1 ~= 0)
    bestIndex = 1;
else
    bestIndex = 2;
end
for i=2:elasticNetN
    if (elasticNetStruct(i).lambda1 ~= 0 || elasticNetStruct(i).lambda1 ~= 0)
        if(elasticNetStruct(i).validationPrecisionsVector(end) > elasticNetStruct(bestIndex).validationPrecisionsVector(end))
            bestIndex = i;
        end
    else
        batchIndex = i;
    end
end

% Affichage du meilleur couple de lambda
fprintf('Meilleur couple : lambda1=%.3f & lambda2=%.3f\n', elasticNetStruct(bestIndex).lambda1, elasticNetStruct(bestIndex).lambda2);

%% Tracé des histogrammes
figure;
subplot(2,2,1);
bar([sum(abs(elasticNetStruct(batchIndex).Theta(1,1:100)))+sum(abs(elasticNetStruct(batchIndex).Theta(2,1:100)))+...
    sum(abs(elasticNetStruct(batchIndex).Theta(3,1:100)))+sum(abs(elasticNetStruct(batchIndex).Theta(4,1:100))) 
    sum(abs(elasticNetStruct(batchIndex).Theta(1,101:200)))+sum(abs(elasticNetStruct(batchIndex).Theta(2,101:200)))+...
    sum(abs(elasticNetStruct(batchIndex).Theta(3,101:200)))+sum(abs(elasticNetStruct(batchIndex).Theta(4,101:200)))]);
title({'Poids total des coefficients sans régularisation selon qu''ils soient associées'
    'aux 100 valeurs originales ou au 100 valeurs générées aléatoirement'});
xlabel('1 : coefficients originaux de Theta; 2 : coefficients supplémentaires de Theta');
ylabel('Poids total de l''ensemble');
subplot(2,2,2);
bar([(abs(elasticNetStruct(batchIndex).Theta(1,1:100)))+(abs(elasticNetStruct(batchIndex).Theta(2,1:100)))+...
    (abs(elasticNetStruct(batchIndex).Theta(3,1:100)))+(abs(elasticNetStruct(batchIndex).Theta(4,1:100))) 
    (abs(elasticNetStruct(batchIndex).Theta(1,101:200)))+(abs(elasticNetStruct(batchIndex).Theta(2,101:200)))+...
    (abs(elasticNetStruct(batchIndex).Theta(3,101:200)))+(abs(elasticNetStruct(batchIndex).Theta(4,101:200)))]);
title('Poids de chaque coefficient sans régularisation');
xlabel('1 : coefficients originaux de Theta; 2 : coefficients supplémentaires de Theta');
ylabel('Poids du coefficient');
subplot(2,2,3);
bar([sum(abs(elasticNetStruct(bestIndex).Theta(1,1:100)))+sum(abs(elasticNetStruct(bestIndex).Theta(2,1:100)))+...
    sum(abs(elasticNetStruct(bestIndex).Theta(3,1:100)))+sum(abs(elasticNetStruct(bestIndex).Theta(4,1:100))) 
    sum(abs(elasticNetStruct(bestIndex).Theta(1,101:200)))+sum(abs(elasticNetStruct(bestIndex).Theta(2,1:100)))+...
    sum(abs(elasticNetStruct(bestIndex).Theta(3,101:200)))+sum(abs(elasticNetStruct(bestIndex).Theta(4,101:200)))]);
title({'Poids total des coefficients avec régularisation selon qu''ils soient associées'
    'aux 100 valeurs originales ou au 100 valeurs générées aléatoirement'});
xlabel('1 : coefficients originaux de Theta; 2 : coefficients supplémentaires de Theta');
ylabel('Poids total de l''ensemble');
subplot(2,2,4);
bar([(abs(elasticNetStruct(bestIndex).Theta(1,1:100)))+(abs(elasticNetStruct(bestIndex).Theta(2,1:100)))+...
    (abs(elasticNetStruct(bestIndex).Theta(3,1:100)))+(abs(elasticNetStruct(bestIndex).Theta(4,1:100))) 
    (abs(elasticNetStruct(bestIndex).Theta(1,101:200)))+(abs(elasticNetStruct(bestIndex).Theta(2,101:200)))+...
    (abs(elasticNetStruct(bestIndex).Theta(3,101:200)))+(abs(elasticNetStruct(bestIndex).Theta(4,101:200)))]);
title('Poids total des coefficients avec régularisation');
xlabel('1 : coefficients originaux de Theta; 2 : coefficients supplémentaires de Theta');
ylabel('Poids du coefficient');

% Sauvegarde de la figure
% saveas(gcf,'figures/ElasticNetBars_taux0.0005.png');

% close all;