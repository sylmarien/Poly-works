function imc = EPEGc(im, alpha)
%
% Fonction permettant le codage "EPEG" d’une image à 256 niveaux de gris
%
% Paramètres d’entrée
% im : image à coder, de type uint8
% alpha : facteur multiplicatif de la table de normalisation JPEG,
% compris entre 10^(-1) et 10
%
% Paramètre de sortie
% imc : image codée. Structure comprenant les champs suivants
% M : nombre de lignes de l’image. uint16
% N : nombre de colonnes de l’image. uint16
% Facteur : entier compris entre -128 et 127 codant alpha selon une
% échelle logarithmique. int8
% Code : suite des codes. Tableau int8 1D de taille variable
%

load('EPEG_Q.mat');

% On stocke les nombres de lignes et colonnes
imc.M = size(im,1);
imc.N = size(im,2);
% Ancienne formule (ne correspond pas à la demande de l'énoncé)
% imc.Facteur = ceil(-128 + 255*(log(alpha) - log(0.1))/(log(10) - log(0.1)));
imc.Facteur = int8(128 * log10(alpha));

% On rajoute le nombre de lignes et colonnes nécessaires pour que les
% dimensions soient des multiples de 8
dm = mod(imc.M,8);
dn = mod(imc.N,8);
im = padarray(im, [mod(8-dm,8) mod(8-dn,8)],'post');

% On centre les valeurs de l'image sur 0 (valeurs entre -128 et 127)
im = int8(im) - 128;

% On applique la dct2, puis la normalisation JPEG sur les blocs de 8 par 8
im = blkproc(im, [8,8],'dct2');
myfct = @(x) int8(double(x)./(alpha*Q));
im = blkproc(im,[8,8],myfct);

% On réordonne les coefficients selon l'ordre en "zigzag"
im = blkproc(im, [8,8],'zigzag');

% On effectue le codage par plage des coefficients
imc.Code = [];
for bloc = 0:64:numel(im)-64+1
    current = 1;
    previousZero = 0;
    zeroCpt = 0;
    code64 = zeros(1,64);
    for iter = 1:64
        % Si on trouve un zero on le rajoute au compte du nombre de zeros
        if im(bloc + iter) == 0
            zeroCpt = zeroCpt + 1;
            previousZero = 1;
        % Si on ne trouve pas un zero mais que le précédent coefficient était
        % un zéro on rajoute le compte de zéros consécutifs passés
        elseif previousZero == 1
            code64(current:current+1) = [-128 zeroCpt];
            zeroCpt = 0;
            previousZero = 0;
            current = current + 2;
            
            % Et on rajoute le coefficient non nul courant
            % Pour éviter la confusion avec les 0, on remplace les -128 par
            % -127 (perte d'information nécessaire)
            if im(bloc + iter) == -128
                code64(current) = -127;
            else
                code64(current) = im(bloc + iter);
            end
            current = current + 1;
        % Sinon on rajoute le coefficient
        else
            % Pour éviter la confusion avec les 0, on remplace les -128 par
            % -127 (perte d'information nécessaire)
            if im(bloc + iter) == -128
                code64(current) = -127;
            else
                code64(current) = im(bloc + iter);
            end
            current = current + 1;
        end
    end
    last = current;
    
    code64(last)=-128;
    code64(last+1)=-128;
    last = last+1;
    
%     % Si le dernier élément ajouté est un 0, alors on remplace le compte
%     % par un autre -128 (fin de bloc)
%     if code64(last-1) == -128
%         code64(last) = -128;
%     % Si le dernier élément entré n'est pas un 0, on rajoute à la fin du
%     % code deux -128 (fin de bloc)
%     else
%         code64(last+1)=-128;
%         code64(last+2)=-128;
%         last = last+2;
%     end
    imc.Code = [imc.Code code64(1:last)];
end

end

