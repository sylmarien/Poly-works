clear all;
close all;
clc;

%% Variables

a = 0.1;
b = 0.1;

% On s'arrange pour que le (0,0) soit centré dans la figure
[u, v] = ndgrid(-688/2:688/2, -688/2:688/2);

%% Calcul du filtre

% Calcul de H
H = sinc((u*a + v*b)) .* exp(-1i * pi *(u*a + v*b));

% Affichage du résultat
H2 = 255*mat2gray(abs(H));
figure;
imshow(H2);
% saveas(gcf, '../images/H.png');

% Obtention de la représentation spatiale
H3 = ifft2(ifftshift(H));
figure; colormap gray
imagesc(H3);
% saveas(gcf, '../images/ifftDeH.png');

% Extraction du masque de taille réduite
[M, N] = find(im2bw(mat2gray(H3)), 1, 'last');
h = H3(1:M, 1:N);

% Affichage du masque de taille réduite
figure; colormap gray
imagesc(h);
% saveas(gcf, '../images/h.png');

%% Dégradation de l'image

im = imread('Fig0526a.tif');
figure;
imshow(im);

% Calcul de la déformation de mouvement dans le domaine spatial
im2 = double(im);
imBougeeSpatiale = imfilter(im2, h, 'replicate');

figure;
imshow(uint8(imBougeeSpatiale));
% saveas(gcf, '../images/imBougeeSpatiale.png');

% Calcul de la déformation de mouvement dans le domaine spectral
im2 = padarray(im2, [1, 1], 'post');
F = fft2(im2);

FBougee = F .* ifftshift(H);

imBougeeSpectrale = ifft2(FBougee);
imBougeeSpectrale = imBougeeSpectrale(1:688, 1:688);

figure;
imshow(mat2gray(imBougeeSpectrale));
% saveas(gcf, '../images/imBougeeSpectrale.png');

%% On choisit l'image spatiale et on lui applique un filtre gaussien

% Gaussian = normrnd(0, sqrt(10), 688, 688);

Gaussian = sqrt(10)*randn(688);

imDegradee = imBougeeSpatiale + Gaussian;

% On normalise l'histogramme pour désaturer l'image
imDegradee = mat2gray(imDegradee);

figure;
imshow(imDegradee);
% saveas(gcf, '../images/imDegradee.png');

%% Restauration dans le domaine fréquentiel (Filtre de Wiener)

imDegradee = padarray(imDegradee, [1, 1], 'post');
G = fftshift(fft2(imDegradee));
Hstar = conj(H);
HmodSquared = H .* Hstar;

lambda = 0.02;

Fchapeau = Hstar .* G ./ (HmodSquared + lambda);

imCorrigeeWiener = ifft2(ifftshift(Fchapeau));
imCorrigeeWiener = imCorrigeeWiener(1:688,1:688);

figure; colormap gray
imshow(mat2gray(abs(imCorrigeeWiener)));



