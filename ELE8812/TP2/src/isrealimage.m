function [fl, maxabs] = isrealimage(A)
%
% Fonction permettant de tester si l'image dont on conna�t la FFT 2D A est
% � valeurs r�elles
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
% Param�tre d'entr�e :
%   A : FFT 2D de l'image, selon le format usuel de Matlab (fr�quence nulle
%       en (1, 1)
%
% Param�tres de sortie :
%   maxabs : complexe dont les parties r�elle et imaginaire correspondent
%            respectivement � l'�cart de A vis � vis de la parit� de sa
%            partie r�elle et de l'imparit� de sa partie imaginaire
%   fl : variable logique indiquant si l'image est � valeurs r�lles ou non
%
%

fl = logical(0);
B = fftshift(A);
[M, N] = size(B);

if ~(M - 2 * floor(M/2)) % M pair
    B = [B; B(1, :)]; % Sym�trisation de B
end

if ~(N - 2 * floor(N/2)) % N pair
    B = [B, B(:, 1)]; % Sym�trisation de B
end

C = B - fliplr(flipud(conj(B)));

maxabs  = complex(max(abs(real(C(:)))), max(abs(imag(C(:)))));
fl = abs(maxabs) == 0;

return
