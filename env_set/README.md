# Enviromrnt check and auto installation scriptes

## Enviroment setting step by step instruction

ROOT_GUI are tested with evio-4.4.6, root6.14.04, and libconfig. If you are sure your configure and enrioment are set property, you can skip those step.

### install the evio-4.4.6 


#### Step 1.

```
./evio_install.sh
```

After this step, the script will generate the following folders/files 
```
evio-4.4.6  (can be delete)
evio_env.csh  
evio_env.sh  
evio-install
```

#### Step 2.
install root6.14.04

```
./root_install.sh
```

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
