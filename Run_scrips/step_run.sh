#!/bin/bash 
###############################################################################################################
# June 30th 2016
# Used for process several raw data including Decoder and GEManalysis the DATA
# Parameter : input the path and coresspoding file name, the shell will loop over those files automaticly
# run ./run_process_all.sh
#				--Siyu
# Attention
#full path is strongly suggested

# DATA process shell, used for process the stupid data
# why i write this, because i am stupid
# stay young stay naive stay stupid

# backup code for compicated data analysis 
# int name_index[]={,,,,,,,,,} 
# echo "file_name_conferm: processing SBS30_Part1_round2_temp${name_index[${i}]}.root"
#
################################################################################################################

    #General PATH define, It would be better to use the full path 
    #Software path
    GEMANALYSIS_PATH=/home/newdriver/Research/SBS/Analysis_Program/GEManalysis/
    
    SBS_DETECTOR_NUMBER=34

    DECODED_FILE_MAIN_PATH=/home/newdriver/Research/Test_Result/Decoder/
    GEMANALYSIS_FILE_MAIN_PATH=/home/newdriver/Research/Test_Result/GEManalysis/
    
    DECODED_FILE_PATH=${DECODED_FILE_MAIN_PATH}SBS${SBS_DETECTOR_NUMBER}/
    GEMANALYSIS_FILE_PATH=${GEMANALYSIS_FILE_MAIN_PATH}SBS${SBS_DETECTOR_NUMBER}/

   
        cd ${GEMANALYSIS_PATH}
    	
        
        echo "[SYSTEM INFORMATION]::GEManalysis $1"
        
        echo "OUTPUT FILE  $2"
        echo 
    	./gem $1 -o $2  
    	
    	#cp $1 ${GEMANALYSIS_FILE_PATH}
 
