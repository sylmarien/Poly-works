function imd = EPEGd(imc)
%
% Fonction permettant le décodage "EPEG" d’une image à 256 niveaux de gris
%
% Paramètre d’entrée
% imc : image à décoder. Structure comprenant les champs suivants
% M : nombre de lignes de l’image. uint16
% N : nombre de colonnes de l’image. uint16
% Facteur : entier compris entre -128 et 127 codant alpha selon une
% échelle logarithmique. int8
% Code : suite des codes. Tableau int8 1D de taille variable
%
% Paramètre de sortie
% imd : image décodée. Tableau uint8 de taille (M, N)
%

load('EPEG_Q.mat');

% On retrouve alpha (à l'erreur numérique près)
% Ancienne formule (ne correspond pas à la demande de l'énoncé)
% alpha = exp((imc.Facteur + 128)*(log(10)-log(0.1))/255 + log(0.1));
alpha = double(10).^(double(imc.Facteur)/128);

ims = zeros((imc.M+mod(8-mod(imc.M,8),8))*(imc.N+mod(8-mod(imc.N,8),8)),1);
i = 1;
iter = 1;
while iter <= numel(imc.Code)
    % Si le coefficient est différent de -128 on le recopie tel quel et on
    % passe au prochain coefficient
    if (imc.Code(iter) ~= -128)
        ims(i) = imc.Code(iter);
        i = i+1;
        iter = iter+1;
    % Sinon si ce n'est pas la fin d'un bloc, on recopie autant de 0 que
    % spécifié et on saute le prochain coefficient (qui est le nombre de 0)
    elseif imc.Code(iter+1) ~= -128
        ims(i:i+imc.Code(iter+1)-1) = 0;
        i = i + imc.Code(iter+1);
        iter = iter+2;
    % Sinon on recopie autant de 0 que nécessaire pour finir le bloc et on
    % saute le prochain coefficient (qui signifie la fin du bloc)
    else
        ims(i:i+mod(64-mod(i-1,64),64)-1) = 0;
        i = i+mod(64-mod(i-1,64),64);
        iter = iter+2;
    end
end

% On utilise dezigzag sur ims pour passer d'un vecteur unidimensionnel
% à la matrice des blocs
ims = reshape(ims, [], (imc.N+mod(8-imc.N,8))/8);
imd = blkproc(ims, [64,1], 'dezigzag');

% On "dénormalise" avec la table de normalisation et le coefficient alpha
myfct = @(x) x.*(alpha*Q);
imd = blkproc(imd,[8,8],myfct);

% On applique la transformée en cosinus discrète inverse
imd = blkproc(imd, [8,8],'idct2');

% On replace les éléments entre 0 et 256 puis on retire les éléments liés
% au padding
imd = imd + 128;
imd = uint8(imd(1:imc.M, 1:imc.N));

end

