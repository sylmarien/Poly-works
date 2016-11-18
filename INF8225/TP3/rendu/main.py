# Script principal. Exécute l'apprentissage en utilisant les différentes fonctions

import os
import sys
import argparse
import numpy as np
import matplotlib.pyplot as plt

import dataManagement
import layer
import learningModule

# Parse command line arguments
parser = argparse.ArgumentParser()
parser.add_argument("miniBatchesNumber", help="Number of mini batches to use (strictly positive integer)", type=int)
parser.add_argument("learningRate", help="Learning rate to use for learning (strictly positive number)", type=float)
parser.add_argument("activationParameter", help="Coefficient for the piecewise linear activation function (positive number)", type=float)
parser.add_argument("WInitValue", help="Value such that initial values in W are in [-WInitValue ; WInitValue] (strictly positive number)", type=float)
parser.add_argument("convergenceThreshold", help="Convergence threshold for the learning (strictly positive number)", type=float)
parser.add_argument("Nlayers", help="Number of layers in the Neural Network (> 1)", type=int)
parser.add_argument("sizeLayers", help="Sizes of the layers (strictly positive integer)", type=int, nargs='+')
args = parser.parse_args()
if (args.miniBatchesNumber <= 0):
    print('Number of mini batches must be a strictly positive integer')
    exit(1)
if (args.learningRate <= 0):
    print('The learning rate must be a strictly positive integer')
    exit(2)
if (args.activationParameter < 0):
    print('The coefficient for the activation function must be a positive integer')
    exit(3)
if (args.WInitValue <= 0):
    print('WInitValue must be a strictly positive integer')
    exit(4)
if (args.convergenceThreshold <= 0):
    print('convergenceThreshold must be a strictly positive integer')
    exit(5)
if (args.Nlayers <= 0):
    print('Number of layers should be > 1')
    exit(6)
if len(args.sizeLayers) != args.Nlayers:
    print('Number of sizes given doesn\'t match number of layers')
    exit(7)
elif (any(sizeLayer <= 0 for sizeLayer in args.sizeLayers)):
    print('Layers sizes must be strictly positive integers')
    exit(8)

miniBatchesNumber = args.miniBatchesNumber
learningRate = args.learningRate
activationParameter = args.activationParameter
WInitValue = args.WInitValue
Nlayers = args.Nlayers
sizeLayers = args.sizeLayers
convergenceThreshold = args.convergenceThreshold

# Get the data
train_x,train_y,valid_x,valid_y,test_x,test_y = dataManagement.dataLoad('mnist.pkl.gz')
train_x = np.asarray(train_x.T)
valid_x = np.asarray(valid_x.T)
test_x = np.asarray(test_x.T)

# Initialize parameters
# Converting Y vectors to multinomial vectors
outputSize = 10
mTrain_Y = np.zeros([len(train_y), outputSize])
mValid_Y = np.zeros([len(valid_y), outputSize])
mTest_Y = np.zeros([len(test_y), outputSize])
for i in range(len(train_y)):
    mTrain_Y[i, train_y[i]] = 1
for i in range(len(valid_y)):
    mValid_Y[i, valid_y[i]] = 1
for i in range(len(test_y)):
    mTest_Y[i, test_y[i]] = 1

mTrain_Y = mTrain_Y.T
mValid_Y = mValid_Y.T
mTest_Y = mTest_Y.T

(entrySize, _) = train_x.shape

# Initialize Neural Network

# Hidden layers
hiddenLayers = []
i = len(sizeLayers) - 1
previousSizeLayer = entrySize
for sizeLayer in sizeLayers:
    hiddenLayers.append(layer.layer(activationParameter, learningRate, WInitValue, previousSizeLayer+1, sizeLayer, False))
    previousSizeLayer = sizeLayer

# Output layer (connected to the last hidden layer)
outLayer = layer.layer(activationParameter, learningRate, WInitValue, sizeLayers[-1]+1, outputSize, True)

# Learning phase
#learningModule.learningParameters(train_x, mTrain_Y, valid_x, mValid_Y, convergenceThreshold, miniBatchesNumber, hiddenLayers, outLayer)
lossList, trainingAccuracyList, validationAccuracyList, nbIter = learningModule.learningParameters(train_x, mTrain_Y, valid_x, mValid_Y, convergenceThreshold, miniBatchesNumber, hiddenLayers, outLayer)

# Result on testing set
testingOutput = learningModule.networkFeedForward(test_x, hiddenLayers, outLayer)

testingAccuracy = learningModule.accuracyCalculator(testingOutput, mTest_Y)

print("\n\nAccuracy of the model on the testing data set : " + str(testingAccuracy) + "\n\n")

#plt.plot(range(nbIter), lossList)
#plt.legend(['Loss function'], loc='upper right')
#plt.savefig('../rapport/images/lossFunction.png', bbox_inches='tight')
#plt.close()
#plt.plot(range(nbIter), validationAccuracyList)
#plt.plot(range(nbIter-1), trainingAccuracyList)
#plt.legend(['Accuracy on validation set', 'Accuracy on training set'], loc='lower right')
#plt.savefig('../rapport/images/accuracy.png', bbox_inches='tight')
