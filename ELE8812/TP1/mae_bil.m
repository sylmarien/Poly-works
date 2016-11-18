function ims = mae_bil(im, sc)
% 
% Fonction effectuant le changement d'�chelle de l'image im selon le 
% facteur sc, interpolation bilin�aire.
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
% Param�tres d'entr�e : 
% im : image (niveaux de gris) d'un type reconnu par Matlab. Tableau 2D 
% sc : param�tre de changement d'�chelle. R�el strictement positif 
% 
% Param�tre de sortie : 
% ims : image � la nouvelle �chelle, de m�me type que im 
% 

% Test sur la validit� des param�tres d'entr�e

ims = [];

if sc <= 0
    disp('scale : le param�tre d''�chelle doit �tre > 0')
    return
end

if ndims(im) ~= 2
    disp('scale : l''image doit �tre un tableau � deux dimensions')
    return
end

classin = class(im);

% Pr�servation de l'�chelle

if isinteger(im)
    A = mat2gray(im, double([intmin(classin) intmax(classin)]));
else
    A = mat2gray(im);
end

[M, N] = size(A);
Ap = padarray(A, [1 1], 'symmetric');

x = (1:M)' - 1/2; % Coordonn�es des pixels de A
y = (1:N) - 1/2; % 

xp = (0:M+1)' - 1/2; % Coordonn�es des pixels de Ap
yp = (0:N+1) - 1/2; % Ap : A �tendue d'un pixel dans toutes les directions

Ms = round(M*sc); % Taille de l'image mise � l'�chelle ims
Ns = round(N*sc); %

xs = (1:Ms)' - 1/2; % Coordonn�es des pixels de ims
ys = (1:Ns) - 1/2; % 

xsp = xs * M / Ms; % Coordonn�es des pixels de ims
ysp = ys * N / Ns; % ramen�es dans le rep�re de l'image de d�part

xsp = xsp + 3/2; % Coordonn�es des pixels de ims
ysp = ysp + 3/2; % ramen�es dans le rep�re "naturel" de Ap


[Xsp, Ysp] = ndgrid(xsp, ysp);
Xfsp = Xsp - floor(Xsp); % Coefficients pour l'interpolation
Yfsp = Ysp - floor(Ysp); % 

xisp = floor(xsp); % points (x, y) utilis�s pour l'interpolation
yisp = floor(ysp); %

As = (1-Xfsp) .* (1-Yfsp) .* Ap(xisp, yisp) + ...
    Xfsp .* (1-Yfsp) .* Ap(xisp+1, yisp) + ...
    (1-Xfsp) .* Yfsp .* Ap(xisp, yisp+1) + ...
    Xfsp .* Yfsp .* Ap(xisp+1, yisp+1); % Interpolation bilin�aire

eval(['ims = im2', classin, '(As);'])



