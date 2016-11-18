%*************************************************************************%
% Fichier : amelioration.m                                                %
% Description : Script effectuant l'am�lioration d'image de la photo de   %
%               la Lune.                                                  %
% Auteurs : Julien ANTOINE 1813026 & Maxime SCHMITT 1719088               %
% Date : 03/02/2016                                                       %
%*************************************************************************%

clear all;
close all;
clc;

%% Chargement de l'image
im = imread('Lune.tif');
im2 = im2double(im);


%% Correction de l'exposition (filtre gamma)

% Param�tre de la transformation
gamma = 0.3;

% Correction de l'exposition (Transformation gamma)
im3 = im2.^gamma;

% m : dernier niveau non nul dans l'histogramme
m = find(imhist(im3),1,'last');
% �talement de l'histogramme
im4 = im3 .* (256/m);

% Affichage
figure(1);
subplot(3, 2, 1);
imshow(im2);
title('Image originale');
subplot(3, 2, 2);
imhist(im2);
title('Image originale');
subplot(3, 2, 3);
imshow(im3);
title(['Image apr�s transformation gamma (\gamma = ',num2str(gamma),')']);
subplot(3, 2, 4);
imhist(im3);
title(['Histogramme apr�s transformation gamma (\gamma = ',num2str(gamma),')']);
subplot(3, 2, 5);
imshow(im4);
title(['Image apr�s �talement d''histogramme']);
subplot(3, 2, 6);
imhist(im4);
title(['Histogramme apr�s �talement']);

% Ecriture du fichier
% imwrite(im3,'luneClaire.png');


%% Affinage de l'image et �galisation d'histogramme

% Param�tres 
sigma = 0.5;                 % Ecart-type du filtre gaussien
c = 10;                      % Coefficient de r�haussement
taille = 2*ceil(2*sigma)+1;  % Taille du filtre (nombre impair >= 3)

% Affinage de l'image
% Application du filtre gaussien d'�cart type sigma
im4floue = imgaussfilt(im4,sigma,'FilterSize',taille);

% Diff�rence entre l'image originale et l'image floue
diff = im4 - im4floue;
% Affinage de l'image (rehaussement)
im5 = im4 + c * diff;

% Affichage
figure(2);
subplot(3, 2, 1);
imshow(im2);
title('Image originale');
subplot(3, 2, 2);
imhist(im2);
title('Image originale');
subplot(3, 2, 3);
imshow(im4);
title('Image � l''�tape pr�c�dente');
subplot(3, 2, 4);
imhist(im4);
title('Histogramme de l''image � l''�tape pr�c�dente');
subplot(3, 2, 5);
imshow(im5);
title('Image affin�e');
subplot(3, 2, 6);
imhist(im5);
title('Histogramme de l''image affin�e');

% Ecriture du fichier
% imwrite(im4,['netteG',num2str(sigma),'T',num2str(taille),'C',num2str(c),'.png']);

%% �galisation de l'histogramme
im6 = histeq(im5);

% Apr�s �galisation, on constate que seule une partie des tons de gris est
% utilis�e, on "�tale" � nouveau l'histogramme pour corriger cela.
min = find(imhist(im6),1);
max = find(imhist(im6),1, 'last');
im7 = (im6 - min/256) .* (256/(max - min));

figure(3);
subplot(2, 2, 1);
imshow(im6);
title('Image �galis�e');
subplot(2, 2, 2);
imhist(im6);
title('Histogramme �galis�');
subplot(2, 2, 3);
imshow(im7);
title('Image apr�s �talement d''histogramme');
subplot(2, 2, 4);
imhist(im7);
title('Histogramme apr�s �talement');

%% Affichage comparatif avant/apr�s traitement
figure(4);
subplot(1, 2, 1);
imshow(im2);
title('Image originale');
subplot(1, 2, 2);
imshow(im7);
title('Image finale');