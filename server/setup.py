# create an auto rule
from setuptools import setup

setup( name='p4p1',
        version='0.1',
        description='p4p1 server tool',
        url='http://github.com/p4p1/p4p1',
        author='p4p1',
        author_email='k0oo@icloud.com',
        license='None',
        packages=['server'],
        scripts=['bin/p4p1'],
        zip_safe=False)
