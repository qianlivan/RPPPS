#!/bin/bash

echo "WARNING!!! Will Delete all files in current folder, in parallel!"

echo "Are you sure ??? Y/N"
read tmp
echo "Please wait for 10 second, and considering you decision again."
sleep 10

if [ $tmp = "Y" ]; then
  tmp=`pwd`
  echo "Please consider again: DO YOU HAVE IMPORTRANT FILES HERE ($tmp)? Y/N"
  read tmp
  if [ $tmp = "N" ]; then 
    echo "This is your last chance to retract this operation. Will you cancel this deletion? Cancel/Next"
    sleep 5
    read tmp
      if [ $tmp = "N" ]; then
        ls > filelist.txt
        cp filelist.txt filelist1.txt
        lines=`cat filelist1.txt | wc -l`
        for((i=1;i<=lines;i++))do
          tmp=`cat filelist1.txt | sed -n "$i p"`
          echo "Deleting $tmp."
          rm -rdf $tmp &
        done
      fi
  fi
fi

rm -rdf filelist1.txt
