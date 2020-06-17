#!/bin/bash

#checking input parameters
if [ "$#" -eq "0" ];then
  echo "ERROR: no input file."
  echo "Example: bash $RPPPS_DIR/gather_cands.bash /hone/pzc/tmp/PMPS_test/PM0064_00311_DM99.00_ACCEL_50"
  echo
  exit 2
fi

#if [ "$#" -gt "1" ];then
#  echo "WARNNING: no input file more than ONE."
#  echo "Will ONLY process the first file."
#  echo
#fi

#checking if file exist
if [ ! -f "${1}" ]; then
  echo "No such a input file ${1}."
  echo
  exit 1
fi
filename=${1}

#input file data checking
#the first word should Summed
tmp=`cat $filename | sed -n "1 p" | awk '{print $1}'`
#echo $tmp
if [ "$tmp" != "Summed" ];then
  echo "ERROR: the file format could be incorrect."
  echo
  exit 1
fi

#if line_index==0, all the candidates are outputted.
#echo "$filename is in the PRESTO acceleration search result format."
echo "Start generating candidate list from $filename."
#final output filename is cand_ACCEL_(z_max).txt
#temp candidate list can be cand_ACCEL_(a_max)_full.txt
#get z_max value
z_max=${filename##*_}
outfile="cand_ACCEL_"$z_max"_full.txt"

#echo
cand_index=4
cand_num=1
cand_lines=`cat $filename | wc -l`
DM=${filename#*DM}
DM=${DM%%_*}
while((cand_index!=0))
do
  #echo $cand_index
  if [ "$cand_index" -le "$cand_lines" ];then
    #echo "Reading files......"
    #get the candidate line
    tmp=`cat $filename | sed -n "$cand_index p"`
    #echo $tmp
    #get the candidate number
    tmp_num=`echo $tmp | awk '{print $1}'`
    #if it is the end of the list
    if [ "$tmp_num" == "Power" ];then
      echo "Finished."
      exit 0
    fi
    #if the line is for one candidate
    #echo $tmp_num, $cand_num
    if [ "$tmp_num" == "$cand_num" ];then
      p0=`echo $tmp | awk '{print $6}'`
      p0=${p0%%(*}
      f0=`echo $tmp | awk '{print $7}'`
      f0=${f0%%(*}
      sigma=`echo $tmp | awk '{print $2}'`
      num_harm=`echo $tmp | awk '{print $5}'`
      summed_power=`echo $tmp | awk '{print $3}'`
      coherent_power=`echo $tmp | awk '{print $4}'`
      z=`echo $tmp | awk '{print $10}'`
      z=${z%%(*}
      a=`echo $tmp | awk '{print $11}'`
      a_number=${a%(*}
      a_tmp=${a#*)}
      if [ "$a_tmp" == "" ];then
        a_index=""
      else
        a_index="e"${a#*^}
      fi
      notes=`echo $tmp | awk '{print $12}'`
      if [ "$notes" == "" ];then
        notes="No_matched_PSR"
      else
        notes1=`echo $tmp | awk '{print $13}'`
        notes2=`echo $tmp | awk '{print $14}'`
        notes2=`echo $tmp | awk '{print $15}'`
        notes2=`echo $tmp | awk '{print $16}'`
        notes=$notes$notes1$notes2$notes3$notes4
      fi
      #output format: p0, f0, DM, sigma, num_harm, summed_power, coherent_power, z, a, notes
      echo "$cand_num $p0    $f0    $DM    $sigma    $num_harm    $summed_power    $coherent_power    $z    $a_number$a_index $notes" >> "./"$outfile
      ((cand_num++))
    else
      #if the candidate section is finished
      echo "Candidate section done."
      exit 0
    fi
  ((cand_index++))
  else
    #if out of file range
    echo "End of the file."
    exit 0
  fi
done

((cand_num--))
echo "$cand_num candidates found in $filename, processed."
echo

exit 0




