# !/bin/bash
########################
#
#
#
########################

if [[ -z "ROOTSYS" ]]; then
   echo  "ROOTSYS is not defined !"
else
   echo "ROOTSYS is defined !, the installation version is ROOT" $(root-config --version)
   if [  $(root-config --version) \> "6.15/0"  ]; then 
   echo "requirement meet"
   else
   echo "not"
   fi
fi
