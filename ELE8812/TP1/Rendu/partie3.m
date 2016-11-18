%*************************************************************************%
% Fichier : partie3.m                                                     %
% Description : Script appliquant les diff�rents filtres spatiaux         %
%               � l'image d'IRM du genou.                                 %
% Auteurs : Julien ANTOINE 1813026 & Maxime SCHMITT 1719088               %
% Date : 03/02/2016                                                       %
%*************************************************************************%

clear all;
close all;
clc;

%% Chargement de l'image et d�finition des param�tres
im = imread('IRM_genou.tif');

tailleFiltre = 3;

%% Application des filtres

imSimple = filtreImage(im, tailleFiltre, 'simple');
imPonder = filtreImage(im, tailleFiltre, 'ponderee');
imMedian = filtreImage(im, tailleFiltre, 'median');

imwrite(imSimple, 'IRM_genouSimple.tif');
imwrite(imPonder, 'IRM_genouPonder.tif');
imwrite(imMedian, 'IRM_genouMedian.tif');

% Affichage vis-�-vis des images
figure(1);
subplot(2, 2, 1);
imshow(im);
title('Image originale');
subplot(2, 2, 2);
imshow(imSimple);
title('Image filtr�e avec un moyennage simple');
subplot(2, 2, 3);
imshow(imPonder);
title('Image filtr�e avec un moyennage pond�r�');
subplot(2, 2, 4);
imshow(imMedian);
title('Image filtr�e avec un filtrage m�dian');

% Affichage des histogrammes en vis-�-vis
figure(2);
subplot(2, 2, 1);
imhist(im);
title('Histogramme original');
subplot(2, 2, 2);
imhist(imSimple);
title('Histogramme de l''image filtr�e avec un moyennage simple');
subplot(2, 2, 3);
imhist(imPonder);
title('Histogramme de l''image filtr�e avec un moyennage pond�r�');
subplot(2, 2, 4);
imhist(imMedian);
title('Histogramme de l''image filtr�e avec un filtrage m�dian');