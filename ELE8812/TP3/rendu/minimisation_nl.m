function [f, trace] = minimisation_nl(cg, par, f0, varargin)
% 
% Fonction permettant de trouver num�riquement la valeur de f qui minimise
% un crit�re J
% Interface avec la bo�te � outils Problano v 1.1
%
%    Copyright (C) 2013-2015  Yves Goussard
%
% Forme d'appel :
%   [f, trace] = minimisation_nl(cg, par, f0, [GradRelTol])
%
% Param�tres d'entr�e
%   cg : nom d'une fonction (cha�ne de caract�res) qui, pour toute valeur
%        de f, retourne la valeur du crit�re J et de son gradient g en 
%        fonction de param�tres contenus dans la structure par.
%        forme de la fonction : [J, g] = cg(f, par)
%   par : deuxi�me argument (structure) de la fonction cg
%   f0 : valeur initiale de f
%   GradRelTol : r�duction relative de la norme du gradient du crit�re
%                provoquant l'arr�t de la proc�dure de minimisation.
%                Param�tre optionnel. Valeur par d�faut : 1e-05
%
% Param�tres de sortie
%   f : r�sultat de la minimisation
%   trace : structure contenent divers indices relatifs au d�roulement de
%           la proc�dure d'optimisation
%

% Traitement du param�tre optionnel
GradRelTolDef = 1e-05;
nvargs = length(varargin);
switch nvargs
    case 0
        GradRelTol = GradRelTolDef;
    case 1
        GradRelTol = varargin{1};
    otherwise
        error('Nombre de param�tres d''entr�e trop grand')
        return
end
if isempty(GradRelTol)
    GradRelTol = GradRelTolDef;
end

% Pr�paration des param�tres et appel aux fonctions de minimisation
        
HERE = fileparts(mfilename('fullpath'));
addpath(fullfile(HERE, 'poblano_toolbox_1.1'))

optim = @lbfgs;

eval(['cg_l = @(f) ' cg, '(f, par);'])
[J0, g0] = cg_l(zeros(size(f0)));
StopTol = norm(g0) / length(g0) * GradRelTol;
M = 10;

params = optim('defaults');
params.StopTol = StopTol;
params.RelFuncTol = 1e-10;
params.TraceFunc = true;
params.TraceGradNorm = true;
if isfield(params, 'M')
    params.M = M;
end

fprintf('\nMinimisation par Poblano V 1.1 - Algorithme L-BFGS\n')
fprintf('Crit�re d''arr�t : ||G(X)||/N < %9.5g\n\n', StopTol)

OUT = optim(cg_l, f0, params);
f = OUT.X;
trace = OUT;
trace = rmfield(trace, 'X');
trace.StopTol = params.StopTol;
trace.RelFuncTol = params.RelFuncTol;

return
