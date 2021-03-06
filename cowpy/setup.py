
from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext
import numpy as np

# Helpful info on linker environment:
# http://bit.ly/Nsvato

config = {
    'COW_HDF5': 0,
    'COW_HDF5_MPI': 0,
    'COW_FFTW': 0,
    'COW_MPI': 0,
    'include_dirs': [ ],
    'library_dirs': [ ],
    'libraries': [ ],
    'extra_compile_args': [ ],
    'extra_link_args': [ ] }

try:
    import cow_config
    config.update(cow_config.config)
    print "Using system config"
except:
    print "No system config, using default settings"

config['include_dirs'] += ['../include', np.get_include()]
config['library_dirs'] += ['../lib']
config['libraries'] += ['cow']

def make_ext(name, sources):
    return Extension(
        name,
        extra_compile_args = ['-std=c99'] + config['extra_compile_args'],
        extra_link_args    = config['extra_link_args'],
        define_macros      = [a for a in config.items() if a[0].startswith('COW')],
        include_dirs       = config['include_dirs'],
        library_dirs       = config['library_dirs'],
        libraries          = config['libraries'],
        sources            = sources)

cowpy = make_ext('cowpy', sources=['cowpy.pyx'])
srhdpack = make_ext('srhdpack', sources=['srhdpack.pyx'])

setup(name        = 'cowpy',
      version     = '0.5.0',
      author      = "Jonathan Zrake",
      description = """C.O.W. Parallel analysis tools for the Cube of Wonder""",
      ext_modules = [cowpy, srhdpack],
      cmdclass    = {'build_ext': build_ext})
