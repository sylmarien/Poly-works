function [f, trace] = minimisation_nl(cg, par, f0, varargin)
% 
% Fonction permettant de trouver numériquement la valeur de f qui minimise
% un critère J
% Interface avec la boîte à outils Problano v 1.1
%
%    Copyright (C) 2013-2015  Yves Goussard
%
% Forme d'appel :
%   [f, trace] = minimisation_nl(cg, par, f0, [GradRelTol])
%
% Paramètres d'entrée
%   cg : nom d'une fonction (chaîne de caractères) qui, pour toute valeur
%        de f, retourne la valeur du critère J et de son gradient g en 
%        fonction de paramètres contenus dans la structure par.
%        forme de la fonction : [J, g] = cg(f, par)
%   par : deuxième argument (structure) de la fonction cg
%   f0 : valeur initiale de f
%   GradRelTol : réduction relative de la norme du gradient du critère
%                provoquant l'arrêt de la procédure de minimisation.
%                Paramètre optionnel. Valeur par défaut : 1e-05
%
% Paramètres de sortie
%   f : résultat de la minimisation
%   trace : structure contenent divers indices relatifs au déroulement de
%           la procédure d'optimisation
%

% Traitement du paramètre optionnel
GradRelTolDef = 1e-05;
nvargs = length(varargin);
switch nvargs
    case 0
        GradRelTol = GradRelTolDef;
    case 1
        GradRelTol = varargin{1};
    otherwise
        error('Nombre de paramètres d''entrée trop grand')
        return
end
if isempty(GradRelTol)
    GradRelTol = GradRelTolDef;
end

% Préparation des paramètres et appel aux fonctions de minimisation
        
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
fprintf('Critère d''arrêt : ||G(X)||/N < %9.5g\n\n', StopTol)

OUT = optim(cg_l, f0, params);
f = OUT.X;
trace = OUT;
trace = rmfield(trace, 'X');
trace.StopTol = params.StopTol;
trace.RelFuncTol = params.RelFuncTol;

return
