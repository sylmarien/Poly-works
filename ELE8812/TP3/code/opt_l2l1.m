% Restauration d'images, crit�re quadratique p�nalis� par un terme L2L1,
% domaine spatial

% CE SCRIPT EST � COMPL�TER

% Initialiser h (RI de la d�gradation) et ims (image d�grad�e)


% Restauration

im0 =                        % Image initiale de la proc�dure d'optimisation
lambda1 =                    % Coefficient de pond�ration, terme portant sur f
lambda2 =                    % Coefficient de pond�ration, terme portant sur 
                             % les diff�rences premi�res de f
delta =                      % Param�tre d'�chelle

StopTol =                    % Diminution de la norme du gradient (par 
                             % rapport � la solution uniform�ment nulle) �
                             % atteindre pour arr�ter les it�rations


par = struct('im', double(ims), 'h', h, 'lambda1', lambda1, ...
    'lambda2', lambda2, 'delta', delta);

[imr, info] = minimisation_nl('cg_l2l1', par, im0, StopTol);
fhat = imr(:,end); 
