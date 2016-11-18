% ------------------------------------------------------------- %
% ----------- ELE8812 - TP4: Analyse multirésolution ---------- %
%  Julien ANTOINE - 1813026			Maxime SCHMITT - 1805815	%
% ------------------------------------------------------------- %
% -------- Exercice 3: Transformée en ondelettes rapide ------- %
% ------------------------------------------------------------- %


clear all;
% close all;
clc;

im = imread('Lenna.tif');

%%
[c1,s1] = wavedec2(im,3,'db1');
[c4,s4] = wavedec2(im,3,'db4');
imr1 = waverec2(c1,s1,'db1');
imr4 = waverec2(c4,s4,'db4');



%%
% subplot(1,2,1)
% wave2gray(c1,s1);
% subplot(1,2,2)
% wave2gray(c4,s4);

%%
subplot(1,2,1)
imshow(im)
subplot(1,2,2)
imshow(uint8(imr4))

diff = abs(im-uint8(imr4));
maxDiff = max(diff(:));
imshow(diff)


%%

% [Lo_D,Hi_D,Lo_R,Hi_R] = wfilters('db4');
% subplot(2,2,1)
% stem(Lo_D)
% title('Lo\_D')
% subplot(2,2,2)
% stem(Hi_D)
% title('Hi\_D')
% subplot(2,2,3)
% stem(Lo_R)
% title('Lo\_R')
% subplot(2,2,4)
% stem(Hi_R)
% title('Hi\_R')



%%
seuil = 10;
c = c1;
% Indices des coefficients inférieurs au seuil
ind = abs(c1) < seuil;
% Suppression de ces coefficients
c(ind) = 0;
% Calcul du taux de compression
compression = sum(ind(:))/numel(c)
% Reconstitution de l'image compressée
imc = waverec2(c,s1,'db1');
erreur = sqrt(sum((double(im(:)) - double(imc(:))).^2) / sum(double(im(:)).^2))
imshow(uint8(imc))

