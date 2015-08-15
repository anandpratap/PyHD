from distutils.core import setup, Extension
import numpy as np

ext_modules = []

ext = Extension('hyperdual.numpy_hyperdual',
                sources=['hyperdual/hyperdual.c',
                         'hyperdual/numpy_hyperdual.c'],
                include_dirs=[np.get_include()],
                extra_compile_args=['-std=c99', '-O0'])
ext_modules.append(ext)



setup(name='hyperdual',
      version='0.1',
      description='NumPy Hyper Dual numbers extensions',
      packages=['hyperdual'],
      ext_modules=ext_modules)
