#!/bin/bash

#read the file
lines=`cat ${1} | wc -l`

for((i=1;i<=lines;i++))
do
  period=`cat ${1} | sed -n "$i p" | awk '{print $6*1e-3}'`
  echo "$lines in total,ssss processing $i."
  prepfold ./47T131_0031.dat.fil -dm 31.82 -mask ./47T131_0031_rfifind.mask -nsub 128 -npart 128 -n 64 -nosearch -accelfile 47T131_0031_ACCEL_15.cand -accelcand $i
done


