import numpy as np

import dataManagement

# Function that compute the output of the neural network for the given x input
def networkFeedForward(x, hiddenLayers, outputLayer):
    # feedForward
    outputPreviousLayer = x
    for layeri in hiddenLayers:
        outputPreviousLayer = layeri.feedForward(outputPreviousLayer)
    outputNetwork = outputLayer.feedForward(outputPreviousLayer)
    return outputNetwork

def networkBackPropagation(outputNetwork, y, hiddenLayers, outputLayer):
    thetaL = outputLayer.theta
    deltaL = outputLayer.backPropagation(y)
    for layeri in reversed(hiddenLayers):
        deltaL = layeri.backPropagation(thetaL, deltaL)
        thetaL = layeri.theta

def networkUpdateParameters(hiddenLayers, outputLayer):
    for layeri in hiddenLayers:
        layeri.updateParameters()
    outputLayer.updateParameters()

# OK
def lossFunction(networkOutput, y):
    return (- np.sum(np.multiply(y, np.log(networkOutput))))

# OK
def accuracyCalculator(networkOutput, y):
    (numberOfClasses, sampleSize) = networkOutput.shape
    multinomialNetworkOutput = np.zeros([numberOfClasses, sampleSize])
    for i in range(sampleSize):
        multinomialNetworkOutput[np.argmax(networkOutput[:,i]), i] = 1
    correctClasses = np.sum(np.multiply(multinomialNetworkOutput, y))
    return correctClasses / sampleSize

def learningParameters(train_x, train_y, validation_x, validation_y, convergenceThreshold, K, hiddenLayers, outputLayer):
    lossList = []
    validationAccuracyList = []
    trainingAccuracyList = []
    # Initialize lossFunction value
    print("Before learning : ")
    validationOutput = networkFeedForward(validation_x, hiddenLayers, outputLayer)
    loss = lossFunction(validationOutput, validation_y)
    lossList = lossList + [loss]
    validationAccuracy = accuracyCalculator(validationOutput, validation_y)
    validationAccuracyList = validationAccuracyList + [validationAccuracy]
    print("Loss value : " + str(loss))
    print("Accuracy on the validation set : " + str(validationAccuracy))

    print("\nBeginning of the learning phase")
    nbIter = 1
    converged = False
    while not converged:
        # create mini batches
        train_x, train_y, train_xBatches, train_yBatches = dataManagement.miniBatchesCreator(train_x, train_y, K)

        # Feed forward + back propagation phases for all the batches
        for (x,y) in zip(train_xBatches, train_yBatches):

            # feedForward
            outputNetwork = networkFeedForward(x, hiddenLayers, outputLayer)

            # backPropagation
            networkBackPropagation(outputNetwork, y, hiddenLayers, outputLayer)

            # Update the parameters
            networkUpdateParameters(hiddenLayers, outputLayer)

        # Checking convergence using the validation set
        # Loss function
        previousLoss = loss
        validationOutput = networkFeedForward(validation_x, hiddenLayers, outputLayer)
        loss = lossFunction(validationOutput, validation_y)
        lossList = lossList + [loss]
        lossDiff = abs((previousLoss - loss)/previousLoss)

        print("\nInteration number : " + str(nbIter))
        nbIter = nbIter + 1
        print("Loss value : " + str(loss))
        print("Difference of loss values : " + str(lossDiff))

        # Accuracies (validation and training)
        trainingOutput = networkFeedForward(train_x, hiddenLayers, outputLayer)
        trainingAccuracy = accuracyCalculator(trainingOutput, train_y)
        trainingAccuracyList = trainingAccuracyList + [trainingAccuracy]
        print("Accuracy on the training set : " + str(trainingAccuracy))
        validationAccuracy = accuracyCalculator(validationOutput, validation_y)
        validationAccuracyList = validationAccuracyList + [validationAccuracy]
        print("Accuracy on the validation set : " + str(validationAccuracy))

        # Check convergence and modify converged accordingly
        if lossDiff < convergenceThreshold:
            converged = True
    return lossList, trainingAccuracyList, validationAccuracyList, nbIter
