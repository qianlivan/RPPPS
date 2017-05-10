#/bin/bash

#input: bash heimdall_FRBsearch_0.1.bash (input file)

if [ "$#" -eq "0" ];then
  echo "Usage: bash heimdall_FRBsearch_0.1.bash (input file)"
  echo "Try filterbank file?"
  echo "You man need extract in Sigproc-SixByNine to convert PSRFITS to filterbank."
  return 2
fi

#for ${1}=/data1/pzc/aaa.fil
pathname=${1}
#fname=aaa.fil
fname=${pathname##*/}
fname_fil="./"$fname".fil"
#fname_noext=aaa
fname_noext=${fname%.*}

#mkdir $fname_noext
#cd $fname_noext

#check if file exist
if [ ! -f "$pathname" ]; then
  echo "No such a file $pathname."
  echo "No such a file $pathname." >> log.txt
  return 2
fi
echo "Datafile at $pathname."
echo

echo "Processing......"
echo "Start processing file $pathname." >> log.txt
date >> log.txt
echo "Converting PSRFITS to filterbank......"
filterbank ${1} > $fname_fil
echo "Searching......"
heimdall -f $fname_fil -dm 0 5000 & #-nsamps_gulp 1048576 &

user=`whoami`
a=`ps augx | grep $user | grep heimdall | wc -l`
((a=a-1))
b=$a
((a=a+1))
while(( "$a" != "$b"))
do
  sleep 2
  gpu_util
  #echo $a
  echo
  a=`ps augx | grep $user | grep heimdall | wc -l`
done

cat ./*.cand > $fname_fil"_cand.txt"

rm ./*.cand -rdf
rm $fname_fil -rdf

echo "All Finished."


