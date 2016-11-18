function [ XA, XV, XT, YA, YV, YT ] = create_train_valid_test_splits(X, Y)

    learningSize = floor(0.7 * length(X));
    validationSize = floor(0.15 * length(X));
    testingSize = length(X) - learningSize - validationSize;
    
    XAsub = zeros(size(X));
    XVsub = zeros(size(X));
    
    availableIndexes = 1:length(X);
    
    for i=1:learningSize
        index = randi(length(availableIndexes));
        XAsub(:,availableIndexes(index)) = ones(size(X,1),1);
        availableIndexes(index) = [];
    end
    
    for i=1:validationSize
        index = randi(length(availableIndexes));
        XVsub(:,availableIndexes(index)) = ones(size(X,1),1);
        availableIndexes(index) = [];
    end
    
    XTsub = logical(ones(size(X)) - XAsub - XVsub);
    XAsub = logical(XAsub);
    XVsub = logical(XVsub);
    
    % Formation des subsets sous forme matricielle
    XA = X(XAsub);
    XA = reshape(XA, size(X,1), learningSize);
    YA = Y(XAsub(1:4,:)');
    YA = reshape(YA, learningSize, size(Y,2));
    XV = X(XVsub);
    XV = reshape(XV, size(X,1), validationSize);
    YV = Y(XVsub(1:4,:)');
    YV = reshape(YV, validationSize, size(Y,2));
    XT = X(XTsub);
    XT = reshape(XT, size(X,1), testingSize);
    YT = Y(XTsub(1:4,:)');
    YT = reshape(YT, testingSize, size(Y,2));
end

