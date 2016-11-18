function ims = filtreImage(im, taille, typeFiltre)
% Fonction appliquant un filtre de de type et de taille voulus

% Param�tres d'entr�e : 
% im : image (niveaux de gris) d'un type reconnu par Matlab. Tableau 2D 
% taille : dimension de la matrice du filtre (3, 5, 9, etc). Entier impair
% sup�rieur � 3
% typeFiltre : type de filtre � appliquer (simple, ponderee ou median)
% 
% Param�tre de sortie : 
% ims : image filtr�e, de m�me type que im
% 

ims = [];

classin = class(im);

im = im2double(im);

switch typeFiltre
    case 'simple'
        filtre = fspecial('average', taille);
        ims = conv2(im, filtre);
    case 'ponderee'
        filtre = fspecial('gaussian', taille);
        ims = conv2(im, filtre);
    case 'median'
        ims = medfilt2(im);
    otherwise
        error('Utilisez une valeur de filtre de moyenne, ponderee ou median.');
        
eval(['ims = im2', classin, '(ims);'])
end

