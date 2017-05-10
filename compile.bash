#c++ ./pzc_mzl.cpp -o pzc_makezaplist.linux

#c++ ./makeDDlist_para.cpp -o makeDDlist_para.linux 

#c++ ./makeDDlist_para_200.cpp -o makeDDlist_para_200.linux

#c++ ./makeDDlist_para_2_DBF4.cpp -o makeDDlist_para_2_DBF4.linux

#V3 of running trials in parallel
echo "Compiling makeDDlist_para_2.cpp and makeDDlist_para_3.cpp"
c++ $RPPPS_DIR/makeDDlist_para_2.cpp -o $RPPPS_DIR/makeDDlist_para_2.linux

#this is for running one by one round, each round is in parallel
c++ $RPPPS_DIR/makeDDlist_para_3.cpp -o $RPPPS_DIR/makeDDlist_para_3.linux

#to generate the script to copy candidates from each folder
echo "Compiling CpCand.cpp"
c++ $RPPPS_DIR/CpCand.cpp -o $RPPPS_DIR/CpCand.linux

#cut filterbank data
echo "Compiling filterbank_cut.cpp"
c++ $RPPPS_DIR/filterbank_cut.cpp -o $RPPPS_DIR/filterbank_cut.linux

#pfd header extract
echo "Compiling read_pfd.cpp"
c++ $RPPPS_DIR/read_pfd.cpp -o $RPPPS_DIR/read_pfd.linux

echo "Compilation finised."
echo "Don't forget setting RPPPS_DIR path!"
echo
echo "Have fun with RPPPS! Thank you!"
echo



