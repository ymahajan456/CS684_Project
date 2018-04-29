from setuptools import setup

setup(name = 'hexy',
      version = '0.1',
      description = 'Hexapod Robot Code',
      license = 'MIT',
      packages = ['hexy', 'hexy.robot', 'hexy.demo', 'hexy.comm'],
      zip_safe = False)
