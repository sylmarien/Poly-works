%*************************************************************************%
% Fichier : partie3.m                                                     %
% Description : Script appliquant les différents filtres spatiaux         %
%               à l'image d'IRM du genou.                                 %
% Auteurs : Julien ANTOINE 1813026 & Maxime SCHMITT 1719088               %
% Date : 03/02/2016                                                       %
%*************************************************************************%

clear all;
close all;
clc;

%% Chargement de l'image et définition des paramètres
im = imread('IRM_genou.tif');

tailleFiltre = 3;

%% Application des filtres

imSimple = filtreImage(im, tailleFiltre, 'simple');
imPonder = filtreImage(im, tailleFiltre, 'ponderee');
imMedian = filtreImage(im, tailleFiltre, 'median');

imwrite(imSimple, 'IRM_genouSimple.tif');
imwrite(imPonder, 'IRM_genouPonder.tif');
imwrite(imMedian, 'IRM_genouMedian.tif');

% Affichage vis-à-vis des images
figure(1);
subplot(2, 2, 1);
imshow(im);
title('Image originale');
subplot(2, 2, 2);
imshow(imSimple);
title('Image filtrée avec un moyennage simple');
subplot(2, 2, 3);
imshow(imPonder);
title('Image filtrée avec un moyennage pondéré');
subplot(2, 2, 4);
imshow(imMedian);
title('Image filtrée avec un filtrage médian');

% Affichage des histogrammes en vis-à-vis
figure(2);
subplot(2, 2, 1);
imhist(im);
title('Histogramme original');
subplot(2, 2, 2);
imhist(imSimple);
title('Histogramme de l''image filtrée avec un moyennage simple');
subplot(2, 2, 3);
imhist(imPonder);
title('Histogramme de l''image filtrée avec un moyennage pondéré');
subplot(2, 2, 4);
imhist(imMedian);
title('Histogramme de l''image filtrée avec un filtrage médian');