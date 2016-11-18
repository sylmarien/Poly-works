%% Compression EPEG
clear all; clc;
imCameraman = imread('cameraman.tif');
imPaolina = imread('Paolina.gif');
alphav = 0.1:0.1:10;

quadErrorCameraman = [];
nonZipSizesCameraman = [];
zipSizesCameraman = [];
quadErrorPaolina = [];
nonZipSizesPaolina = [];
zipSizesPaolina = [];
Ncameraman = size(imCameraman,1)*size(imCameraman,2)*size(imCameraman,3);
Npaolina = size(imPaolina,1)*size(imPaolina,2)*size(imPaolina,3);

for alpha=alphav
    % Compression puis d�compression de l'image
    EPEGcCameraman = EPEGc(imCameraman, alpha);
    EPEGcPaolina = EPEGc(imPaolina, alpha);
    EPEGdCameraman = EPEGd(EPEGcCameraman);
    EPEGdPaolina = EPEGd(EPEGcPaolina);
    
    % Calcul de l'erreur quadratique moyenne
    SSEcameraman = (EPEGdCameraman(:,:,:,:) - imCameraman(:,:,:,:)).^2;
    SSEcameraman = double(sum(SSEcameraman(:)))/double(Ncameraman);
    SSEPaolina = (EPEGdPaolina(:,:,:,:) - imPaolina(:,:,:,:)).^2;
    SSEPaolina = double(sum(SSEPaolina(:)))/double(Npaolina);
    quadErrorCameraman = [quadErrorCameraman SSEcameraman];
    quadErrorPaolina = [quadErrorPaolina SSEPaolina];
    
    % Stockage et compression
    save('EPEGcCameraman.mat', 'EPEGcCameraman', '-mat', '-v6');
    save('EPEGcPaolina.mat', 'EPEGcPaolina', '-mat', '-v6');
    zip('EPEGcCameraman.zip','EPEGcCameraman.mat');
    zip('EPEGcPaolina.zip','EPEGcPaolina.mat');
    
    % Poids associ�s
    nonZipCameraman = dir('EPEGcCameraman.mat');
    nonZipSizeCameraman = nonZipCameraman.bytes;
    nonZipSizesCameraman = [nonZipSizesCameraman nonZipSizeCameraman];
    zipCameraman = dir('EPEGcCameraman.zip');
    zipSizeCameraman = zipCameraman.bytes;
    zipSizesCameraman = [zipSizesCameraman zipSizeCameraman];
    
    nonZipPaolina = dir('EPEGcPaolina.mat');
    nonZipSizePaolina = nonZipPaolina.bytes;
    nonZipSizesPaolina = [nonZipSizesPaolina nonZipSizePaolina];
    zipPaolina = dir('EPEGcPaolina.zip');
    zipSizePaolina = zipPaolina.bytes;
    zipSizesPaolina = [zipSizesPaolina zipSizePaolina];
    
end

delete('EPEGcCameraman.mat', 'EPEGcCameraman.zip', 'EPEGcPaolina.mat', 'EPEGcPaolina.zip');

% Sauvegarde des variables
save('EPEG.mat', 'nonZipSizesCameraman', 'quadErrorCameraman', 'zipSizesCameraman',...
    'nonZipSizesPaolina', 'quadErrorPaolina', 'zipSizesPaolina', 'alphav');

%% Compression JPEG
clear all; clc;
imCameraman = imread('cameraman.tif');
imPaolina = imread('Paolina.gif');
qualv = 100:-1:1;

jpegSizesCameraman = [];
zipJpegSizesCameraman = [];
JPEGquadErrorCameraman = [];
jpegSizesPaolina = [];
zipJpegSizesPaolina = [];
JPEGquadErrorPaolina = [];
Ncameraman = size(imCameraman,1)*size(imCameraman,2)*size(imCameraman,3);
Npaolina = size(imPaolina,1)*size(imPaolina,2)*size(imPaolina,3);

for qual=qualv
    % Compression et zip
    % Cameraman
    JPEGsizeCameraman = compjpg('JPEGcCameraman.jpg', imCameraman, qual);
    jpegSizesCameraman = [jpegSizesCameraman JPEGsizeCameraman];
    zip('JPEGcCameraman.zip','JPEGcCameraman.jpg');
    zipJpegCameraman = dir('JPEGcCameraman.zip');
    zipJpegSizeCameraman = zipJpegCameraman.bytes;
    zipJpegSizesCameraman = [zipJpegSizesCameraman zipJpegSizeCameraman];
    % Paolina
    JPEGsizePaolina = compjpg('JPEGcPaolina.jpg', imPaolina, qual);
    jpegSizesPaolina = [jpegSizesPaolina JPEGsizePaolina];
    zip('JPEGcPaolina.zip','JPEGcPaolina.jpg');
    zipJpegPaolina = dir('JPEGcPaolina.zip');
    zipJpegSizePaolina = zipJpegPaolina.bytes;
    zipJpegSizesPaolina = [zipJpegSizesPaolina zipJpegSizePaolina];
    
    delete('JPEGcCameraman.zip', 'JPEGcPaolina.zip')
    
    % Calcul de l'erreur quadratique moyenne
    imJPEGcameraman = imread('JPEGcCameraman.jpg');
    imJPEGpaolina = imread('JPEGcPaolina.jpg');
    SSEcameraman = (imJPEGcameraman(:,:,:,:) - imCameraman(:,:,:,:)).^2;
    SSEcameraman = double(sum(SSEcameraman(:)))/double(Ncameraman);
    SSEPaolina = (imJPEGpaolina(:,:,:,:) - imPaolina(:,:,:,:)).^2;
    SSEPaolina = double(sum(SSEPaolina(:)))/double(Npaolina);
    JPEGquadErrorCameraman = [JPEGquadErrorCameraman SSEcameraman];
    JPEGquadErrorPaolina = [JPEGquadErrorPaolina SSEPaolina];
    
    delete('JPEGcCameraman.jpg', 'JPEGcPaolina.jpg');
end

% Sauvegarde des variables
save('JPEG.mat', 'jpegSizesCameraman', 'JPEGquadErrorCameraman', 'zipJpegSizesCameraman',...
    'jpegSizesPaolina', 'JPEGquadErrorPaolina', 'zipJpegSizesPaolina');

%% Trac� des courbes
clear all; clc;

load('EPEG.mat');
load('JPEG.mat');

% Cameraman
figure; hold on
plot(nonZipSizesCameraman, quadErrorCameraman, 'red');
plot(zipSizesCameraman, quadErrorCameraman, 'blue');
plot(jpegSizesCameraman, JPEGquadErrorCameraman, 'green');
plot(zipJpegSizesCameraman, JPEGquadErrorCameraman, 'cyan');
title('Erreur quadratique en fonction de la taille de l''image compress�e (cameraman)');
legend('EPEG non zipp�', 'EPEG zipp�', 'JPEG non zipp�', 'JPEG zipp�', 'Location', 'southeast');
xlabel('Poids en octets de l''image');
ylabel('Erreur quadratique moyenne');
saveas(gcf,'../rapport/images/quadraticErrorCameraman.png');

% Paolina
figure; hold on
plot(nonZipSizesPaolina, quadErrorPaolina, 'red');
plot(zipSizesPaolina, quadErrorPaolina, 'blue');
plot(jpegSizesPaolina, JPEGquadErrorPaolina, 'green');
plot(zipJpegSizesPaolina, JPEGquadErrorPaolina, 'cyan');
title('Erreur quadratique en fonction de la taille de l''image compress�e (Paolina)');
legend('EPEG non zipp�', 'EPEG zipp�', 'JPEG non zipp�', 'JPEG zipp�', 'Location', 'southeast');
xlabel('Poids en octets de l''image');
ylabel('Erreur quadratique moyenne');
saveas(gcf,'../rapport/images/quadraticErrorPaolina.png');

% Note : les points de plus grand poids correspondent aux points de plus
% PETIT alpha ! (alpha=0.1 � droite et alpha=10 � gauche)

% Effet de la compression zip sur les images originales
zip('cameraman.zip','cameraman.tif');
zipCameraOriginal = dir('cameraman.zip');
sizeZipCameraOriginal = zipCameraOriginal.bytes;
CameraOriginal = dir('cameraman.tif');
sizeCameraOriginal = CameraOriginal.bytes;
zip('Paolina.zip','Paolina.gif');
zipPaolinaOriginal = dir('Paolina.zip');
sizeZipPaolinaOriginal = zipPaolinaOriginal.bytes;
PaolinaOriginal = dir('Paolina.gif');
sizePaolinaOriginal = PaolinaOriginal.bytes;

fprintf('Cameraman :\n taille originale = %d   |   taille zipp�e = %d\n', sizeCameraOriginal, sizeZipCameraOriginal);
fprintf('Paolina :\n taille originale = %d   |   taille zipp�e = %d\n', sizePaolinaOriginal, sizeZipPaolinaOriginal);

delete('cameraman.zip', 'Paolina.zip');

% R�cup�ration des alpha optimaux pour affichage de la meilleure image compress�e
[~,alphaOptiCameramanIndex] = min(quadErrorCameraman);
[~,alphaOptiPaolinaIndex] = min(quadErrorPaolina);
alphaOptiCameraman = alphav(alphaOptiCameramanIndex);
alphaOptiPaolina = alphav(alphaOptiPaolinaIndex);

%% Compression des images (Pour toi Julien, pas obligatoire de le mettre dans le rapport)
% Compression EPEG
imCameraman = imread('cameraman.tif');
imPaolina = imread('Paolina.gif');

EPEGcCameraman = EPEGc(imCameraman, alphaOptiCameraman);
EPEGcPaolina = EPEGc(imPaolina, alphaOptiPaolina);

EPEGdCameraman = EPEGd(EPEGcCameraman);
EPEGdPaolina = EPEGd(EPEGcPaolina);

% Compressions JPEG donnant un poids proche de la compression EPEG avec
% alpha optimal (trouv� � la main avec les donn�es calcul�es pr�c�demment)
compjpg('JPEGcCameraman.jpg', imCameraman, 73);
compjpg('JPEGcPaolina.jpg', imPaolina, 82);

jpegCameraman = imread('JPEGcCameraman.jpg');
jpegPaolina = imread('JPEGcPaolina.jpg');

delete('JPEGcCameraman.jpg', 'JPEGcPaolina.jpg');

figure;
subplot(1,3,1);
imshow(EPEGdCameraman);
title('Image compress�e avec EPEG');
subplot(1,3,2);
imshow(imCameraman);
title('Image originale');
subplot(1,3,3);
imshow(jpegCameraman);
title('Image compress�e avec JPEG');
saveas(gcf,'../rapport/images/cameramanEPEGComparaison.png');

figure;
subplot(1,3,1);
imshow(EPEGdPaolina);
title('Image compress�e avec EPEG');
subplot(1,3,2);
imshow(imPaolina);
title('Image originale');
subplot(1,3,3);
imshow(jpegPaolina);
title('Image compress�e avec JPEG');
saveas(gcf,'../rapport/images/paolinaEPEGComparaison.png');


