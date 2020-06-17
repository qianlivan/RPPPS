#!/bin/bash

#echo "Convert all the FAST raw files in current directory to filterbank for single pulses search."

#channels for cutting, low frequency
startn=1160
endn=1359

#channels for cutting, middle frequency
#startn=2300
#endn=2899

#channels for cutting, high frequency
#startn=2350
#endn=2849

#entering raw data folder
mkdir single_pulse_middle
cd ./raw/

lines=`ls *0-1G*.fits | wc -l`
ls *.fits > fitslist.txt
echo "$lines files found."
echo

for((i=1;i<=lines;i++)); do
  filename=`cat fitslist.txt | sed -n "$i p"`
  subints=`readfile $filename | grep Subints | awk '{print $5}'`
  filename_noext=${filename%.*}
  filename_noext=${filename_noext##*/}
  echo "Processing file $filename, $subints subints."
  ((subints--))
  echo "Cutting the file."
  #cut the file with given
  python -W ignore $RPPPS_DIR/cut_FASTpsrfits_freq_time_splitpol.py $startn $endn 0 $subints $filename >> log.txt
  tot_filename=$filename_noext"_tot_"$startn"_"$endn"_0_"$subints".fits"
  pol1_filename=$filename_noext"_pol1_"$startn"_"$endn"_0_"$subints".fits"
  pol2_filename=$filename_noext"_pol2_"$startn"_"$endn"_0_"$subints".fits"
  rm -rdf $pol1_filename $pol2_filename
  mv ./$tot_filename ../single_pulse_middle/
  #echo "Converting to filterbank."
  #echo filterbank $tot_filename > $tot_filename".fil"
  #rm -rdf $tot_filename
  echo "Process finished."
  echo
done

#rm -rdf fitslist.txt

#move to the same place
cd .. 





