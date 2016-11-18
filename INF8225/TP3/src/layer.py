import numpy as np

class layer:
    def __init__(self, a, learningRate, WInitValue, entrySize, outputSize, isOutput):
        self.a = a
        self.learningRate = learningRate
        # Initialize W with random values in [-WInitValue ; WInitValue]
        self.theta = np.random.uniform(low=-WInitValue, high=WInitValue, size=(outputSize, entrySize))
        # We initialize the bias to 0
        self.theta[:,-1] = np.zeros((len(self.theta),))
        # Set the output layer type (False : hidden layer ; True : output layer)
        self.isOutput = isOutput

    # feed forward phase
    def feedForward(self, previousLayerOutput):
        # We add the bias to the input data
        self.previousLayerOutput = np.vstack((previousLayerOutput, np.array(np.ones([1, previousLayerOutput.shape[1]]))))
        self.thetaX = np.dot(self.theta, self.previousLayerOutput)

        # if the layer is the output layer we use softmax
        if self.isOutput:
            self.output = self.softMax(self.thetaX)
            return self.output
        # if the layer is a hidden layer, we use the piecewise linear function
        else:
            self.output = self.activationFunction(self.thetaX)
            return self.output

    # back propagation phase
    def backPropagation(self, nextLayerTheta, nextLayerDelta=None):
        # if the layer is the output nextLayerTheta = yExperiments
        if self.isOutput:
            yExperiments = nextLayerTheta
            self.delta = - (yExperiments - self.output)
        # if the layer us a hidden layer
        else:
            partialDerivatives = self.derivatedActivationFunction(self.thetaX)
            self.delta = np.multiply(partialDerivatives, np.dot(nextLayerTheta[:,:-1].T, nextLayerDelta))

        return self.delta

    def updateParameters(self):
        batchSize = self.previousLayerOutput.shape[1]
        # Update W
        self.theta[:,:-1] = self.theta[:,:-1] - self.learningRate * np.dot(self.delta, self.previousLayerOutput[:-1,:].T)/float(batchSize)
        # Update bias
        self.theta[:,-1] = self.theta[:,-1] - self.learningRate * np.sum(self.delta, axis=1)/float(batchSize)

    # Activation function implementation (piecewise linear)
    def activationFunction(self, x):
        return np.piecewise(x,[x<0, x>=0],[lambda x: self.a*x, lambda x: x])

    def derivatedActivationFunction(self, x):
        return np.piecewise(x,[x<0, x>=0],[self.a, 1])

    # softmax function for output layer
    def softMax(self, x):
        xMax = np.amax(x)
        sumExp = np.sum(np.exp(x - xMax), axis=0)
        logResult = x - (xMax + np.log(sumExp))
        return np.exp(logResult)
        #return np.exp(x)/ np.sum(np.exp(x), axis=0)
