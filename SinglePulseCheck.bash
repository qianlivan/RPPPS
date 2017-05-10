#/bin/bash
#using dspsr and psrchive(pav, pam) to check FRB candidates

source ~yumeng/.bashrc

#input file 47T194_0011.dat.fil_cand.txt

#candfile=${1}
#lines=`cat $candfile | wc -l`
#datapath="/data2/pzc/47Tuc/filterbank_all/"
#datafile=${candfile%_*}

#intg_time=3
#width_upperlimit=5
#dm_lowerlimit=10.0
#sigma_lowerlimit=8.0
#cand_available=0

#echo "$lines candidates in $candfile."

length=1 #second
filetime=`readfile ${1} | grep Time | sed -n '2 p' | awk '{print $6}'`
filetime=46
dm=0
fname=${1}
fname=${fname##*/}

for((i=0;i<filetime;i=i+length)); do

  #echo "Processing candidate $i in $lines."

  #FRB_dm=`cat $candfile | sed -n "$i p" | awk '{print $6}'`
  #FRB_time=`cat $candfile | sed -n "$i p" | awk '{print $3}'`
  #FRB_width=`cat $candfile | sed -n "$i p" | awk '{print $4}'`
  #FRB_sigma=`cat $candfile | sed -n "$i p" | awk '{print $1}'`
  
#commands:
#1, dspsr (filterbank) -O tmp -D (dm) -s (time) -T 3 -c 3
#2, pav -GTp tmp.ar ()
#with more? ---------------------------
#3, pam -FTp -e FTp tmp.ar
#4, pav -D tmp.FTp
  #if (($FRB_width <= $width_upperlimit));then
    #if [ $(echo "$FRB_dm > $dm_lowerlimit" | bc) -eq 1 ];then
      #if [ $(echo "$FRB_sigma > $sigma_lowerlimit" | bc) -eq 1 ];then

    #echo "DM $FRB_dm, at $FRB_time second, width $FRB_width, sigma $FRB_sigma."
    dspsr ${1} -O tmp -D $dm -S $i -T $length -c $length #-skz -skz_start 1000 -skz_end 2000
    pav -GTp tmp.ar -g /ps #-k 450,550
    mv pgplot.ps $fname"_"$i"_data.ps"
    pam -FTp -e FTp tmp.ar
    pav -D tmp.FTp -g /ps #-k 450,550
    mv pgplot.ps $fname"_"$i"_pulse.ps"
    #((cand_available++))

      #fi
    #fi
  #fi

done

#echo "Processed $cand_available candidates in $lines."
#echo

rm -rdf tmp.ar
rm -rdf tmp.FTp

