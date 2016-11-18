function [J, g] = cg_l2l1(imr, par)
%
% Fonction calculant le crit�re gaussien p�nalis� par un terme L2L1 utilis�
% en restauration d'images, ainsi que son gradient
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
% L'hypoth�se de convolution est "corr�lation" (sortie de m�me taille que
% l'entr�e, avec pr�fen�trage par des z�ros pour la taille de la RI)
%
% Param�tres d'entr�e
%  imr : valeur courante de l'estim�e de l'image restaur�e. Vecteur (MN, 1)
%  par : structure comportant les champs suivants
%  im : image � restaurer. Tableau r�el de taille (M, N)
%  h : PSF de la fonction de d�gradation
%  lambda1 : param�tre de r�gularisation, terme de rappel �z�ro
%  lambda2 : param�tre de r�gularisation, diff�rences premi�res
%  delta :  facteur d'�chelle du terme semi-quadratique
%          
% Param�tres de sortie
%   J : crit�re
%   g : gradient de J
%

im = par.im;
h = par.h;
lambda1 = par.lambda1;
lambda2 = par.lambda2;
delta = par.delta;

[M, N] = size(im);
[P, Q] = size(h);

if (M*N ~= length(imr))
    fprintf('cg_l2l2 -- Erreur : tailles de im et imr incoh�rentes')
    return
end

imr = reshape(imr, M, N);
ht = flipdim(flipdim(h, 1), 2);

% Valeur du crit�re

predic = ifft2(fft2(imr, M+P-1, N+Q-1) .* fft2(h, M+P-1, N+Q-1));
predic = predic(1:M, 1:N);

v2 = im - predic;

ux = imr(1:M-1, :) - imr(2:M, :);
uy = imr(:, 1:N-1) - imr(:, 2:N);

J = v2(:)' * v2(:) + ...
    lambda1 * sum(sqrt(delta^2 + imr(:).^2)) + ...
    lambda2 * (sum(sqrt(delta^2 + ux(:).^2)) + sum(sqrt(delta^2 + uy(:).^2)));

% Valeur du gradient

B0 = imr ./ sqrt(delta^2 + imr.^2);

bx = zeros(size(imr));
bx(1:M-1, :) = ux ./ sqrt(delta^2 + ux.^2);
Bx = bx - circshift(bx, [1, 0]);

by = zeros(size(imr));
by(:, 1:N-1) = uy ./ sqrt(delta^2 + uy.^2);
By = by - circshift(by, [0, 1]);

gmq = ifft2(fft2(v2, M+P-1, N+Q-1) .* fft2(ht, M+P-1, N+Q-1));
g = -2 * gmq(P:end, Q:end) + lambda1 * B0 + lambda2 * (Bx + By);

g = g(:);
