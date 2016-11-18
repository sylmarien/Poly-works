function [XA, YA, batches] = createMiniBatches( XA, YA, K, Theta)

    permutations = randperm(size(XA,2));
    
    % On "mélange" XA et YA de la même façon pour faire des extractions
    % aléatoires
    XA = XA(:,permutations);
    YA = YA(permutations,:);
    
    % Création d'une structure contenant les K batches
    batches = struct('X', {}, 'Y', {}, 'pYgivenX', {}, 'Zx', {});
    batchesSize = floor(size(XA,2)/K);
    % Extractions des valeurs pour les K-1 premiers batches (qui sont de
    % même taille)
    for k=1:K-1
        batches(k).X = XA(:,((k-1)*batchesSize+1):((k-1)*batchesSize+1)+batchesSize-1);
        batches(k).Y = YA(((k-1)*batchesSize+1):((k-1)*batchesSize+1)+batchesSize-1,:);
    end
    % Extraction du dernier batch (qui peut être de taille différente des
    % autres
    batches(K).X = XA(:,((K-1)*batchesSize+1):end);
    batches(K).Y = YA(((K-1)*batchesSize+1):end,:);
    
    % Calcul des pYgivenX pour chacun des batches
    for k=1:K
        Xk = batches(k).X;
        Yk = batches(k).Y;
        Bk = size(Xk,2);
        
        pYgivenX = exp(Theta*Xk);
        Zx = zeros(1, Bk);
        for i = 1:Bk
            Zx(i) = sum(pYgivenX(:,i));
            pYgivenX(:,i) = pYgivenX(:,i) / Zx(i);
        end
        
        batches(k).pYgivenX = pYgivenX;
        batches(k).Zx = Zx;
    end
end

