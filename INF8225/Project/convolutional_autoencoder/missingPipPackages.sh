#!/bin/bash

sudo apt-get install libx11-dev gfortran libopenblas-dev liblapack-dev libtiff5-dev libjpeg8-dev zlib1g-dev libfreetype6-dev liblcms2-dev libwebp-dev tcl8.6-dev tk8.6-dev python-tk imagemagick

# nolearn depends on lasagne and Theano and will instal the version it requires for both of them
pip install -U -r https://raw.githubusercontent.com/dnouri/nolearn/master/requirements.txt
pip install -U git+https://github.com/dnouri/nolearn.git@master#egg=nolearn==0.7.git
