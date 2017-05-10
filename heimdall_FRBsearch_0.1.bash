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
heimdall -f ${1} -dm 0 5000 &

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

cat ./*.cand > $fname""_cand.txt

rm ./*.cand -rdf

echo "All Finished."


