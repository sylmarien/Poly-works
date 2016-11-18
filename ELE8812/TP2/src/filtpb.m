function [ims, HH] = filtpb(im, fc, type, pad, ordre) 
% 
% Fonction effectuant le filtrage passe-bas d'une image
%
%-------------------------------------------------------------------------
%
%    Copyright (C) 2008-2015  Yves Goussard
%
%    This program is free software: you can redistribute it and/or modify
%    it under the terms of the GNU General Public License as published by
%    the Free Software Foundation, either version 3 of the License, or
%    (at your option) any later version.
%
%    This program is distributed in the hope that it will be useful,
%    but WITHOUT ANY WARRANTY; without even the implied warranty of
%    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
%    GNU General Public License for more details.
%
%    You should have received a copy of the GNU General Public License
%    along with this program.  If not, see <http://www.gnu.org/licenses/>.
% 
%-------------------------------------------------------------------------
%
% Paramètres d'entrée : 
% im : image (niveaux de gris) d'un type reconnu par Matlab. Tableau 2D 
% fc : fréquence de coupure, en nombre de pixels "fréquentiels". Doit être
%      positive
% type : type du filtre
%        'butter' : filtre de butterworth
%        'ideal' : idéal
%        'gauss' : gaussien
% pad : Nombre de pixels de padding de l'image
% ordre : ordre du filtre, si nécessaire. Defaut : 2
% 
% Paramètre de sortie : 
% ims : image filtrée, de même taille et de même type que A
% HH : RF du filtre utilisé
%

% Test sur la validité des paramètres d'entrée

ims = [];
HH = [];

if ~ exist('pad', 'var') | isempty(pad)
    pad = 0;
end

if ~ exist('ordre', 'var') | isempty(ordre)
    ordre = 2;
end

if ordre < 2
    disp('filtpb : l''ordre du filtre, si précisé doit être > 1')
    return
end

if pad < 0
    disp('filtpb : le paramètre "pad" doit être positif ou nul')
    return
end

if fc <= 0
    disp('filtpb : la fréquence de coupure doit être > 0')
    return
end

if ndims(im) ~= 2
    disp('filtpb : l''image doit être un tableau à deux dimensions')
    return
end

classin = class(im);
A = double(im);

[M, N] = size(A);
M1 = M + pad;
N1 = N + pad;

AA = fftshift(fft2(A, M1, N1));
M0 = ceil((M1+1) / 2);
N0 = ceil((N1+1) / 2);

[U, V] = ndgrid(1:M1, 1:N1);
D2 = (U - M0).^2 + (V - N0).^2;

switch lower(type)
    case 'butter'
        HH = 1 ./ (1 + (D2 / fc^2).^ordre);
        
    case 'ideal'
        HH = D2 <= fc^2;
        
    case 'gauss'
        HH = exp(-D2 / (2*fc^2));
        
    otherwise
        disp('filtpb : type de filtre non reconnu')
        return
end

BB = ifftshift(AA .* HH);
if ~ isrealimage(BB)
    disp('filtpb : mauvaises dimensions ; certaines images seront complexes')
end

B = ifft2(BB);

eval(['ims = ', classin, '(B(1:M, 1:N));'])
        
return
