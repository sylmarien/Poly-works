im = imread('Barbara.tif');

%% Exercice 1
ims = mae_ppv(im,0.2);
imwrite(ims,'BarbaraScaled.tif');
% subplot(1,2,1);
% imshow(im)
% subplot(1,2,2)
% imshow(ims)

%% Exercice 2
filtreMoyen = fspecial('average');
filtrePondere = [0.05 0.1 0.05; 0.1 0.4 0.1; 0.05 0.1 0.05];
medfilt2(im)

%% Exercice 3
histo = imhist(im);
maxHisto = find(histo ~= 0, 1, 'last');
egalise = histeq(im);
im2 = mat2gray(im);
gamma = 0.35;
im2 = im2.^gamma;
% im2 = histeq(im2);
% im2 = medfilt2(im2);

subplot(1,2,1)
imshow(im)
title('Image originale')
subplot(1,2,2)
imshow(im2)
title('Image égalisée manuellement')
