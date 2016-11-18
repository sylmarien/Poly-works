%% Exercice 2
im = imread('Voiture.tif');
[M,N,~] = size(im);
imFreq = fft2(im);
imFreq = fftshift(imFreq);
imFreq = abs(imFreq);
imFreq = log(imFreq+1);
imFreq = mat2gray(imFreq);
subplot(1,2,1)
imshow(im)
title('Image originale')
subplot(1,2,2)
imshow(imFreq)
title('Image dans le domaine fréquentiel')
axis on

%% Exercice 3
im = imread('Lune2.tif');
[M,N,~] = size(im);
% Paramètres
fc = 5;						% Fréquence de coupure en pixels
type = 'butter';            % Type du filtre (butter,ideal ou gauss)
pad = 0;                    % Nombre de pixels de padding
ordre = 2;                  % Ordre du filtre
c = 1;                      % Coefficient de masquage

% Rehaussement par masquage flou dans le domaine de Fourier
% [ims, HH] = filtpb(im, fc, type, pad, ordre);   % Filtrage passe-bas
% diff = im-ims;              % Différence entre image originale et image filtrée
% imr = im + c*diff;          % Rehaussement

% Rehaussement par laplacien dans le domaine spectral
u0 = M/2; v0 = N/2;             % Fréquences nulle
[u,v] = ndgrid(1:M,1:N);
duv = ((u-u0).^2 + (v-v0).^2);	% Calcul de D(u,v) (p10/26 du chapitre 4)
duv = duv/max(max(duv));		% Normalisation de D(u,v)
laplacien = -4*pi^2*duv;		% Calcul du Laplacien (p21/26 du chapitre 4)
fuv = fftshift(fft2(im));		% Calcul de la transformée de Fourier de l'image
fxy = ifft2(ifftshift(laplacien.*fuv));	% On repasse dans le domaine spatial
imr = double(im) + c*abs(fxy);	% Rehaussement

% Rehaussement par laplacien dans le domaine spatial
laplace = [0 -1 0; -1 4 -1; 0 -1 0];	% Noyau laplacien
laplacian = imfilter(im,laplace);		% Filtrage de l'image par le noyau
ims = im + c*laplacian;					% Rehaussement
% imr = uint8(imr);

% Affichage
subplot(1,2,1)
imagesc(imr)
title('Image rehaussée fréquentiellement')
axis off;
subplot(1,2,2)
imagesc(ims)
% imshow(imr)
title('Image rehaussée spatialement')
axis off;

