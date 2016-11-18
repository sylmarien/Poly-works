function ims = mae_ppv(im, sc)
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
% Taille de l'image mise � l'�chelle voulue (ims)
Ms = round(M*sc);
Ns = round(N*sc);

% Coordonn�es des milieux des pixels de ims dans le rep�re d'ims
xs = (1:Ms)' - 1/2;
ys = (1:Ns) - 1/2;

% Coordonn�es des milieux des pixels de ims ramen�es dans le rep�re de
% l'image de d�part (im)
xsp = xs * M / Ms;
ysp = ys * N / Ns;

% Coordonn�es des pixels correspondant � l'interpolation
% (e.g. xisp(i),yisp(j) est la coordonn�e du pixel dans im qui doit �tre
% utilis� dans le pixel de coordonn�e i,j dans ims.
xisp = floor(xsp)+1;
yisp = floor(ysp)+1;

As = A(xisp, yisp);

eval(['ims = im2', classin, '(As);'])



