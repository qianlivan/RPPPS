#!/bin/bash
pfd_select_path="/data2/home2/pzc/pfd_select/"
p0_limit=800.0
snr_limit=10.0

#1, read all the pfd files in current folder
echo "Searching for pfd files......"
find | grep pfd > pfd_list.txt

#2, check the candidates number
lines=`cat pfd_list.txt | wc -l`
echo "$lines pfd files found."
sleep 1

#3, if there is no candidates
if [ $lines -eq 0 ]; then
  echo "No pfd files in current folder."
  exit 1
fi

#4, start checking candidates
numbers=0
echo "echo Start copying pfd files......" > cp_script.bash
for((i=1;i<=lines;i++))do
  tmp=`cat pfd_list.txt | sed -n "$i p"`
  p0=${tmp%ms*}
  p0=${p0##*_}
  snr=${tmp#*_s}
  snr=${snr%*.pfd}
  #echo 
  #echo "Checking file $tmp, p0 is $p0, SNR is $snr."
  echo "Checked $i candidates, $lines in total."
  if [[ $(echo "$p0 <= $p0_limit" | bc) -eq 1 && $(echo "$snr >= $snr_limit" | bc) -eq 1 ]]; then
    #echo "$(echo "$p0 <= $p0_limit" | bc)  $(echo "$snr >= $snr_limit" | bc)"
    #if [ $(echo "$snr >= $snr_limit" | bc) ]; then
      echo "cp $tmp $pfd_select_path" >> cp_script.bash
      ((numbers++))
      echo "Candidates $tmp meets the requirements, $numbers candidates selected."
    #fi
  fi
  #rst=`$RPPPS_DIR/candidates_p0_snr_checking p0 snr snr_limit p0_limit`
  #if [ $rst -eq 1 ]; then
    #echo "cp ./$tmp $pfd_seclect_path" >> cp_script.bash
  #fi
done

echo "$numbers candidates selected in total."
bash cp_script.bash
#rm -rdf pfd_list.txt
#rm -rdf cp_script.bash




