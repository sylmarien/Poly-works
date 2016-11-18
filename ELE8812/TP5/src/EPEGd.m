function imd = EPEGd(imc)
%
% Fonction permettant le d�codage "EPEG" d�une image � 256 niveaux de gris
%
% Param�tre d�entr�e
% imc : image � d�coder. Structure comprenant les champs suivants
% M : nombre de lignes de l�image. uint16
% N : nombre de colonnes de l�image. uint16
% Facteur : entier compris entre -128 et 127 codant alpha selon une
% �chelle logarithmique. int8
% Code : suite des codes. Tableau int8 1D de taille variable
%
% Param�tre de sortie
% imd : image d�cod�e. Tableau uint8 de taille (M, N)
%

load('EPEG_Q.mat');

% On retrouve alpha (� l'erreur num�rique pr�s)
% Ancienne formule (ne correspond pas � la demande de l'�nonc�)
% alpha = exp((imc.Facteur + 128)*(log(10)-log(0.1))/255 + log(0.1));
alpha = double(10).^(double(imc.Facteur)/128);

ims = zeros((imc.M+mod(8-mod(imc.M,8),8))*(imc.N+mod(8-mod(imc.N,8),8)),1);
i = 1;
iter = 1;
while iter <= numel(imc.Code)
    % Si le coefficient est diff�rent de -128 on le recopie tel quel et on
    % passe au prochain coefficient
    if (imc.Code(iter) ~= -128)
        ims(i) = imc.Code(iter);
        i = i+1;
        iter = iter+1;
    % Sinon si ce n'est pas la fin d'un bloc, on recopie autant de 0 que
    % sp�cifi� et on saute le prochain coefficient (qui est le nombre de 0)
    elseif imc.Code(iter+1) ~= -128
        ims(i:i+imc.Code(iter+1)-1) = 0;
        i = i + imc.Code(iter+1);
        iter = iter+2;
    % Sinon on recopie autant de 0 que n�cessaire pour finir le bloc et on
    % saute le prochain coefficient (qui signifie la fin du bloc)
    else
        ims(i:i+mod(64-mod(i-1,64),64)-1) = 0;
        i = i+mod(64-mod(i-1,64),64);
        iter = iter+2;
    end
end

% On utilise dezigzag sur ims pour passer d'un vecteur unidimensionnel
% � la matrice des blocs
ims = reshape(ims, [], (imc.N+mod(8-imc.N,8))/8);
imd = blkproc(ims, [64,1], 'dezigzag');

% On "d�normalise" avec la table de normalisation et le coefficient alpha
myfct = @(x) x.*(alpha*Q);
imd = blkproc(imd,[8,8],myfct);

% On applique la transform�e en cosinus discr�te inverse
imd = blkproc(imd, [8,8],'idct2');

% On replace les �l�ments entre 0 et 256 puis on retire les �l�ments li�s
% au padding
imd = imd + 128;
imd = uint8(imd(1:imc.M, 1:imc.N));

end

