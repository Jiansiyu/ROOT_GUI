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

echo " Jeffersion EVIO installation scrapt"
echo "  Siyu Jian (jiansiyu@gmail.com)"

THIS_DIR=`cd "\`dirname \"$0\"\`";pwd`
EVIO_PATH=$THIS_DIR/evio

DOWNLOAD_PATH=$EVIO_PATH/evio
INSTALL_PATH="$THIS_DIR/evio-install"

DOWNLOAD_URL="https://coda.jlab.org/drupal/system/files/evio-4.4.6.tgz"
DOWNLOAD_FILE=$( basename $DOWNLOAD_URL )
TAR_PATH="${DOWNLOAD_FILE%.*}"

INSTALL_BIN=${INSTALL_PATH}/$(arch)/bin
INSTALL_INC=${INSTALL_PATH}/$(arch)/include
INSTALL_LIB=${INSTALL_PATH}/$(arch)/lib

ENV_SETFILE_CSH=${THIS_DIR}/evio_env.csh
ENV_SETFILE_SH=${THIS_DIR}/evio_env.sh
rm ${ENV_SETFILE_CSH} ${ENV_SETFILE_SH}
touch ${ENV_SETFILE_CSH}
touch ${ENV_SETFILE_SH}
chmod +x ${ENV_SETFILE_SH}
chmod +x ${ENV_SETFILE_CSH}
echo "Dowload url   : ${DOWNLOAD_URL}"
echo "Dowload file  : ${DOWNLOAD_FILE}"
echo "extract folder: ${TAR_PATH}"
echo "install bin : ${INSTALL_BIN}"
echo "install inc : ${INSTALL_INC}"
echo "install lib : ${INSTALL_LIB}"
echo "create the install folder"

until test -e ${INSTALL_PATH}
do 
    mkdir ${INSTALL_PATH}
done 

until test -f ${DOWNLOAD_FILE}
do
    wget ${DOWNLOAD_URL} ${DOWNLOAD_FILE}
done

tar -vxf ${DOWNLOAD_FILE}

if [ $(dpkg-query -W -f='${Status}' scons 2>/dev/null | grep -c "ok installed") -eq 0 ];
then
  sudo apt-get install scons;
fi

cd ${TAR_PATH}
scons --incdir=${INSTALL_INC} --libdir=${INSTALL_LIB} --bindir=${INSTALL_BIN} install

# generate the env file
echo "# !/bin/bash" >> ${ENV_SETFILE_SH}
echo "export EVIO_BIN=${INSTALL_BIN}" >> ${ENV_SETFILE_SH}
echo "export EVIO_INC=${INSTALL_INC}" >> ${ENV_SETFILE_SH}
echo "export EVIO_LIB=${INSTALL_LIB}" >> ${ENV_SETFILE_SH}

echo "setenv EVIO_INC ${INSTALL_INC}" >> ${ENV_SETFILE_CSH}
echo "setenv EVIO_LIB ${INSTALL_LIB}" >> ${ENV_SETFILE_CSH}
echo "setenv EVIO_BIN ${INSTALL_BIN}" >> ${ENV_SETFILE_CSH}
