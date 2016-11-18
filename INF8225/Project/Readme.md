
Based mainly on the tutorials found at :
- [https://lepisma.github.io/articles/2015/07/30/up-with-theano-and-cuda/](Also provides scripts to test that the installation actually works)
- [https://www.ibm.com/developerworks/community/blogs/jfp/entry/Installing_PyCUDA_On_Anaconda_For_Windows?lang=en](correct installation for PyCuda in particular)
- [https://groups.google.com/a/continuum.io/forum/#!topic/anaconda/hPRIt9WUaeg](recommendations for the use of conda and pip together)
- [https://groups.google.com/forum/#!msg/caffe-users/VvIKNbb_Iw0/OX4JEqsd5dwJ](simple installation of cuDNN by copying the files in the CUDA directories)
- [http://deeplearning.net/software/pylearn2/](pylearn2 install)
- [https://sourceforge.net/p/mingw-w64/wiki2/Compile%20pthreads/](pthreads for Windows/MinGW)

## Installation of Theano, Lasagne, nolearn with CUDA and cuDNN on Windows 10

1. Install Anaconda 3 (Python distribution with lots of dependencies included)
2. Install Microsoft Visual Studio 2013 (for CUDA 7.5 support) and add `cl.exe` path to the PATH environment variable (`C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin`)
3. Install CUDA 7.5 (NVidia installer express install)
4. Create conda virtual environment with the basic dependencies  
  ```python
  conda create -n projectName mingw libpython
  ```
5. Activate environment  
  ```python
  activate projectName
  ```
6. Install 'complex' dependencies (that would require some manual work and building if not using Anaconda)  
  ```python
  conda install scipy==0.16.1
  ```
7. Install packages unhandled by conda with pip  
  ```python
  pip install -U -r https://raw.githubusercontent.com/dnouri/nolearn/master/requirements.txt
  pip install -U git+https://github.com/dnouri/nolearn.git@master#egg=nolearn==0.7.git
  ```
8. Install other packages (could be preferable to use conda here instead of pip. Especially jupyter needs to be install through conda)  
  ```python
  pip install -r requirement.txt
  ```
9. Download (http://www.lfd.uci.edu/~gohlke/pythonlibs/#pycuda and download correct version*) and install PyCUDA  
  ```python
  pip install pycuda-2016.1+cuda7518-cp34-cp34m-win_amd64.whl
  ```
10. Create `.theanorc` file in the HOME directory (`%userprofile%` on Windows) with following content  
    [global]
    floatX = float32
    device = gpu
    [nvcc]
    flags=-LC:\Anaconda\libs
    compiler_bindir=C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin
11. Check that it works until this point (Theano is installed and loads) in `ipython` (output should look like the second line)  
  ```python
  > import theano
  Using gpu device 0: GeForce GT 630M (CNMeM is disabled)
  ```
12. More intensive test of Theano  
  ```python
  from theano import function, config, shared, sandbox
  import theano.tensor as T
  import numpy
  import time
  vlen = 10 * 30 * 768  # 10 x #cores x # threads per core
  iters = 1000
  rng = numpy.random.RandomState(22)
  x = shared(numpy.asarray(rng.rand(vlen), config.floatX))
  f = function([], T.exp(x))
  print f.maker.fgraph.toposort()
  t0 = time.time()
  for i in xrange(iters):
      r = f()
  t1 = time.time()
  print 'Looping %d times took' % iters, t1 - t0, 'seconds'
  print 'Result is', r
  if numpy.any([isinstance(x.op, T.Elemwise) for x in f.maker.fgraph.toposort()]):
      print 'Used the cpu'
  else:
      print 'Used the gpu'
  ```
Output should look something like :  
  ```python
  Using gpu device 0: GeForce GT 630M (CNMeM is disabled)
  [GpuElemwise{exp,no_inplace}(<CudaNdarrayType(float32, vector)>), HostFromGpu(GpuElemwise{exp,no_inplace}.0)]
  Looping 1000 times took 1.42199993134 seconds
  Result is [ 1.23178029  1.61879349  1.52278066 ...,  2.20771813  2.29967761
    1.62323296]
  Used the gpu
  ```
13. Checking that PyCUDA works  
  ```python
  import pycuda.autoinit
  import pycuda.driver as drv
  import numpy

  from pycuda.compiler import SourceModule
  mod = SourceModule("""
  __global__ void multiply_them(float *dest, float *a, float *b)
  {
    const int i = threadIdx.x;
    dest[i] = a[i] * b[i];
  }
  """)

  multiply_them = mod.get_function("multiply_them")

  a = numpy.random.randn(400).astype(numpy.float32)
  b = numpy.random.randn(400).astype(numpy.float32)

  dest = numpy.zeros_like(a)
  multiply_them(
          drv.Out(dest), drv.In(a), drv.In(b),
          block=(400,1,1), grid=(1,1))

  print dest-a*b
  ```
Should output an array of zeros.
14. Donwload and unzip cuDNN (https://developer.nvidia.com/rdp/cudnn-download)
15. Copy cuDNN files into corresponding (cuDNN's bin directory -> CUDA's bin directory, etc) directories of CUDA (`C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v7.5`)
16. Check that cuDNN is loading by importing Theano once again  
  ```python
  import theano
  ```
17. (Not working yet : problem with pthread dependency) Install pylearn2  
  ```
  git clone git://github.com/lisa-lab/pylearn2.git
  python setup.py develop
  ```
And add `PYLEARN2_DATA_PATH` to environment variable with a value pointing to a directory that will contain the pylearn2 data sets.
18. (Not figured out yet. That's what blocks everything right now) Adding pthread files to nvcc
(Currently : Copied include (headers) files of pthread_win32 in the include directory of CUDA and added dll files to path (still not working))
https://pachonomicon.wordpress.com/2015/02/26/how-to-get-the-theano-wrapper-from-pylearn-2-for-krizhevskys-cuda-convnet-working-on-64bit-python-3-4-on-windows-8-1/
https://pachonomicon.wordpress.com/category/oh-exploitable/
(STILL NOPE)


* Correct version explanation : pycuda-2016.1+cuda7518-cp34-cp34m-win_amd64.whl
- cuda7518 : CUDA version (7.5 is what interests us here)
- cp34-cp34m : Python version (3.4 here)
- win_amd64 : 64 bits architecture
