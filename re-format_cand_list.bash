#!/bin/bash
#add the post script path in the end of each line

file="./cand_20161120_all.txt"

lines=`cat $file | wc -l`

old_filename=`cat $file | sed -n "$i p" | awk '{print $1}'`
old_filename=${old_filename%_DM*}
index_1=0
current_path=`pwd`

for((i=1;i<=lines;i++))
do
  #get the line
  tmp=`cat $file | sed -n "$i p"`
  #get the string of filename
  filename=`echo $tmp | awk '{print $1}'`
  index_2=${filename#*:}
  filename=${filename%:*}
  path_1=${filename%_tot*}
  path_2=${filename%_DM*}
  if [ "$old_filename"x = "$path_2"x ]
  then
    ((index_1++))
    #echo $tmp" /home/pzc/FAST_201611_cut_data/19/"$path_1"/"$path_2"/"$path_2"_ACCEL_10_"$index_1"_ACCEL_Cand_"$index_2".pfd.ps"
  else
    index_1=1
    #echo $tmp" /home/pzc/FAST_201611_cut_data/19/"$path_1"/"$path_2"/"$path_2"_ACCEL_10_"$index_1"_ACCEL_Cand_"$index_2".pfd.ps"
    old_filename=$path_2
  fi
  echo $tmp" "$current_path"/"$path_1"/"$path_2"/"$path_2"_ACCEL_10_"$index_1"_ACCEL_Cand_"$index_2".pfd.ps" >> $file"_CandViewer.txt"
done









