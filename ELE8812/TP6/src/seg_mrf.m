function [ims, mu, var] = seg_mrf(im, N)
%
% Segmentation d'une image champs de markov unilatéraux, paramétrisation
% télégraphique
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
% Syntaxe : [ims, mu, var] = seg_mrf(im, N)
%
%           im : image à traiter 
%           N : nombre de niveaux
%
%           ims : image seuillée (même taille et même type que im)
%           mu : évolution de la valeur moyenne de chaque classe au cours
%                des itérations. Tableau de taille N x NbIter+1
%           var : évolution de la variance de chaque classe au cours
%                 des itérations. Tableau de taille N x NbIter+1
%

classin = class(im);
penal = 1; % Pénalisation de la vraisemblance pour l'estimation des paramètres du modèle

y = double(im);
emtel

eval(['ims = ', classin, '(MAPimage);'])
mu = mvect;
var = sigma2vect;

return







