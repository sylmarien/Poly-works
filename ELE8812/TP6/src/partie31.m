clear all; close all; clc;

%% Variables
im = imread('cerveau.tif');
% Pour afficher en couleur
imc(:,:,1) = im;
imc(:,:,2) = im;
imc(:,:,3) = im;

% Paramètres
seuilSobel1 = 0.2;
seuilSobel2 = 0.06;
seuilLoG1 = 0.001;
seuilLoG2 = 0.01;
sigmaLoG1 = 1;
sigmaLoG2 = 3;
seuilCanny1 = [0.01 0.2];
seuilCanny2 = [0.06 0.09];
seuilCanny3 = [0.2 0.5];
seuilCanny4 = [0.05 0.19];
sigmaCanny1 = 1.2;
sigmaCanny2 = 1.6;

%% Méthode du gradient

[g1, s] = edge(im, 'Sobel', []);
gv1 = edge(im, 'Sobel', [], 'horizontal');
gh1 = edge(im, 'Sobel', [], 'vertical');

g2 = edge(im, 'Sobel', seuilSobel1);
gv2 = edge(im, 'Sobel', seuilSobel1, 'horizontal');
gh2 = edge(im, 'Sobel', seuilSobel1, 'vertical');

g3 = edge(im, 'Sobel', seuilSobel2);
gv3 = edge(im, 'Sobel', seuilSobel2, 'horizontal');
gh3 = edge(im, 'Sobel', seuilSobel2, 'vertical');

figure;
subplot(3,3,4);
imc(:,:,2) = im + 255*uint8(gh1);
imshow(imc);
title(['Gradient horizontal avec seuil par défaut (' num2str(s) ')']);
subplot(3,3,5);
imc(:,:,2) = im + 255*uint8(g1);
imshow(imc);
title(['Gradient avec seuil par défaut (' num2str(s) ')']);
subplot(3,3,6);
imc(:,:,2) = im + 255*uint8(gv1);
imshow(imc);
title(['Gradient vertical avec seuil par défaut (' num2str(s) ')']);
subplot(3,3,1);
imc(:,:,2) = im + 255*uint8(gh2);
imshow(imc);
title(['Gradient horizontal avec seuil de ' num2str(seuilSobel1)]);
subplot(3,3,2);
imc(:,:,2) = im + 255*uint8(g2);
imshow(imc);
title(['Gradient avec seuil de ' num2str(seuilSobel1)]);
subplot(3,3,3);
imc(:,:,2) = im + 255*uint8(gv2);
imshow(imc);
title(['Gradient vertical avec seuil de ' num2str(seuilSobel1)]);
subplot(3,3,7);
imc(:,:,2) = im + 255*uint8(gh3);
imshow(imc);
title(['Gradient horizontal avec seuil de ' num2str(seuilSobel2)]);
subplot(3,3,8);
imc(:,:,2) = im + 255*uint8(g3);
imshow(imc);
title(['Gradient avec seuil de ' num2str(seuilSobel2)]);
subplot(3,3,9);
imc(:,:,2) = im + 255*uint8(gv3);
imshow(imc);
title(['Gradient vertical avec seuil de ' num2str(seuilSobel2)]);

%% Méthode du Marr-Hildreth (LoG)

% Effet du changement de seuil
g1 = edge(im, 'log', seuilLoG1);
[g2, s]= edge(im, 'log', []);
g3 = edge(im, 'log', seuilLoG2);

% Effet du changement du sigma
g4 = edge(im, 'log', [], 'both', sigmaLoG1);
g5 = edge(im, 'log', [], 'both');
g6 = edge(im, 'log', [], 'both', sigmaLoG2);

figure;
subplot(2,3,1);
imc(:,:,2) = im + 255*uint8(g1);
imshow(imc);
title(['Gradient avec seuil de ' num2str(seuilLoG1) ' et sigma par défaut (2)']);
subplot(2,3,2);
imc(:,:,2) = im + 255*uint8(g2);
imshow(imc);
title(['Gradient avec seuil par défaut (' num2str(s) ') et sigma par défaut (2)']);
subplot(2,3,3);
imc(:,:,2) = im + 255*uint8(g3);
imshow(imc);
title(['Gradient avec seuil de ' num2str(seuilLoG2) ' et sigma par défaut (2)']);
subplot(2,3,4);
imc(:,:,2) = im + 255*uint8(g4);
imshow(imc);
title(['Gradient avec seuil par défaut (' num2str(s) ') et sigma de ' num2str(sigmaLoG1)]);
subplot(2,3,5);
imc(:,:,2) = im + 255*uint8(g5);
imshow(imc);
title(['Gradient avec seuil par défaut (' num2str(s) ') et sigma par défaut (2)']);
subplot(2,3,6);
imc(:,:,2) = im + 255*uint8(g6);
imshow(imc);
title(['Gradient avec seuil par défaut (' num2str(s) ') et sigma de ' num2str(sigmaLoG2)]);

%% Méthode du Canny

% Effet du changement de seuil
g1 = edge(im, 'canny', seuilCanny1);
[g2, s]= edge(im, 'canny', []);
g3 = edge(im, 'canny', seuilCanny2);

% Effet du changement du sigma
g4 = edge(im, 'canny', [], 'both', sigmaCanny1);
g5 = edge(im, 'canny', [], 'both');
g6 = edge(im, 'canny', [], 'both', sigmaCanny2);
I = ones(size(im,1),size(im,2));
figure;
subplot(2,3,1);
imc(:,:,2) = im + 255*uint8(g1);
imshow(imc);
title(['Gradient avec seuil de [' num2str(seuilCanny1) '] et sigma par défaut (2)']);
subplot(2,3,2);
imc(:,:,2) = im + 255*uint8(g4);
imshow(imc);
title(['Gradient avec seuil par défaut ([' num2str(s) ']) et sigma par défaut (2)']);
subplot(2,3,3);
imc(:,:,2) = im + 255*uint8(g3);
imshow(imc);
title(['Gradient avec seuil de [' num2str(seuilCanny2) '] et sigma par défaut (2)']);
subplot(2,3,4);
imc(:,:,2) = im + 255*uint8(g4);
imshow(imc);
title(['Gradient avec seuil par défaut ([' num2str(s) ']) et sigma de ' num2str(sigmaCanny1)]);
subplot(2,3,5);
imc(:,:,2) = im + 255*uint8(g5);
imshow(imc);
title(['Gradient avec seuil par défaut ([' num2str(s) ']) et sigma par défaut (2)']);
subplot(2,3,6);
imc(:,:,2) = im + 255*uint8(g6);
imshow(imc);
title(['Gradient avec seuil par défaut ([' num2str(s) ']) et sigma de ' num2str(sigmaCanny2)]);




