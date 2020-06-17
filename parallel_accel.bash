#!/bin/bash

#in how many nodes will this script run
#nodes=4

#initial settings
listname="fft.list"
ls ./*.fft > $listname
zmax_value=200
cpu_value=24
harmonics=16

lines=`cat $listname | wc -l`
echo $lines files to be processed in total.

count=0
file_index=0
flag_exist=0

#ls parallel*.bash | xargs -n 1 --replace rm -rdf {}
echo "" > parallel_$file_index.bash
for((i=1;i<=lines;i++)); do
  #echo "Create file $file_index......"
  fft_name=`cat $listname | sed -n "$i p"`
  accel_file=${fft_name%.fft*}"_ACCEL_"$zmax_value
  if [ ! -f ./$accel_file ] ; then
    echo "echo Processing $fft_name......" >> parallel_$file_index.bash
    echo "accelsearch -inmem -numharm $harmonics -zmax $zmax_value $fft_name >> /dev/null &" >> parallel_$file_index.bash
    ((count++))
  else
    echo "File $fft_name processed, skip."
  fi
  if((count >= $cpu_value)); then
    echo "tmp=\`ps augx | grep pzc | grep accelsearch | wc -l\`" >> parallel_$file_index.bash
    echo "((tmp--))" >> parallel_$file_index.bash
    echo "while ((tmp >0)) ; do date ; echo ACCELSEARCH: \$tmp left. ; sleep 10 ; tmp=\`ps augx | grep pzc | grep accelsearch | wc -l\` ; ((tmp--)) ; done" >> parallel_$file_index.bash
    echo "Parallel file $file_index created."
    ((file_index++))
    echo "" > parallel_$file_index.bash
    count=0
  fi 
done

for((i=0;i<=file_index;i++));do
  #echo "bash parallel_$i.bash >> paralle_run_$count.bash"
  #((count++))
  bash parallel_$i.bash
done


