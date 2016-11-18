function ims = mae_bil(im, sc)
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
Ap = padarray(A, [1 1], 'symmetric');

x = (1:M)' - 1/2; % Coordonnées des pixels de A
y = (1:N) - 1/2; % 

xp = (0:M+1)' - 1/2; % Coordonnées des pixels de Ap
yp = (0:N+1) - 1/2; % Ap : A étendue d'un pixel dans toutes les directions

Ms = round(M*sc); % Taille de l'image mise à l'échelle ims
Ns = round(N*sc); %

xs = (1:Ms)' - 1/2; % Coordonnées des pixels de ims
ys = (1:Ns) - 1/2; % 

xsp = xs * M / Ms; % Coordonnées des pixels de ims
ysp = ys * N / Ns; % ramenées dans le repère de l'image de départ

xsp = xsp + 3/2; % Coordonnées des pixels de ims
ysp = ysp + 3/2; % ramenées dans le repère "naturel" de Ap


[Xsp, Ysp] = ndgrid(xsp, ysp);
Xfsp = Xsp - floor(Xsp); % Coefficients pour l'interpolation
Yfsp = Ysp - floor(Ysp); % 

xisp = floor(xsp); % points (x, y) utilisés pour l'interpolation
yisp = floor(ysp); %

As = (1-Xfsp) .* (1-Yfsp) .* Ap(xisp, yisp) + ...
    Xfsp .* (1-Yfsp) .* Ap(xisp+1, yisp) + ...
    (1-Xfsp) .* Yfsp .* Ap(xisp, yisp+1) + ...
    Xfsp .* Yfsp .* Ap(xisp+1, yisp+1); % Interpolation bilinéaire

eval(['ims = im2', classin, '(As);'])



