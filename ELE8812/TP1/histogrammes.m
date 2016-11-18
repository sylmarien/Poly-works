%*************************************************************************%
% Fichier : histogrammes.m                                                %
% Description : Script effectuant l'amélioration d'image de la photo de   %
%               la Lune.                                                  %
% Auteurs : Julien ANTOINE 1813026 & Maxime SCHMITT 1719088               %
% Date : 03/02/2016                                                       %
%*************************************************************************%

clear all;
close all;
clc;

%% Chargement de l'image
im = imread('Lune.tif');
classin = class(im);
im2 = im2double(im);


%% Correction de l'exposition (filtre gamma)

% Paramètre de la transformation
gamma = 0.3;

% Correction de l'exposition (Transformation gamma)
im3 = im2.^gamma;

% Affichage
figure(1);
subplot(2, 2, 1);
imshow(im2);
title('Image originale');
subplot(2, 2, 2);
imhist(im2);
title('Image originale');
subplot(2, 2, 3);
imshow(im3);
title(['Image après transformation gamma (\gamma = ',num2str(gamma),')']);
subplot(2, 2, 4);
imhist(im3);
title(['Image après transformation gamma (\gamma = ',num2str(gamma),')']);

% Ecriture du fichier
% imwrite(im3,'luneClaire.png');


%% Affinage de l'image et égalisation d'histogramme

% Paramètres 
sigma = 0.5;                 % Ecart-type du filtre gaussien
c = 10;                      % Coefficient de réhaussement
taille = 2*ceil(2*sigma)+1;  % Taille du filtre (nombre impair >= 3)

% Affinage de l'image
% Application du filtre gaussien d'écart type sigma
im3floue = imgaussfilt(im3,sigma,'FilterSize',taille);

% Différence entre l'image originale et l'image floue
diff = im3 - im3floue;
% Affinage de l'image (rehaussement)
im4 = im3 + c * diff;

m = find(imhist(im4)~=0,1,'last');
im5 = im4 .* (256/m);


% Affichage
figure(2);
subplot(2, 2, 1);
imshow(im4);
title('Image affinée');
subplot(2, 2, 2);
imhist(im4);
title('Histogramme de l''image affinée');
subplot(2, 2, 3);
imshow(im5);
title('Image d''histogramme étalé');
subplot(2, 2, 4);
imhist(im5);
title('Histogramme étalé');

% Ecriture du fichier
% imwrite(im4,['netteG',num2str(sigma),'T',num2str(taille),'C',num2str(c),'.png']);

%% Égalisation de l'histogramme
im6 = histeq(im5, 255);

m = find(imhist(im6)~=0,1,'last');
im6 = im6 .* (256/m);

figure(3);
subplot(1, 2, 1);
imshow(im6);
title('Image égalisée');
subplot(1, 2, 2);
imhist(im6);
title('Histogramme de l''image égalisée');