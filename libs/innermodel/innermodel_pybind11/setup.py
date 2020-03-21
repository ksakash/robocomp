from distutils.core import setup

import sys

setup(
  name        = 'innermodel_pybind11',
  version     = '0.0.1',
  packages    = ['innermodel_pybind11'],
  package_dir = {
    '': '${CMAKE_CURRENT_BINARY_DIR}'
  },
  package_data = {
    '': ['inner_model.cpython-36m-x86_64-linux-gnu.so']
  }
)
