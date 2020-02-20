# Setup environment to run before make-ing and running ROOT_GUI

# For EVIO
arch=$(uname)-$(uname -m)
export EVIO_LIB=$CODA/$arch/lib
export EVIO_INC=$CODA/$arch/include
export LD_LIBRARY_PATH=$EVIO_LIB:$LD_LIBRARY_PATH
export PATH=$EVIO_INC/include:$PATH

# Needed to use local install of libconfig
#export CFLAG="-I/home/sbs-onl/local/include"
export EXTRA_LIB=/usr/lib64
export LD_LIBRARY_PATH=$EXTRA_LIB:$LD_LIBRARY_PATH
