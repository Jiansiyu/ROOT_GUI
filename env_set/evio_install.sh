# !/bin/bash
########################
#             evio INSTALLER 
# It will read the url in required_pachage, 
# Dowload and install the lib in the current path
# it will also pass the newest env to the higher level program
# Tested root Version : 6.14.04
# build path          : root-$version-build
# install path        : root-$version-install
# run instruction     : when runing in standalone mode : ./root_install.sh [root_version]
#                                            ex. ./root_install.sh 6.14.04
#                       when the input parameter is not set, it will read form required_package file 
########################
THIS_DIR=`cd "\`dirname \"$0\"\`";pwd`

EVIO_PATH=$THIS_DIR/evio

DOWNLOAD_PATH=$EVIO_PATH/evio
INSTALL_PATH="$THIS_DIR/evio-install"

DOWNLOAD_URL="https://coda.jlab.org/drupal/system/files/evio-4.4.6.tgz"
DOWNLOAD_FILE=$( basename $DOWNLOAD_URL )
TAR_PATH="${DOWNLOAD_FILE%.*}"

echo "create the install folder"

