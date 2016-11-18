# System imports
import os, sys, urllib, gzip
import argparse
import pickle
import copy
import warnings
sys.setrecursionlimit(10000)

# Printing imports
import matplotlib
#matplotlib.use('Agg') # Change matplotlib backend, in case we have no X server running..
import matplotlib.pyplot as plt
#%matplotlib inline
import numpy as np
from PIL import Image

# Basic Lasagne and Theano imports
with warnings.catch_warnings():
    warnings.simplefilter("ignore")
    from lasagne.layers import get_output, InputLayer, DenseLayer, Upscale2DLayer, ReshapeLayer
    from lasagne.nonlinearities import rectify, leaky_rectify, tanh
    from lasagne.updates import nesterov_momentum
    from lasagne.objectives import categorical_crossentropy
    from nolearn.lasagne import NeuralNet, BatchIterator, PrintLayerInfo
    from nolearn.lasagne import objective
    from nolearn.lasagne.visualize import plot_loss, plot_conv_weights, plot_conv_activity
    import theano

    # Convolutional layers import (fast cuda_convnet layers)
    from lasagne.layers import Conv2DLayer as Conv2DLayerSlow
    from lasagne.layers import MaxPool2DLayer as MaxPool2DLayerSlow
    # pylearn2 version (fastest)
    from lasagne.layers.cuda_convnet import Conv2DCCLayer as Conv2DLayerFast
    from lasagne.layers.cuda_convnet import MaxPool2DCCLayer as MaxPool2DLayerFast

    # Importing the custom functions
    from networkFunctions import get_layer_by_name, get_picture_array, compare_images, mixedLoss

parser = argparse.ArgumentParser()
parser.add_argument("--no-training", dest="noTraining", help="Set this flag if you don't want to do the training (and load an existing trained network)", action="store_true")
parser.add_argument("--network-path", dest='networkPath', help="Path to save the network once trained (or path to the network to load when --no-training is set)", default='mnist/conv_ae.pkl', type=str)
args = parser.parse_args()

# Making sureall the required directories are here. Creating them otherwise
try:
    os.mkdir('mnist')
    print('mnist directory created.')
except:
    print('mnist directory already existed. Nothing to do.')
try:
    os.mkdir('data')
    print('data directory created.')
except:
    print('data directory already existed. Nothing to do.')
try:
    os.mkdir('montage')
    print('montage directory created.')
except:
    print('montage directory already existed. Nothing to do.')

# Getting the data (if not already available) and creating data sets
fname = 'mnist/mnist.pkl.gz'
if not os.path.isfile(fname):
    testfile = urllib.request.URLopener()
    testfile.retrieve("http://deeplearning.net/data/mnist/mnist.pkl.gz", fname)
f = gzip.open(fname, 'rb')
train_set, valid_set, test_set = pickle.load(f, encoding='latin1')
f.close()
X, y = train_set

# reshape from (50000, 784) to 4D tensor (50000, 1, 28, 28)
#X = X[0:10000]
X = np.reshape(X, (-1, 1, 28, 28))
#print('X type and shape:', X.dtype, X.shape)
#print('X.min():', X.min())
#print('X.max():', X.max())

# we need our target to be 1 dimensional
X_out = X.reshape((X.shape[0], -1))
#print('X_out:', X_out.dtype, X_out.shape)

if args.noTraining:
    # Loading instead of Learning
    print('Loading existing network at ' + args.networkPath)
    ae = pickle.load(open(args.networkPath,'rb'))
    PrintLayerInfo()(ae)
else:
    # Defining the layers of the network
    print('Training new network')
    conv_num_filters = 16
    filter_size = 3
    pool_size = 2
    encode_size = 10
    dense_mid_size = 128
    pad_in = 'valid'
    pad_out = 'full'
    layers = [
        (InputLayer, {'shape': (None, X.shape[1], X.shape[2], X.shape[3])}),
        (Conv2DLayerFast, {'num_filters': conv_num_filters, 'filter_size': filter_size, 'pad': pad_in}),
        (Conv2DLayerFast, {'num_filters': conv_num_filters, 'filter_size': filter_size, 'pad': pad_in}),
        (MaxPool2DLayerFast, {'pool_size': pool_size}),
        (Conv2DLayerFast, {'num_filters': 2*conv_num_filters, 'filter_size': filter_size, 'pad': pad_in}),
        (MaxPool2DLayerFast, {'pool_size': pool_size}),
        (ReshapeLayer, {'shape': (([0], -1))}),
        (DenseLayer, {'num_units': dense_mid_size}),
        (DenseLayer, {'name': 'encode', 'num_units': encode_size}),
        (DenseLayer, {'num_units': dense_mid_size}),
        (DenseLayer, {'num_units': 800}),
        (ReshapeLayer, {'shape': (([0], 2*conv_num_filters, 5, 5))}),
        (Upscale2DLayer, {'scale_factor': pool_size}),
        (Conv2DLayerFast, {'num_filters': conv_num_filters, 'filter_size': filter_size, 'pad': pad_out}),
        (Upscale2DLayer, {'scale_factor': pool_size}),
        (Conv2DLayerSlow, {'num_filters': conv_num_filters, 'filter_size': filter_size, 'pad': pad_out}),
        (Conv2DLayerSlow, {'num_filters': 1, 'filter_size': filter_size, 'pad': pad_out}),
        (ReshapeLayer, {'shape': (([0], -1))}),
    ]
    # 2 last convolutional layers are regular "slow" ones because it is actually slower to alternate cuda and normal ones than just use normal ones
    # (Upscale2DLayer is not in cuda_convnet)

    # initializing the Neural Network
    ae = NeuralNet(
        layers=layers,
        max_epochs=100,

        #objective=mixedLoss,

        update=nesterov_momentum,
        update_learning_rate=0.01,
        update_momentum=0.975,

        regression=True,
        verbose=1
    )
    # ae.initialize()
    # PrintLayerInfo()(ae)

    # Learning phase
    ae.fit(X, X_out)

    # Plot loss function over epochs
    try:
        os.mkdir('plots')
        print('plots directory created.')
    except:
        print('plots directory already existed. Nothing to do.')
    plot_loss(ae).savefig('plots/lossFunction.png', bbox_inches='tight')

    # Saving the trained Neural Network for future usage
    # ae.save_params_to('mnist/conv_ae.np')
    print('Saving trained network at ' + args.networkPath)
    pickle.dump(ae, open(args.networkPath,'wb'))

################################################################################

# Plotting the convolutional layers' filters
plot_conv_weights(ae.get_all_layers()[1]).savefig('plots/convWeightsL1.png', bbox_inches='tight')
plot_conv_weights(ae.get_all_layers()[15]).savefig('plots/convWeightsL15.png', bbox_inches='tight')

# Getting output of the NN for the training set, reshaping to the size of an image
X_pred = ae.predict(X).reshape(-1, 28, 28)
X_pred = np.rint(256. * X_pred).astype(int)
X_pred = np.clip(X_pred, a_min = 0, a_max = 255)
X_pred = X_pred.astype('uint8')
print(os.linesep + 'Shape of predection, shape of training set')
print(X_pred.shape , X.shape)

###  show an input and an output side by side
#compare_images(X, X_pred, 2)
randomCompare = np.random.randint(50000)
compare_images(X, X_pred, randomCompare)

# We get the code using this encoder for 10 000 samples (maximum possible, more than that causes an out-of-memory error)
encode_layer, encode_layer_index = get_layer_by_name(ae, 'encode')
input_layer, input_layer_index = get_layer_by_name(ae, 'input0')
encodeFunction = theano.function([input_layer.input_var], get_output(encode_layer))
X_encoded = encodeFunction(X[0:10000])

# We create a decoder by extracting only the second part of the network
# Deepcopy to let the original neural network untouched and functioning
aeDecode = copy.deepcopy(ae)
next_layer = aeDecode.get_all_layers()[encode_layer_index + 1]
final_layer = aeDecode.get_all_layers()[-1]
# New input for the decoder
input_decode = InputLayer(shape=(None, encode_layer.num_units))
reshape_layer = ReshapeLayer(input_decode, ([0],[1]))
next_layer.input_layer = reshape_layer
# Decoding function
decodeFunction = theano.function([input_decode.input_var], get_output(final_layer))

# We use this new decoder to decode the previously encoded inputs (only one input currently)
# the '.reshape(1,X_encoded[2].shape[0])' required to avoid problem with second dimension being non-existant in numpy array (instead of 1)
randomDecode = np.random.randint(10000)
X_decoded = 256 * decodeFunction(X_encoded[randomDecode].reshape(1,X_encoded[2].shape[0]))

X_decoded = theano.tensor.iround(X_decoded)
X_decoded = theano.tensor.clip(X_decoded, 0, 255)
X_decoded  = X_decoded.astype('uint8')
print(X_decoded.shape.eval())
# Printing the decoded image
pic_array = get_picture_array(X_decoded.eval())
image = Image.fromarray(pic_array)
image.save('data/decoded' + str(randomDecode) + '.png', format="PNG")
