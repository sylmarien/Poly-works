import gzip
import pickle
import math
import numpy as np

'''
Function that load the experiments data and returns the various data sets
'''
def dataLoad(filePath):
    with gzip.open(filePath, 'rb') as f:
        train_set, valid_set, test_set = pickle.load(f, encoding='latin1')
    train_x, train_y = train_set
    valid_x, valid_y = valid_set
    text_x, text_y = test_set
    return train_x, train_y, valid_x, valid_y, text_x, text_y


'''
Function that returns the two given data sets after having permuted them in the same way
'''
def dataShuffle(dataSetEntries, dataSetResults):
    # Shuffle the data sets
    assert len(dataSetEntries) == len(dataSetResults)
    p = np.random.permutation(len(dataSetEntries))
    return dataSetEntries[p,:], dataSetResults[p,:]


'''
Function that given an entry and a result data sets and a number K returns the
lists of K batches of entry and result sets as well as the entry and result sets
ordered in the same way as the batches.
'''
def miniBatchesCreator(dataSetEntries, dataSetResults, K):
    # Create a new set of K minibatches from the dataSet (entries and results)
    dataSetEntries, dataSetResults = dataShuffle(dataSetEntries.T, dataSetResults.T)

    standardBatchSize = math.ceil(len(dataSetEntries)/K)

    # We store the mini batches in 2 lists
    batchesEntries = []
    batchesResults = []

    for i in range(K-1):
        batchesEntries.append(dataSetEntries[i*standardBatchSize:i*standardBatchSize+standardBatchSize].T)
        batchesResults.append(dataSetResults[i*standardBatchSize:i*standardBatchSize+standardBatchSize].T)
    # Last mini batch may be of different size than the previous ones so we add it separately
    batchesEntries.append(dataSetEntries[(K-1)*standardBatchSize:].T)
    batchesResults.append(dataSetResults[(K-1)*standardBatchSize:].T)

    return dataSetEntries.T, dataSetResults.T, batchesEntries, batchesResults
