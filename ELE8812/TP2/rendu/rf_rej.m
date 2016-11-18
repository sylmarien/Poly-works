function HH = rf_rej(nu1, nu2, W, M, N)
%
% Fonction calculant la r�ponse fr�quentielle d'un filtre R�EL 2D gaussien
% coupant les fr�quences situ�es autour de (nu1, nu2) avec une "largeur
% de bande" W.
%
% nu1, nu2 et W sont exprim�s en fr�quences r�duites (quantit�s comprises
% entre -0,5 et 0,5).
%
% Param�tres d'entr�es
% nu1 : premi�re coordonn�e de la fr�quence � rejeter
% nu2 : deuxi�me coordonn�e de la fr�quence � rejeter
% W : largeur de la bande � rejeter
% M : nombre de lignes de la r�ponse fr�quentielle � synth�tiser
% N : nombre de colonnes de la r�ponse fr�quentiele � synth�tiser
%
% Param�tre de sortie
% HH : r�ponse fr�quentielle du filtre. Tableau 2D de taille (M, N)

% Construction du filtre passe-bas

    % Coordonn�es du centre de la repr�sentation fr�quentielle
    M0 = ceil((M+1) / 2);
    N0 = ceil((N+1) / 2);    

    [U, V] = ndgrid(1:M, 1:N);

    % On fait le choix d'avoir HH tel que la fr�quence nulle
    % soit au voisinage de son centre
    D2 = (U - M0).^2 + (V - N0).^2;
    
    fc1 = sqrt((nu1*N)^2 + (nu2*M)^2);
    
    % On construit et renvoie le filtre gaussien
    HH = 1 - exp(- (D2 - fc1^2).^2 ./ ((W*(M+N)/2)^2 * D2));

end