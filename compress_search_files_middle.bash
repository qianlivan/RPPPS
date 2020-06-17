#!/bin/bash

current_path=`pwd`
folder_name="beams_middle"

cd $folder_name
#alias ldir='ls -F | grep /'
ls -F | grep / > folder_list.txt
lines=`cat folder_list.txt | wc -l`

for((i=1;i<=lines;i++))
do
  data_folder=`cat folder_list.txt | sed -n "$i p"`
  echo Entering folder $data_folder
  cd $data_folder
  tmp=`pwd`
  tmp=${tmp##*/}
  mkdir $tmp
  echo Moving files......
  mv -f ./*.inf ./$tmp/
  mv -f ./*_ACCEL* ./$tmp/
  echo Compressing......
  tar -zcvf ./$tmp.tar.gz ./$tmp >> /dev/null
  echo Removing files and folders......
  rm -rdf ./$tmp
  ls -F | grep / | xargs -n 1 --replace rm -rdf {}
  echo
  cd ..
done

rm -f folder_list.txt
cd $current_path





