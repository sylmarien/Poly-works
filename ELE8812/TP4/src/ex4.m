% ------------------------------------------------------------- %
% ----------- ELE8812 - TP4: Analyse multirésolution ---------- %
%  Julien ANTOINE - 1813026			Maxime SCHMITT - 1805815	%
% ------------------------------------------------------------- %
% ------- Exercice 4: Correction de défauts localisés --------- %
% ------------------------------------------------------------- %
clear all;

% Paramètres
delta = 1;
a = [0.1 0.5];
b = [0.9 0.5];
im = imread('Lenna_r.tif');

%% Calcul de la transformée en ondelettes db4
lvl = 4;
[c,s] = wavedec2(im,lvl,'db4');
% wave2gray(c,s);

for n = 1:lvl
	imt = wavecopy('v', c, s, n);
	coef = suppr(imt,a,b,delta);
	c = wavepaste('v', c, s, n, coef);
end

ims = waverec2(c,s, 'db4');
subplot(1,2,1)
imshow(im)
subplot(1,2,2)
imshow(uint8(ims))

%% Calcul de la transformée en ondelettes db1
lvl = 3;
[c2,s2] = wavedec2(im,lvl,'db1');
for n = 1:lvl
	imt = wavecopy('v', c2, s2, n);
	coef = suppr(imt,a,b,delta);
	c2 = wavepaste('v', c2, s2, n, coef);
end

ims2 = waverec2(c2,s2, 'db1');
subplot(1,2,1)
imshow(im)
subplot(1,2,2)
imshow(uint8(ims2))

%% Calcul de la transformée en ondelettes sym6
lvl = 4;
delta = 1;
[c3,s3] = wavedec2(im,lvl,'sym6');
for n = 1:lvl
	imt = wavecopy('v', c3, s3, n);
	coef = suppr(imt,a,b,delta);
	c3 = wavepaste('v', c3, s3, n, coef);
end

ims3 = waverec2(c3,s3, 'sym6');
subplot(1,2,1)
imshow(im)
subplot(1,2,2)
imshow(uint8(ims3))
