% ------------------------------------------------------------- %
% ----------- ELE8812 - TP4: Analyse multirésolution ---------- %
%  Julien ANTOINE - 1813026			Maxime SCHMITT - 1805815	%
% ------------------------------------------------------------- %
% ------- Exercice 4: Correction de défauts localisés --------- %
% ------------------------------------------------------------- %
clear all;

% Paramètres
delta = 2;
lvl = 4;
a = [0.1 0.4];
b = [0.9 0.6];
im = imread('Lenna_s.tif');


% Calcul de la transformée en ondelettes
[c3,s3] = wavedec2(im,lvl,'sym6');
for n = 1:lvl
	for t = ['v', 'd']
		imt = wavecopy(t, c3, s3, n);
		coef = suppr(imt,a,b,delta);
		c3 = wavepaste(t, c3, s3, n, coef);
	end
end

ims3 = waverec2(c3,s3, 'sym6');
subplot(1,2,1)
imshow(im)
subplot(1,2,2)
imshow(uint8(ims3))
