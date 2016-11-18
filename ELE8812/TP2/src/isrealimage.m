function [fl, maxabs] = isrealimage(A)
%
% Fonction permettant de tester si l'image dont on connaît la FFT 2D A est
% à valeurs réelles
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
%
% Paramètre d'entrée :
%   A : FFT 2D de l'image, selon le format usuel de Matlab (fréquence nulle
%       en (1, 1)
%
% Paramètres de sortie :
%   maxabs : complexe dont les parties réelle et imaginaire correspondent
%            respectivement à l'écart de A vis à vis de la parité de sa
%            partie réelle et de l'imparité de sa partie imaginaire
%   fl : variable logique indiquant si l'image est à valeurs rélles ou non
%
%

fl = logical(0);
B = fftshift(A);
[M, N] = size(B);

if ~(M - 2 * floor(M/2)) % M pair
    B = [B; B(1, :)]; % Symétrisation de B
end

if ~(N - 2 * floor(N/2)) % N pair
    B = [B, B(:, 1)]; % Symétrisation de B
end

C = B - fliplr(flipud(conj(B)));

maxabs  = complex(max(abs(real(C(:)))), max(abs(imag(C(:)))));
fl = abs(maxabs) == 0;

return
