import numpy as np
import theano.tensor
from lasagne.layers import get_output
from lasagne.objectives import squared_error, categorical_crossentropy, aggregate
from PIL import Image

## we find the encode layer from our ae, and use it to define an encoding function
def get_layer_by_name(net, name):
    for i, layer in enumerate(net.get_all_layers()):
        if layer.name == name:
            return layer, i
    return None, None

def get_picture_array(X, rescale=4):
    array = X.reshape(28,28)
    array = np.clip(array, a_min = 0, a_max = 255)
    return  array.repeat(rescale, axis = 0).repeat(rescale, axis = 1).astype(np.uint8())

def compare_images(X, X_pred, index):
    print(index)
    original_image = Image.fromarray(get_picture_array(255 * X[index]))
    new_size = (original_image.size[0] * 2, original_image.size[1])
    new_im = Image.new('L', new_size)
    new_im.paste(original_image, (0,0))
    rec_image = Image.fromarray(get_picture_array(X_pred[index]))
    new_im.paste(rec_image, (original_image.size[0],0))
    new_im.save('data/compareImage' + str(index) + '.png', format="PNG")

def mixedLoss(layers, lambda1=1., *args, **kwargs):
    # loss of CAE
    x = T.matrix('x')
    y = T.ivector('y')
    predictionCAE = get_output(layers[-1], x)
    lossCAE = aggregate(squared_error(predictionCAE, y), mode='mean')
    losses = lambda1 * lossCAE
    return losses
