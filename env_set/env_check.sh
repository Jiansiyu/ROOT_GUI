# !/bin/bash
########################
#  MPD Analyzer ENV set
#  
#
########################

if [[ -z "ROOTSYS" ]]; then
   echo  "ROOTSYS is not defined !"
else
   echo "ROOTSYS is defined !, the installation version is ROOT" $(root-config --version)
   if [  $(root-config --version) \< "6.15/0"  ]; then 
   echo "installed version is "$(root-config --version) 
   echo "Mark to reinstall the latest version"
   fi
fi
