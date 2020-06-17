#!/bin/bash

ACCEL_list=ACCEL_list.txt
lines=`cat $ACCEL_list | wc -l`
fname=info.txt

#lines=10

for((i=1;i<=lines;i++))
do
  tmp=`cat $ACCEL_list | sed -n "$i p"`
  echo $tmp >> $fname
  cat $tmp | grep 1.9799 >> $fname
  cat $tmp | grep 1.9800 >> $fname
  #cat $tmp | grep 3.160 >> $fname
  #cat $tmp | grep 3.161 >> $fname
#  echo
#  echo 
done


