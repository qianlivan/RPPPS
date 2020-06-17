#!/bin/bash

#give the beam forming parameters, for low frequency, centerting at 315 MHz with 50 MHz bandwidth
#channel_start=1160
#channel_end=1359
#beam_size_subint=64
#band_selection="low"
#raw_file_string="0-1G"
#beam_offset=32

#give the beam forming parameters, for middle frequency, centerting at 650 MHz with 150 MHz bandwidth
#channel_start=2350
#channel_end=2849
#beam_size_subint=32
#band_selection="middle"
#raw_file_string="0-1G"
#beam_offset=16

#give the beam forming parameters, for high frequency (L-band), centerting at 1250 MHz with 400 MHz bandwidth
channel_start=104
channel_end=1703
beam_size_subint=32
band_selection="high"
raw_file_string="1-2G"
beam_offset=16

#other constants
file_subints=64
beam_folder="beams_"$band_selection
filelist="list_"$band_selection".txt"
filelist_subint="list_"$band_selection"_subints.txt"
cutting_script="run_file_process_"$band_selection".bash"
raw_data_path=`pwd`
raw_data_path=$raw_data_path"/raw"

#generate the file list
ls $raw_data_path"/"*$raw_file_string*.fits > $filelist

#see how many files do we have
lines=`cat ./$filelist | wc -l`

#get the detailed filelist
if [ ! -d "./$filelist_subint" ];then
  rm -rdf "./$filelist_subint"
fi
echo "Checking FAST psrfits in this folder."
subints_total=0
for((i=1;i<=lines;i++))
do
  filename=`cat $filelist | sed -n "$i p"`
  subints=`readfile $filename | grep "Subints per file" | awk '{print $5}'`
  ((subints_total=subints_total+subints))
  #echo "File $filename has $subints subints."
  echo "$filename $subints" >> $filelist_subint
done
echo "$lines files found."
echo "Total subints we got are $subints_total."

#get the filename string and then for generate filenames
#filename_string=${filename#*/}
#filename_noext=${filename_string%.*}
#filename_string=${filename_string%_*}"_0"
#echo $filename_string

subints_start=0
if [ ! -d "./$cutting_script" ];then
  rm -rdf ./$cutting_script
fi
if [ ! -d "./$beam_folder/" ];then
  mkdir ./$beam_folder
fi
echo "Creating SHELL script."
while [ $subints_start -lt $subints_total ]
do
  flag_combine=0
  flag_cut=0
  #seeking the file
  index=$((subints_start/file_subints+1))
  #deciding cut or combine
  subints_left=$[ subints_start % file_subints ]
  if((subints_left+beam_size_subint > file_subints))
  then
    flag_combine=1
  else
    flag_cut=1
  fi
  #echo $subints_left, $beam_size_subint, $file_subints, $flag_combine, $flag_cut, $subints_start
  #generate the command
  file1=`cat $filelist_subint | sed -n "$index p" | awk '{print $1}'`
  #file1=${file1#*/}
  file1=${file1#*raw/}
  #file1_noext=${file1#*/}
  file1_noext=${file1%.*}
  ((subints_file1_start=subints_left))
  #echo "echo Processing $file1, start subints $subints_file1_start, channel start $channel_start, channel end $channel_end." >> $cutting_script
  if((flag_combine==1))
  then
    ((index++))
    file2=`cat $filelist_subint | sed -n "$index p" | awk '{print $1}'`
    #file2=${file2#*/}
    file2=${file2#*raw/}
    #filename_string=${file1#*/}
    file2_noext=${file2%.*}
    ((subints_file2_end=subints_file1_start+beam_size_subint-$file_subints-1))
    #seems with python input error
    #echo "python $RPPPS_DIR/combine_cut_FASTpsrfits_freq_time_splitpol.py $channel_start $channel_end $subints_file1_start $subints_file2_end $raw_data_path"/"$file1 $raw_data_path"/"$file2 >> /dev/null" >> $cutting_script
    echo "echo Combining data sections, starts from sunint $subints_file1_start of $file1, ends at subint $subints_file2_end of $file2" >> $cutting_script
    echo "cd ./raw/" >> $cutting_script
    echo "python -W ignore $RPPPS_DIR/combine_cut_FASTpsrfits_freq_time_splitpol.py $channel_start $channel_end $subints_file1_start $subints_file2_end $file1 $file2 >> /dev/null" >> $cutting_script
    echo "mv -f ./"$file1_noext"_"$file2_noext"_tot_"$channel_start"_"$channel_end"_"$subints_file1_start"_"$subints_file2_end".fits ../"$beam_folder"/">> $cutting_script
    echo "cd .." >> $cutting_script
    #echo "rm -rdf $file1_noext"_"$file2_noext"_pol1_"$channel_start"_"$channel_end"_"$subints_file1_start"_"$subints_file2_end".fits"" >> $cutting_script
    #echo "rm -rdf $file1_noext"_"$file2_noext"_pol2_"$channel_start"_"$channel_end"_"$subints_file1_start"_"$subints_file2_end".fits"" >> $cutting_script
    #echo "mv -f "$raw_data_path"/"$file1_noext"_"$file2_noext"_tot_"$channel_start"_"$channel_end"_"$subints_file1_start"_"$subints_file2_end".fits ./"$beam_folder"/" >> $cutting_script
  fi
  if((flag_cut==1))
  then
    ((subints_file1_end=subints_file1_start+beam_size_subint-1))
    echo "echo Cutting data section in $file1, for subint $subints_file1_start to $subints_file1_end. " >> $cutting_script
    echo "python -W ignore $RPPPS_DIR/cut_FASTpsrfits_freq_time_splitpol.py $channel_start $channel_end $subints_file1_start $subints_file1_end $raw_data_path"/"$file1 >> /dev/null" >> $cutting_script
    #remove polarization files due to that they wont be needed in pulsar search(?).
    #echo "rm -rdf $file1_noext"_pol1_"$channel_start"_"$channel_end"_"$subints_file1_start"_"$subints_file1_end".fits"" >> $cutting_script
    #echo "rm -rdf $file1_noext"_pol2_"$channel_start"_"$channel_end"_"$subints_file1_start"_"$subints_file1_end".fits"" >> $cutting_script
    echo "mv -f "$raw_data_path"/"$file1_noext"_tot_"$channel_start"_"$channel_end"_"$subints_file1_start"_"$subints_file1_end".fits ./"$beam_folder"/" >> $cutting_script
  fi
  ((subints_start=subints_start+beam_offset))
done

echo "Cutting files."
bash ./$cutting_script

#clean up
#rm -rdf $filelist_subint
#rm -rdf $filelist
#rm -rdf $cutting_script
echo "All Finished."

