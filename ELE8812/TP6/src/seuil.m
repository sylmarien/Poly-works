function ims = seuil(im, s)
%
% Segmentation d'une image par seuillage
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
% Syntaxe : ims = seuil(im, s)
%
%           im : image à traiter 
%           s : scalaire ou vecteur contenant la valeur des seuils, en ordre
%               croissant 
%
%           ims : image seuillée (même taille et même type que im)
%

classin = class(im);
A = double(im);

m = min(A(:));
M = max(A(:));

v = [m; s(:); M];
niveaux = (v(2:length(v)) + v(1:length(v)-1)) / 2;

B = niveaux(1) * ones(size(A));

for niv = 2:length(niveaux)
  I = find((A(:) > v(niv)) & (A(:) <= v(niv+1)));
  B(I) = niveaux(niv) * ones(size(I));
end

eval(['ims = ', classin, '(B);'])

return

