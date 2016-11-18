% Restauration d'images, critère quadratique pénalisé par un terme L2L1,
% domaine spatial

% CE SCRIPT EST À COMPLÉTER

% Initialiser h (RI de la dégradation) et ims (image dégradée)


% Restauration

im0 =                        % Image initiale de la procédure d'optimisation
lambda1 =                    % Coefficient de pondération, terme portant sur f
lambda2 =                    % Coefficient de pondération, terme portant sur 
                             % les différences premières de f
delta =                      % Paramètre d'échelle

StopTol =                    % Diminution de la norme du gradient (par 
                             % rapport à la solution uniformément nulle) à
                             % atteindre pour arrêter les itérations


par = struct('im', double(ims), 'h', h, 'lambda1', lambda1, ...
    'lambda2', lambda2, 'delta', delta);

[imr, info] = minimisation_nl('cg_l2l1', par, im0, StopTol);
fhat = imr(:,end); 
