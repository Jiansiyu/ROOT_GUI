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
   if [  $(root-config --version) \< "6.14/04"  ]; then 
   echo "installed version is "$(root-config --version)
   echo "Need to reinstall root 6.14.04"
   fi
fi
