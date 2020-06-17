#!/bin/bash

if [ "$#" -eq "0" ]; then
  echo "Cutting and combining files in parallel"
  echo "Usage: bash combine_parallel.bash (filelist)"
  exit 1
fi

source ~pzc/.bashrc

list=${1}
lines=`cat $list | wc -l `
user=`whoami`
paranum=72

psrfits=`cat $list | sed -n "1 p"`
#echo $psrfits
psrfits=${psrfits%_????.fits*}
psrfits=${psrfits##*/}

list_file="combine_files_"$psrfits".txt"

rm -rdf $list_file

echo "Cutting files in parallel......"

for((i=1;i<=lines;i++)); do
  fits_file=`cat $list | sed -n "$i p"`
  fits_cutted=${fits_file%.fits*}"_single"
  fits_cutted=${fits_cutted##*/}
  echo $fits_cutted".fits" >> $list_file
  #using fitsio based code
  python $RPPPS_DIR/fitsio_splitpol_FASTpsrfits.py $fits_cutted $fits_file >> /dev/null &
  #using pyfits based code
  #python $RPPPS_DIR/splitpol_FASTpsrfits.py $fits_cutted $fits_file >> /dev/null &
  a=`ps augx | grep $user | grep python | grep splitpol | wc -l`
  finished_files=`ls $psrfits*.fits | wc -l`
  echo "Cutting: $lines in total, $a running, $finished_files finished."
  while(( $a >= $paranum ))
  do
    finished_files=`ls $psrfits*.fits | wc -l`
    a=`ps augx | grep $user | grep python | grep splitpol | wc -l`
    echo "Cutting: $lines in total, $a running, $finished_files finished."
    #((i--))
    sleep 2
  done
done

a=`ps augx | grep $user | grep python | grep splitpol | wc -l`
while(( $a >= 1 ))
  do
    finished_files=`ls $psrfits*.fits | wc -l`
    a=`ps augx | grep $user | grep python | grep splitpol | wc -l`
    echo "Cutting: $lines in total, $a running, $finished_files finished."
    #((i--))
    sleep 5
  done

echo "File cutting finished, now combining......"

#export RPPPS_DIR=/home/pzc/pulsar_search/RPPPS/RPPPS_gznu_20181227
psrfits=$psrfits"_combined"
#echo $psrfits
python $RPPPS_DIR/fitsio_combine_many_1pol_FASTpsrfits.py $psrfits $list_file
#python $RPPPS_DIR/fitsio_splitpol_combine_many_FASTpsrfits.py $psrfits combine_files.txt

cat combine_files.txt | xargs -n 1 --replace rm -rdf {}
rm -rdf combine_files.txt


