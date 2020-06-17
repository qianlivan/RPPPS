#!/bin/bash
#the top level script for processing FAST's one day data

#check if raw folder exist
if [ ! -d "./raw" ]; then
  echo "Please create the folder for raw data: mkdir raw"
  echo "Then, link the raw data to the folder: ln -s /home/data/yyyymm/dd/*.fits ./"
  exit 0
fi

#copy all the scripts
cp $RPPPS_DIR/generate_list_low.bash ./
cp $RPPPS_DIR/generate_list_middle.bash ./
cp $RPPPS_DIR/generate_list_high.bash ./
cp $RPPPS_DIR/cut_for_singlepulse_low.bash ./
cp $RPPPS_DIR/cut_for_singlepulse_low.bash ./
cp $RPPPS_DIR/cut_for_singlepulse_low.bash ./

#for 290-340 MHz band
#low frequency periodic search (FFT and FFA)
#bash ./generate_list_low.bash
#low frequency single pulse search
#bash ./cut_for_singlepulse_low.bash

#575-725 MHz
#middle frequency periodic search (FFT and FFA)
bash ./generate_list_middle.bash
#middle frequency single pulse search
bash ./cut_for_singlepulse_low.bash

#1.05-1.45 GHz
#high frequency periodic search (FFT and FFA)
#bash ./generate_list_high.bash
#high frequency single pulse search
#bash ./cut_for_singlepulse_low.bash



