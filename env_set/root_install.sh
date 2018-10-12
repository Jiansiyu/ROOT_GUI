# !/bin/bash
########################
#             CERN ROOT INSTALLER 
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

ROOT_COMPILE_VARIABLES=\

THIS_DIR=`cd "\`dirname \"$0\"\`";pwd`
ROOT_VERSION=$( awk -F= '/root_version/ { print $2 }' required_package )
ROOT_URL="https://root.cern.ch/download/root_v$ROOT_VERSION.source.tar.gz"

ROOT_DOWNLOADFILE=$( basename $ROOT_URL )
ROOT_DOWNLOADFILE_NAME="${ROOT_DOWNLOADFILE%.*.*.*}"
ROOT_PATH="$THIS_DIR/root-$ROOT_VERSION"

TAR_PATH="$ROOT_PATH/root-$ROOT_VERSION"
BUILD_PATH="$ROOT_PATH/root-$ROOT_VERSION-build"
INSTALL_PATH="$ROOT_PATH/root-$ROOT_VERSION-install"

mkdir $ROOT_PATH
mkdir $BUILD_PATH
mkdir $INSTALL_PATH

echo "Folder      : $ROOT_PATH"
echo "build path  : $BUILD_PATH"
echo "install path: $INSTALL_PATH"

cd $ROOT_PATH

echo "Downloading the file: $ROOT_URL"
until test -f $ROOT_DOWNLOADFILE
do wget $ROOT_URL
done

echo "Untar the file $ROOT_DOWNLOADFILE"
until test -d $TAR_PATH
do tar -zxvf $ROOT_DOWNLOADFILE
done

echo "Go into folder $BUILD_PATH  and start compile"
cd $BUILD_PATH
cmake -DCMAKE_INSTALL_PREFIX=${INSTALL_PATH} \
      $TAR_PATH

j=`cat /proc/cpuinfo | grep processor | wc -l`
echo "Make will use $j parallel jobs."

echo "Building root-v$ROOT_VERSION"
make -j $j -k

echo "Finish build root"

echo "installing the root"
make install

echo "Generate the file enviroment"
source $INSTALL_PATH/bin/thisroot.sh
echo "install version " root-config --version
echo "install path $ROOTSYS"
cd $THIS_DIR