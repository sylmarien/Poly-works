%*************************************************************************%
% Fichier : representation.m                                              %
% Description : Script effectuant la transform�e de Fourier de l'image et %
%               diverses manipulations sur celle-ci                       %
% Auteurs : Julien ANTOINE 1813026 & Maxime SCHMITT 1719088               %
% Date : 10/02/2016                                                       %
%*************************************************************************%

clear all; clc;

voiture = imread('Voiture.tif');
voitured = double(voiture);

%% 2.1 Indexation des axes fr�quentiels

% Obtention de la repr�sentation fr�quentielle
F = fft2(voitured);

% Centrer la repr�sentation fr�quentielle
F = fftshift(F);
F2 = F; % On garde une version de la fft avec ses valeurs non transform�es

% On rend la repr�sentation affichable et lisible (utilisation du log pour
% utiliser une �chelle de perception)
F = abs(F);
F = log(F+1);
F = mat2gray(F);

% Passage en fr�quences r�duites et affichage
figure(1);
imshow(F);
title('Repr�sentation fr�quentielle');
saveas(gcf, '../rapport/images/part2_freqBasiques.png');

figure(2);
colormap gray;
imagesc([-0.5 0.5],[-0.5 0.5],F);
title('Repr�sentation fr�quentielle en fr�quences r�duites');
saveas(gcf, '../rapport/images/part2_freqR�duites.png');

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

% Affichage des donn�es
for i=1:9
    fprintf('Coordonn�es du point brillant %d en fr�quences r�duites : (%.4f,%.4f) ; en pixels : (%d,%d)\n', i, PtsLumineux(i, 1, 1), PtsLumineux(i, 1, 2), PtsLumineux(i, 2, 1), PtsLumineux(i, 2, 2));
end

%% 2.2 Effet de la parit� des dimensions de l'image

% Comparaison de l'utilisation de fftshift ou ifftshift
figure(3);
subplot(1,3,1);
imshow(uint8(ifft2(ifftshift(F2))));
title('Image obtenue avec ifftshift');
subplot(1,3,2);
imshow(voiture);
title('Image originale');
subplot(1,3,3);
imshow(uint8(ifft2(fftshift(F2))));
title('Image obtenue avec fftshift');
saveas(gcf, '../rapport/images/part2_dimPairesComparaison.png');

% M�me comparaison en enlevant un ligne et une colonne de l'image voiture
voitureTronquee = voiture(1:end-1,1:end-1);
voitureTronqueed = double(voitureTronquee);
FTronquee = fft2(voitureTronqueed);
FTronquee = fftshift(FTronquee);

figure(4);
subplot(1,3,1);
imshow(uint8(ifft2(ifftshift(FTronquee))));
title('Image obtenue avec ifftshift');
subplot(1,3,2);
imshow(voitureTronquee);
title('Image originale');
subplot(1,3,3);
imshow(uint8(ifft2(fftshift(FTronquee))));
title('Image obtenue avec fftshift');
saveas(gcf, '../rapport/images/part2_dimImpairesComparaison.png');
