function [ RL1, dRL1, RL2, dRL2 ] = regularizationTerms( Theta )

    W = Theta(:, 1:end-1);
    RL1 = sum(abs(W(:)));
    dRL1 = [sign(W) zeros(size(Theta,1), 1)];
    RL2 = sumsqr(W);
    dRL2 = [(2*W) zeros(size(Theta,1), 1)];

end

