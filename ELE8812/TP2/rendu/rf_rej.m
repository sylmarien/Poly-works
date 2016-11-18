function HH = rf_rej(nu1, nu2, W, M, N)
%
% Fonction calculant la réponse fréquentielle d'un filtre RÉEL 2D gaussien
% coupant les fréquences situées autour de (nu1, nu2) avec une "largeur
% de bande" W.
%
% nu1, nu2 et W sont exprimés en fréquences réduites (quantités comprises
% entre -0,5 et 0,5).
%
% Paramètres d'entrées
% nu1 : première coordonnée de la fréquence à rejeter
% nu2 : deuxième coordonnée de la fréquence à rejeter
% W : largeur de la bande à rejeter
% M : nombre de lignes de la réponse fréquentielle à synthétiser
% N : nombre de colonnes de la réponse fréquentiele à synthétiser
%
% Paramètre de sortie
% HH : réponse fréquentielle du filtre. Tableau 2D de taille (M, N)

% Construction du filtre passe-bas

    % Coordonnées du centre de la représentation fréquentielle
    M0 = ceil((M+1) / 2);
    N0 = ceil((N+1) / 2);    

    [U, V] = ndgrid(1:M, 1:N);

    % On fait le choix d'avoir HH tel que la fréquence nulle
    % soit au voisinage de son centre
    D2 = (U - M0).^2 + (V - N0).^2;
    
    fc1 = sqrt((nu1*N)^2 + (nu2*M)^2);
    
    % On construit et renvoie le filtre gaussien
    HH = 1 - exp(- (D2 - fc1^2).^2 ./ ((W*(M+N)/2)^2 * D2));

end