function taille = compjpg(nomfic, x, qual)
%
% Compression d'une image 2D selon la norme jpeg et stockage dans un fichier
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
% Syntaxe : taille = compjpg(nomfic, x, qual)
%
%           nomfic : nom du fichier (cha�ne de caract�res) dans lequel sera
%                    stock�e l'image compress�e
%           x : image (2D) � compresser
%           qual : qualit� de l'image compress�e (entier compris entre et
%                  100) Qualit� "normale": 75
%
%           taille : taille du fichier jpeg (en bytes)
%
% Fonction appel�e : imwrite (Image Processing Toolbox)
%

imwrite(uint8(x), nomfic, 'jpg', 'Bitdepth', 8, 'Quality', qual)
D = dir(nomfic);

taille = D.bytes;

return

