function ims = mae_ppv(im, sc)
% 
% Fonction effectuant le changement d'échelle de l'image im selon le 
% facteur sc, interpolation bilinéaire.
% 
%    Copyright (C) 2008-2013  Yves Goussard
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
% Paramètres d'entrée : 
% im : image (niveaux de gris) d'un type reconnu par Matlab. Tableau 2D 
% sc : paramètre de changement d'échelle. Réel strictement positif 
% 
% Paramètre de sortie : 
% ims : image à la nouvelle échelle, de même type que im 
% 

% Test sur la validité des paramètres d'entrée

ims = [];

if sc <= 0
    disp('scale : le paramètre d''échelle doit être > 0')
    return
end

if ndims(im) ~= 2
    disp('scale : l''image doit être un tableau à deux dimensions')
    return
end

classin = class(im);

% Préservation de l'échelle

if isinteger(im)
    A = mat2gray(im, double([intmin(classin) intmax(classin)]));
else
    A = mat2gray(im);
end

[M, N] = size(A);
% Taille de l'image mise à l'échelle voulue (ims)
Ms = round(M*sc);
Ns = round(N*sc);

% Coordonnées des milieux des pixels de ims dans le repère d'ims
xs = (1:Ms)' - 1/2;
ys = (1:Ns) - 1/2;

% Coordonnées des milieux des pixels de ims ramenées dans le repère de
% l'image de départ (im)
xsp = xs * M / Ms;
ysp = ys * N / Ns;

% Coordonnées des pixels correspondant à l'interpolation
% (e.g. xisp(i),yisp(j) est la coordonnée du pixel dans im qui doit être
% utilisé dans le pixel de coordonnée i,j dans ims.
xisp = floor(xsp)+1;
yisp = floor(ysp)+1;

As = A(xisp, yisp);

eval(['ims = im2', classin, '(As);'])



