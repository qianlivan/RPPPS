//This is the dedispersion plan
//For PMPS, the command we may use is :
//DDplan.py -l 0.0 -d 3000.0 -f 1374 -b 228 -n 96 -c 0 -t 0.00025 -s 96 -r 0.5

//For FAST low frequency search, 290 to 340 MHz, 200 channels
//DDplan.py -l 0 -d 800 -f 315 -b 50 -n 200 -t 0.0001 -r 1.0
  int numcall=6;
  float startDM[]={0.0, 31.5, 62.9, 139.4, 278.4, 556.4};
  float DMstep[]={0.1, 0.2, 0.5, 1.0, 2.0, 3.0};
//  int downsamp[]={8, 16, 32, 64, 128, 256};
  int downsamp[]={8, 16, 32, 32, 32, 32};
//  int numDMs[]={315, 157, 153, 139, 139, 100};
  int numDMs[]={315, 157, 153, 139, 139, 500};

//For searching for FAST 201611 test data
//DDplan.py -l 0 -d 2000 -f 650 -b 150 -n 500 -t 0.001 -r 0.001
/*  int numcall=4;
  float startDM[]={0.0, 400.0, 703.0, 1353.0};
  float DMstep[]={0.5, 1.0, 2.0, 3.0};
  int downsamp[]={1, 2, 4, 10};
  int numDMs[]={800, 303, 325, 216};
*/

//For DM 4000 search, for reprocessing PMPS data
/*  int numcall=6;
  float startDM[]={0.0, 114.0, 266.0, 476.0, 856.0, 2376.0};
  float DMstep[]={0.5, 1.0, 2.0, 5.0, 10.0, 20.0};
  int downsamp[]={1, 2, 4, 8, 16, 32};
  int numDMs[]={228, 152, 70, 76, 152, 76};
*/

//For 2017 George simulation test 1
//DDplan.py -l 0 -d 5000 -f 1250 -b 400 -n 256 -t 0.000256 -r 1.0
/*  int numcall=4;
  float startDM[]={0.0, 215.0, 409.0, 807.0};//, 1782.0, 3562.0};
  float DMstep[]={0.5, 1.0, 2.0, 5.0};//, 10.0, 20.0};
  int downsamp[]={2, 4, 8, 16};//, 32, 64};
  int numDMs[]={430, 194, 199, 195};//, 178, 72};
*/

//For cluster test, low frequency 300-400 MHz
//For small_low
//DDplan.py -l 0 -d 1200 -f 350.03954 -b 100 -n 400 -t 0.0001 -r 0.5
//ignore the last DM trials
/*  int numcall=6;
  float startDM[]={0.0, 20.85, 38.9, 77.5, 178.9, 331.9};//, 663.9};
  float DMstep[]={0.03, 0.05, 0.10, 0.30, 0.5, 1.0};//, 3.0};
  int downsamp[]={4, 8, 16, 32, 64, 64, 128};//, 256};
  int numDMs[]={695, 361, 386, 338, 306, 332};//, 179};
*/

//For cluster test, middle frequency 500-700 MHz
//For small_mid
//DDplan.py -l 0 -d 1200 -f 599.97852 -b 200 -n 800 -t 0.0001 -r 0.5
//ignore the last DM trials
/*  int numcall=6;
  float startDM[]={0.0, 38.01, 61.41, 117.41, 232.01, 411.11};//, 777.61};
  float DMstep[]={0.03, 0.05, 0.10, 0.20, 0.30, 0.5};//, 1.0};
  int downsamp[]={1, 2, 4, 8, 16, 32, 64};//, 64};
  int numDMs[]={1267, 468, 560, 573, 597, 733};//, 423};
*/

//For cluster test, high frequency 1050-1450 MHz
//For small_high, mid, long, and very long
//DDplan.py -l 0 -d 2000 -f 1250 -b 400 -n 1600 -t 0.0001 -r 0.5
//ignore the last DM trials
/*  int numcall=2;
  float startDM[]={0.0, 1115.0};
  float DMstep[]={0.5, 1.0};
  int downsamp[]={4, 8};
  int numDMs[]={2230, 885};
*/

//For FAST cut test, DM 0 to 2000, 400-600 MHz, 0.25 MHz res, 0.2 ms sampling, 0.5 ms time res
/*  int numcall=6;
  float startDM[]={0.0, 67.15, 127.65, 251.85, 556.35, 1112.35};
  float DMstep[]={0.05, 0.1, 0.2, 0.5, 1.0, 2.0};
  //int downsamp[]={2, 4, 8, 16, 32, 64};
  int downsamp[]={2, 4, 10, 20, 25, 50};
  int numDMs[]={1343, 605, 621, 609, 556, 444};*/
  //int numcall=2;
  //float startDM[]={0.0, 67.15};
  //float DMstep[]={0.05, 0.1};
  //int downsamp[]={2, 4, 8, 16, 32, 64};
  //int downsamp[]={2, 4};
  //int numDMs[]={1343, 605};

  //int numcall=1;//6 rounds in total
  //float startDM[]={0.0, 88.0, 151.0, 289.0, 639.0, 1279.0};//start DM
  //float startDM[]={0.0, 114.0, 266.0, 476.0, 856.0, 2376.0};
  //float startDM[]={300.0};
  //float DMstep[]={0.5, 1.0, 2.0, 5.0, 10.0, 20.0};//separation of each DM
  //float DMstep[]={0.5, 1.0, 3.0, 5.0, 10.0, 20.0};
  //float DMstep[]={2.0};
  //int downsamp[]={1, 2, 4, 8, 16, 32};
  //int downsamp[]={8};
  //int numDMs[]={176, 63, 69, 70, 64, 37};//number of DMs of each round
  //int numDMs[]={228, 152, 70, 76, 152, 76};
  //int numDMs[]={500};
