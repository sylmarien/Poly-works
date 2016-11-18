%*************************************************************************%
% Fichier : rehaussement.m                                                %
% Description : Script effectuant un rehaussement par masquage flou       %
%				puis par Laplacien (domaines spectral et spatial)		  %
% Auteurs : Julien ANTOINE 1813026 & Maxime SCHMITT 1719088               %
% Date : 10/02/2016                                                       %
%*************************************************************************%

% Acquisition de l'image
im = imread('Lune2.tif');
[M,N,~] = size(im);

% Paramètres
fc = 50;						% Fréquence de coupure en pixels
type = 'gauss';            % Type du filtre (butter,ideal ou gauss)
pad = 0;                    % Nombre de pixels de padding
ordre = 2;                  % Ordre du filtre
c = 10;                      % Coefficient de masquage

%% Rehaussement par masquage flou
% Domaine de Fourier
[ims, HH] = filtpb(im, fc, type, pad, ordre);   % Filtrage passe-bas
diff = im - ims;              % Différence entre images originale et filtrée
imrf = im + c*diff;          % Rehaussement

% Paramètres (spatial)
sigma = 2;

% Domaine spatial
imFloue = imgaussfilt(im,sigma);
diff = im - imFloue;
imrs = im + c*diff;

% Affichage
subplot(1,2,1)
imshow(imrs)
title('Domaine spatial','FontSize',22)
subplot(1,2,2)
imshow(imrf)
title('Domaine spectral','FontSize',22)


%% Rehaussement par laplacien
% Domaine spectral
c = 0.25;
u0 = M/2; v0 = N/2;             % Fréquences nulle
[u,v] = ndgrid(1:M,1:N);
duv = ((u-u0).^2 + (v-v0).^2);	% Calcul de D(u,v) (p10/26 du chapitre 4)
duv = duv/max(max(duv));		% Normalisation de D(u,v)
laplacien = -4*pi^2*duv;		% Calcul du Laplacien (p21/26 du chapitre 4)
fuv = fftshift(fft2(im));		% Calcul de la transformée de Fourier de l'image
fxy = ifft2(ifftshift(laplacien.*fuv));	% On repasse dans le domaine spatial
imr = double(im) + c*abs(fxy);	% Rehaussement
% imr = imr/max(max(imr));
imr=uint8(imr);

% Domaine spatial
laplace = [0 -1 0; -1 4 -1; 0 -1 0];	% Noyau laplacien
% laplace = [-1 -1 -1; -1 8 -1; -1 -1 -1];	% Noyau laplacien
laplacian = imfilter(im,laplace);		% Filtrage de l'image par le noyau
ims = im + c*laplacian;					% Rehaussement


% Affichage
subplot(1,2,1)
imshow(imr)
title('Domaine spectral','FontSize',30)
axis off;
subplot(1,2,2)
imshow(ims)
title('Domaine spatial','FontSize',30)
axis off;

%% Enregistrement des fichiers
imwrite(imr,['SpectralC',num2str(c),'.png']);
imwrite(ims,['SpatialC',num2str(c),'.png']);