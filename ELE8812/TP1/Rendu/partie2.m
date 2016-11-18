%*************************************************************************%
% Fichier : partie2.m                                                     %
% Description : Script effectuant une r�duction puis augmentation         %
%               d'�chelle sur l'image Barbara.tif avec les 2 m�thodes.    %
% Auteurs : Julien ANTOINE 1813026 & Maxime SCHMITT 1719088               %
% Date : 03/02/2016                                                       %
%*************************************************************************%

clear all;
close all;
clc;

%% Chargement de l'image et d�finition des param�tres
im = imread('Barbara.tif');

facteurEchelle = 0.5;

%% Changements d'�chelle successif utilisant les 2 m�thodes

% Avec l'interpolation bilin�aire
imBil = mae_bil(mae_bil(im, facteurEchelle), 1/facteurEchelle);

% Avec l'interpolation Plus Proche Voisin
imPpv = mae_ppv(mae_ppv(im, facteurEchelle), 1/facteurEchelle);

imwrite(imBil, 'BarbaraBil.tif');
imwrite(imPpv, 'BarbaraPpv.tif');

% Affichage vis-�-vis des images
figure(1);
subplot(1, 2, 1);
imshow(imBil);
title('Interpolation bilin�aire');
subplot(1, 2, 2);
imshow(imPpv);
title('Interpolation Plus Proche Voisin');