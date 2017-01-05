This is the package of Re-analysing Pipeline for Parkes Pulsar Survey and other pulsar data

INSTALLATION
1, I suppose that you have installed gcc-c++, so just running compile.bash
with BASH or SH, like
bash compile.bash
OR
./compile.bash

2, Editing the initial file for your shell, such as .bashrc or .cshrc;
For .bashrc, add this line
export RPPPS_DIR=/xxxx/xxxx
while /xxxx/xxxx is where this package located.
for example, I use
export RPPPS_DIR=/home/pzc/pulsars/scripts

3, Setting where you put PMPS data, like
export PMPS=/psrdata/pmb
in you shell initial file, such as .bashrc or .cshrc

4, Of course you need a fully installed PRESTO

THAT'S ENOUGH!!!
Don't forget to use the scripts in a newly opened terminal!

Descriptions for files

ACCEL_sift.py
--- Modified ACCEL_sift.py in PRESTO package

compile.bash
--- Shell script to compile all the C++ source codes

CpCand.cpp
--- Source file for creating scripts to parallel folding candidates

makeDDlist_para_2.cpp
--- Source file for creating scritps to parallel dedisperion, FFT and
acceleration search

prepfold_end.bash
--- Script to check if prepfold has finished

RPPPS_accel.presto
--- Script to perform acceleration search, single pulse searching is added

RPPPS_list.presto
--- Script to perform an acceleration search with the script RPPPS_accel.presto for a input file list with given parameters

RPPPS_single.presto
--- Script to perform single pulse search

Readme.txt
--- This file

Usage:
source $RPPPS_DIR/RPPPS_accel.presto PM0001_00211.sf 64 0
--- run the script
--- script filename
--- PMPS raw file name, with extenstion
--- parallel thread number, here is 64, default is 64
--- parameter for acceleration search, between 0 and 300, default is 0

source $RPPPS_DIR/RPPPS_list.presto PMPS.list 1 100 128 50
--- run the script
--- script file name
--- input file list, format: ONLY filename, without extentions (.sf), one for each line
--- start number
--- end number
--- the threads you want to run at the same time
--- acceleration

source $RPPPS_DIR/RPPPS_single.presto PM0001_00211.sf
--- run the script
--- PMPS raw file name, with extenstions

For the file in one folder, just use:
ls $PMPS/PM0abc/*.sf | xargs -n 1 RPPPS_accel.presto
ls $PMPS/PM0abc/*.sf | xargs -n 1 RPPPS_single.prsto

Good luck!

Pzc

