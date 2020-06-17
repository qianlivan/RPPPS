#!/bin/bash

#copy files
echo "Copying files......"
cp -rf $RPPPS_DIR/candViewer_pfd ./
cp -rf $RPPPS_DIR/knownPsrList ./
cp -rf $RPPPS_DIR/candViewer_pfd_prepare_low.bash ./
cp -rf $RPPPS_DIR/candViewer_pfd_prepare_middle.bash ./

#generate candidates lists
cd ./beams_low/
cp -rf ../candViewer_pfd_prepare_low.bash ./
echo "Generating candidate list for low frequency band......"
bash ./candViewer_pfd_prepare_low.bash
echo "Finished."
echo

cd ../beams_middle/
cp -rf ../candViewer_pfd_prepare_middle.bash ./
echo "Generating candidate list for middle frequency band......"
bash ./candViewer_pfd_prepare_middle.bash
echo "Finished."
echo

#start candViewer
echo "Viewing low frequency band candiates......"
cd ..
./candViewer_pfd -f ./beams_low/cand_list_full_pfd.txt

echo "Viewing middle frequency band candiates......"
./candViewer_pfd -f ./beams_middle/cand_list_full_pfd.txt

echo "Cleaning up......"
#rm -rdf ./beams_low/candViewer_pfd_prepare_low.bash
#rm -rdf ./beams_middle/candViewer_pfd_prepare_middle.bash
#rm -rdf ./beams_low/cand_list_full_pfd.txt
#rm -rdf ./beams_middle/cand_list_full_pfd.txt
#rm -rdf ./candViewer_pfd
#rm -rdf ./candViewer_pfd_prepare_low.bash
#rm -rdf ./candViewer_pfd_prepare_middle.bash
#rm -rdf ./FP*

