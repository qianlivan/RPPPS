#!/bin/bash


for((i=0;i<=280;i++))
do
  #forming the file name string
  tmp=""
  for((j=1;j<=20;j++))
  do
    ((index=i*5+j))
    filename=`printf "M14_tracking_%04d.fits" $index`
    tmp=$tmp$filename" "
  done
  ((index_start=i*5+1))
  ((index_end=index_start+19))
  output_string=`printf "M14_tracking_%04d-%04d" $index_start $index_end`
  echo Processing file $index_start to $index_end
  echo "RFI masking......"
  rfifind -ncpus 24 -time 1.6 -o $output_string $tmp >> /dev/null
  echo "Dedispersing......"
  prepdata -nobary -ncpus 24 -numout 5242880 -dm 82.076 -mask $output_string"_rfifind.mask" -o $output_string $tmp >> /dev/null
  echo "FFTing......"
  realfft -fwd -mem $output_string".dat" >> /dev/null
  echo "Acceleration seaching......"
  accelsearch -zmax 50 -inmem -numharm 32 $output_string".fft" >> /dev/null
  echo
done

