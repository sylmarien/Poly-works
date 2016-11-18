%*************************************************************************%
% Fichier : detramage.m                                                   %
% Description : Script effectuant le d�tramage pqr r�jection de fr�quences%
%               de l'image Voiture.tif                                    %
% Auteurs : Julien ANTOINE 1813026 & Maxime SCHMITT 1719088               %
% Date : 16/02/2016                                                       %
%*************************************************************************%

clear all; clc;

voiture = imread('Voiture.tif');
voitured = double(voiture);

% Obtention de la repr�sentation fr�quentielle
F = fftshift(fft2(voitured));

%% Entr�e des coordonn�es des fr�quences � couper (cf question 2)

% Matrices des coordonn�es des points lumineux
% PtsLumineux(:,1,[1 2]) -> coordonn�es x et y en fr�quences r�duites
% PtsLumineux(:,2,[1 2]) -> coordonn�es x et y en pixels
PtsLumineux = zeros(2,9,2);
% Coordonn�es en fr�quences r�duites
PtsLumineux(1, 1, [1 2]) = [-0.1766 -0.3204];
PtsLumineux(2, 1, [1 2]) = [-0.1707 -0.1531];
PtsLumineux(3, 1, [1 2]) = [0.1647 -0.3367];
PtsLumineux(4, 1, [1 2]) = [0.1647 -0.1694];
PtsLumineux(5, 1, [1 2]) = [0.002994 0.002041];
PtsLumineux(6, 1, [1 2]) = [-0.1587 0.1735];
PtsLumineux(7, 1, [1 2]) = [-0.1587 0.3408];
PtsLumineux(8, 1, [1 2]) = [0.1766 0.1571];
PtsLumineux(9, 1, [1 2]) = [0.1826 0.3245];
% Coordonn�es en pixels
PtsLumineux(1, 2, [1 2]) = [55 45];
PtsLumineux(2, 2, [1 2]) = [56 86];
PtsLumineux(3, 2, [1 2]) = [112 41];
PtsLumineux(4, 2, [1 2]) = [112 82];
PtsLumineux(5, 2, [1 2]) = [85 124];
PtsLumineux(6, 2, [1 2]) = [58 166];
PtsLumineux(7, 2, [1 2]) = [58 207];
PtsLumineux(8, 2, [1 2]) = [114 162];
PtsLumineux(9, 2, [1 2]) = [115 203];

%% Coupure des fr�quences les plus significatives (centre de la repr�sentation fr�quentielle)

% On ne le fait pas ici (l'�nonc� demande de filtrer les points lumineux
% mais celui-ci n'est pas un artefact du type que l'on veut supprimer)

% [~, HH1] = filtpb(voitured, 5, 'gauss');
% HH1 = ones(size(voitured)) - HH1;
% imFilt1 = uint8(ifft2(ifftshift(F .* HH1)));
% 
% figure(1);
% subplot(1,2,1);
% imshow(mat2gray(log(abs(F)+1)));
% subplot(1,2,2);
% imshow(mat2gray(log(abs(F.*HH1)+1)));
% 
% figure(2);
% subplot(1,2,1);
% imshow(voiture);
% title('Image originale');
% subplot(1,2,2);
% imshow(imFilt1);
% title('Image filtr�e');

%% Coupure des secondes fr�quences les plus significatives ("Premier cercle" : points d'indice pair)

% On d�termine W en trouvant la largeur de l'anneau contenant tous les
% points lumineux

    % On calcule la distance de chaque point au centre
    distances = zeros(4,1);
    for i=[2 4 6 8]
        distances(i/2) = sqrt(PtsLumineux(i,1,1)^2 + PtsLumineux(i,1,2)^2);
    end
    maxDist = max(distances);
    minDist = min(distances);

    % On choisit comme point o� appliquer le filtre un point "moyen" des points
    % concern�s
    nu1 = mean(abs(PtsLumineux([2 4 6 8],1,1)));
    nu2 = mean(abs(PtsLumineux([2 4 6 8],1,2)));

    % On choisit comme largeur de bande en fonction de la diff�rence de
    % distance entre le point le plus �loign� et celui le plus proche
    % (Chiffre 40 trouv� empiriquement; rel�ve d'une pr�f�rence personnelle du r�sultat)
    W = (maxDist - minDist) * 40;

% On applique le filtre
HH2 = rf_rej(nu1, nu2, W, size(voiture,1), size(voiture,2));
imFilt2 = uint8(ifft2(ifftshift(F .* HH2)));

% Affichage du r�sultat
figure(1);
subplot(2,2,1);
imshow(voiture);
title('Image originale');
subplot(2,2,2);
imshow(imFilt2);
title('Image filtr�e des points les plus lumineux');
subplot(2,2,3);
imshow(mat2gray(log(abs(F)+1)));
title({'Repr�sentation spectrale de' 'l''image originale'});
subplot(2,2,4);
imshow(mat2gray(log(abs(F.*HH2)+1)));
title({'Repr�sentation spectrale de l''image' 'filtr�e des points les plus lumineux'});

% Sauvegarde de la figure
saveas(gcf, '../rapport/images/part4_filtreBas.png');

%% Coupure des derni�res fr�quences les plus significatives ("Deuxi�me cercle" : points d'indice impair sauf 5)

% On d�termine W en trouvant la largeur de l'anneau contenant tous les
% points lumineux

    % On calcule la distance de chaque point au centre
    distances = zeros(4,1);
    for i=[1 3 7 9]
        distances(logical(floor(i/2))) = sqrt(PtsLumineux(i,1,1)^2 + PtsLumineux(i,1,2)^2);
    end
    maxDist = max(distances);
    minDist = min(distances);

    % On choisit comme point o� appliquer le filtre un point "moyen" des points
    % concern�s
    nu1 = mean(abs(PtsLumineux([1 3 7 9],1,1)));
    nu2 = mean(abs(PtsLumineux([1 3 7 9],1,2)));

    % On choisit comme largeur de bande en fonction de la diff�rence de
    % distance entre le point le plus �loign� et celui le plus proche
    W = (maxDist - minDist);

% On applique le filtre
HH3 = rf_rej(nu1, nu2, W, size(voiture,1), size(voiture,2));
imFilt3 = uint8(ifft2(ifftshift(F .* HH3)));

% Affichage du r�sultat
figure(2);
subplot(2,2,1);
imshow(voiture);
title('Image originale');
subplot(2,2,2);
imshow(imFilt3);
title('Image filtr�e des points les moins lumineux');
subplot(2,2,3);
imshow(mat2gray(log(abs(F)+1)));
title({'Repr�sentation spectrale de' 'l''image originale'});
subplot(2,2,4);
imshow(mat2gray(log(abs(F.*HH3)+1)));
title({'Repr�sentation spectrale de l''image','filtr�e des points les moins lumineux'});

% Sauvegarde de la figure
saveas(gcf, '../rapport/images/part4_filtreHaut.png');

%% Image filtr�e avec tous les filtres

figure(3);
subplot(2,2,1);
imshow(voiture);
title('Image originale');
subplot(2,2,2);
imshow(uint8(ifft2(ifftshift(F .* HH2 .* HH3))));
title('Image filtr�e');
subplot(2,2,3);
imshow(mat2gray(log(abs(F)+1)));
title({'Repr�sentation spectrale de' 'l''image originale'});
subplot(2,2,4);
imshow(mat2gray(log(abs(F .* HH2 .* HH3)+1)));
title({'Repr�sentation spectrale de' ' l''image filtr�e des points lumineux'});

% Sauvegarde de la figure
saveas(gcf, '../rapport/images/part4_filtreComplet.png');
