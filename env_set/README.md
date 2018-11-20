# Enviromrnt check and auto installation scriptes

## JLab EVIO installer

copy the file 'evio_install.sh' to the path where you want to install the evio

run :

'''
 % ./evio_install.sh
''' 

it will automaticly download compile and install  the evio in the current path. It will also generate two files:
 *  evio_env.csh
 *  evio_env.sh
It need to add to the enviroment according to your system

for csh
'''
source evio_env.csh
'''
for bash

'''
source evio_env.sh
'''
