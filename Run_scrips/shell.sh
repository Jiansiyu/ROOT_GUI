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
    
    THIS_DIR=`cd "\`dirname \"$0\"\`";pwd`
    
    GEMANALYSIS_FILE_MAIN_PATH=/home/newdriver/Research/SBS/SBS_GEM_labtest/GEManalysis_Result/
    DECODED_FILE_PATH=${THIS_DIR}/../results/Hit_*
    OUTPUT_FILE_PATH=${THIS_DIR}/../results/
    
    NR_CPUS=7     #maximum concorrent process threads
    echo
    echo "            	      GEM analysis Framework"
    echo
    echo
    echo "************  calling the data process framework  *************"
    echo
    	
   for FILE_NAME_DECODED in ${DECODED_FILE_PATH}*
    do 
        OUTPUT_FILENAME="Tracking_"${FILE_NAME_DECODED##*/}
        echo "[SYSTEM INFORMATION]::GEManalysis ${FILE_NAME_DECODED}"
        
        echo "OUTPUT FILE  $OUTPUT_FILENAME"
        echo 
        ./step_run.sh ${FILE_NAME_DECODED} $OUTPUT_FILE_PATH$OUTPUT_FILENAME  &

    	$((CPU_i ++ ))
    	if [ $CPU_i = 7 ]
    	then
    	 wait
    	 CPU_i=0
    	fi  
   done
  END=$(date +%s)
  echo "TIME SPEED: $(expr $END - $BEGIN)"
  echo "******Finish all the progress******"
  echo "*********** THE END ***************" 
