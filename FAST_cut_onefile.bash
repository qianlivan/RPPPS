#/bin/bash

#usage: bash FAST_cut_onefile.bash (input file) (start channel) (end channel) (number of subint)
#start and end channels are also included, e.g., 100 and 200 will lead to 101 channels.
#number of subint is the length of the cut data section.

#for FAST ultra wideband receiver
#the test data is 0-1024 MHz with 4096 channels
#one subint corresponds to 0.5 s

#the lenght for cutting comes from 1.22*(lambda/D)
#here, lambda is the wavelenght, D is the diameter of FAST, 300 meter

#input checking
if [ "$#" -ne "5" ];then
  echo "-------------------------------------------------------------------"
  echo "Usage: bash FAST_cut_onefile.bash (input file) (start channel) (end channel) (number of subint for one beam) (offset subint number)"
  echo "Start and end channels are also included, e.g., 100 and 200 will lead to 101 channels."
  echo "Number of subint is the length of the cut data section."
  echo "offset subint number can be one third or one fourth of the number of subint for one beam"
  echo "Last updated: 2016-12-22"
  echo "-------------------------------------------------------------------"
  echo
  echo "All the 5 input parameters are needed."
  echo "You only provided $#."
  echo
  exit 2
fi

  echo "-------------------------------------------------------------------"
  echo "This script is for FAST drift scan pulsar survey, for cutting ONE psrfits file into beams."
  echo "Last updated: 2016-12-22"
  echo "-------------------------------------------------------------------"
  echo

filename=${1}
filename_noext=${filename%.*}
filename_noext=${filename_noext##*/}
startchannel=${2}
endchannel=${3}
n_subint=${4}
offset_subint=${5}

#check if input file exist
if [ ! -f "$filename" ];then
  echo "No input file $filename found."
  echo
  exit 2
fi

#get the file lenghth
lenght=`readfile $filename | grep "Time per file" | awk '{print $6}'`
#get the subint number
subint=`readfile $filename | grep "Subints per file" | awk '{print $5}'`

#forming the scripts for cutting
#cleaning the file
echo > cutting_time.bash
echo "mkdir $filename_noext" >> cutting_time.bash
echo >> cutting_time.bash
for((i=0;i<subint-n_subint-1;i=i+offset_subint))
do
  ((tmp=i+n_subint-1))
  #show the information
  echo "echo Cutting the subint $i to $tmp, $subint in total." >> cutting_time.bash
  #forming the cutting command
  echo "python $RPPPS_DIR/cut_FASTpsrfits_freq_time_splitpol.py $startchannel $endchannel $i $tmp $filename >> /dev/null" >> cutting_time.bash
  #removing and renaming files
  tot_filename=$filename_noext"_tot_"$startchannel"_"$endchannel"_"$i"_"$tmp".fits"
  pol1_filename=$filename_noext"_pol1_"$startchannel"_"$endchannel"_"$i"_"$tmp".fits"
  pol2_filename=$filename_noext"_pol2_"$startchannel"_"$endchannel"_"$i"_"$tmp".fits"
  echo "rm -rdf $pol1_filename" >> cutting_time.bash
  echo "rm -rdf $pol2_filename" >> cutting_time.bash
  echo "mv $tot_filename ./$filename_noext/" >> cutting_time.bash
  echo >> cutting_time.bash
done

#performe the cutting
echo "Cutting files......"
bash cutting_time.bash
rm -rdf cutting_time.bash

#only for RPPPS test purpose
#echo "Performing searching test......"
#cd $filename_noext
#ls ./*.fits | xargs -n 1 --replace bash $RPPPS_DIR/RPPPS_search.presto {} 128 0

echo "Cutting finished."
echo
exit 0





















