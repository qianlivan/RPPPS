#include <stdio.h>
#include <string.h>

void init(){
//initial display
  printf("\nThis is a part of RPPPS package.\n");
  printf("This is the code used for creating a shell script for parallel running PRESTO.\n");
  printf("This also saves disk space!\n");
  printf("NOTICT: checking modifying the dedispersion setting in source before using it.\n");
  printf("Last modified 2016-12-19.\n\n");
}

int main(int argc, char *argv[]){

//This is the dedispersion plan
//For PMPS, the command we may use is :
//DDplan.py -l 0.0 -d 3000.0 -f 1374 -b 228 -n 96 -c 0 -t 0.00025 -s 96 -r 0.5

/* For high DM search
  int numcall=1;
  float startDM[]={300.0};
  float DMstep[]={2.0};
  int downsamp[]={8};
  int numDMs[]={500};
*/

/* For DM 2000 search, from the given Scott's example
  int numcall=6;
  float startDM[]={0.0, 88.0, 151.0, 289.0, 639.0, 1279.0};
  float DMstep[]={0.5, 1.0, 2.0, 5.0, 10.0, 20.0};
  int downsamp[]={1, 2, 4, 8, 16, 32};
  int numDMs[]={176, 63, 69, 70, 64, 37};
*/

//For DM 4000 search, for reprocessing PMPS data
/*  int numcall=6;
  float startDM[]={0.0, 114.0, 266.0, 476.0, 856.0, 2376.0};
  float DMstep[]={0.5, 1.0, 3.0, 5.0, 10.0, 20.0};
  int downsamp[]={1, 2, 4, 8, 16, 32};
  int numDMs[]={228, 152, 70, 76, 152, 76};
*/

//For FAST cut test, DM 0 to 2000, 400-600 MHz, 0.25 MHz res, 0.2 ms sampling, 0.5 ms time res
  int numcall=6;
  float startDM[]={0.0, 67.15, 127.65, 251.85, 556.35, 1112.35};
  float DMstep[]={0.05, 0.1, 0.2, 0.5, 1.0, 2.0};
  //int downsamp[]={2, 4, 8, 16, 32, 64};
  int downsamp[]={2, 4, 10, 20, 25, 50};
  int numDMs[]={1343, 605, 621, 609, 556, 444};

  float totalround=0,roundcount=0;

  init();

//input checking
//example: makeDDlist_para (filename) (total numout) (paranumber) (accelnum)
  if(argc<3){
    printf("Not enough parameters.\n");
    printf("Here is an example: \n");
    printf("usage: ().linux filename numout para_num accle_num\n");
    printf("example: makeDDlist_para_3.linux PM0001_00011 8400896 (8) (0) (ext)\n\n");
    return 1;
  }

  //if the acceleration number is set, read it
  int accelnum=0;
  if(argc>=5){
    sscanf(argv[4],"%d",&accelnum);
    printf("Acceleration zmax: %d\n",accelnum);
  } else {
    printf("Warning: no acceleration number set, use 0 as default.\n\n");
  }

  //if the parallel number is set, read it
  int para_num=8;
  if(argc>=4){
    sscanf(argv[3],"%d",&para_num);
    printf("Number of parallel process: %d\n",para_num);
  } else {
    printf("Warning: no parallel threads number set, use 8 as default.\n\n");
  }

//read numout
  int numout=0;
  sscanf(argv[2],"%d",&numout);

  int a=0;
  for(a=0;a<numcall;a++){
    totalround=totalround+numDMs[a];
  }

  FILE *outfile;
  static char outfilename[100]={'\0'}, maskfile[100]={'\0'}, rawfile[100]={'\0'};
  char DDpara[para_num][100], FFTpara[para_num][100];
  int i=0, j=0, k=0;
  int tmp=0, tmp1=0, tmp2=0;//for counting repeats
  static char datfile[100]={'\0'}, FFTfile[100]={'\0'}, DDout[100]={'\0'};
  float DDlist[5000]={0};
  long int Downsamplist[5000]={0};

//create DDlist
  for(i=0;i<numcall;i++){
    for(j=0;j<numDMs[i];j++){
      DDlist[tmp]=startDM[i]+DMstep[i]*j;
      Downsamplist[tmp]=downsamp[i];
      tmp++;
      //printf("%d\n",tmp);
    }
  }
  //printf("%d, %.1f\n",totalround,tmp);

//create output file
  sprintf(outfilename,"%s%s",argv[1],"_DDpara_3.bash");
  sprintf(maskfile,"%s%s",argv[1],"_rfifind.mask");
  if(argc==6){
    printf("The extension is given as %s.\n", argv[5]);
    sprintf(rawfile,"%s%s",argv[1],argv[5]);
  }else{
    printf("The extension is not given, use .sf as default.\n");
    sprintf(rawfile,"%s%s",argv[1],".sf");
  }

  if((outfile=fopen(outfilename,"w"))==NULL){
    printf("\nI can't create output file!\n");
    return 1;
  }

//displaying parallel information
  printf("\nCreating the script......\n");
  fprintf(outfile,"echo \"Parallely dedispersing, FFT and acceleration searching scripts will be run.\"\n");
  fprintf(outfile,"echo \"Total round %.0f.\"\n",totalround);
  fprintf(outfile,"echo \"Parallel %d.\"\n",para_num);
  fprintf(outfile,"echo \"\"\n\n");
  fprintf(outfile,"username=`whoami`\n\n");

//create all the sub-tmp-folders in which the scripts are contained.
  fprintf(outfile,"echo Makinig %.0f folders and scriptsfor dedispersing, FFT, and acceleration search.\n\n", totalround);

//create empty files
  fprintf(outfile,"file_prepdata=\"tmp_prepdata.txt\"\n");
  fprintf(outfile,"file_realfft=\"tmp_realfft.txt\"\n");
  fprintf(outfile,"file_accelsearch=\"tmp_accelsearch.txt\"\n");
  fprintf(outfile,"echo \"00\" > ../$file_prepdata\n");
  fprintf(outfile,"echo \"00\" > ../$file_realfft\n");
  fprintf(outfile,"echo \"00\" > ../$file_accelsearch\n\n");

//make all the folders for trials
  for(i=0;i<totalround;i++){
    //forming the time series filename
    sprintf(datfile,"%s%s%.2f",argv[1],"_DM",DDlist[i-1]);
    //making the folder
    fprintf(outfile,"mkdir %d\n", i);
    //entering
    fprintf(outfile,"cd %d\n", i);
    //dedispersion command
    
    //prepdata++, tmp_prepdata.txt
    fprintf(outfile,"echo \"num_prepdata=\\`cat ../$file_prepdata\\`\" >> tmp.bash\n");
    fprintf(outfile,"echo \"((num_prepdata++))\" >> tmp.bash\n");
    fprintf(outfile,"echo \"echo \"\\$num_prepdata\" > ../$file_prepdata\" >> tmp.bash\n");

    //printf("%d, %s, %d, %.1f, %d, %s, %s\n", i, maskfile, numout/Downsamplist[i], DDlist[i], Downsamplist[i], datfile, rawfile);
    fprintf(outfile,"echo \"prepdata -mask ../%s -numout %d -dm %.1f -downsamp %d -o %s ../%s\" >> tmp.bash \n", maskfile, numout/Downsamplist[i], DDlist[i], Downsamplist[i], datfile, rawfile);

    //prepdata--, tmp_prepdata.txt
    fprintf(outfile,"echo \"num_prepdata=\\`cat ../$file_prepdata\\`\" >> tmp.bash\n");
    fprintf(outfile,"echo \"((num_prepdata--))\" >> tmp.bash\n");
    fprintf(outfile,"echo \"echo \"\\$num_prepdata\" > ../$file_prepdata\" >> tmp.bash\n");

    //realfft++, tmp_realfft.txt
    fprintf(outfile,"echo \"num_realfft=\\`cat ../$file_realfft\\`\" >> tmp.bash\n");
    fprintf(outfile,"echo \"((num_realfft++))\" >> tmp.bash\n");
    fprintf(outfile,"echo \"echo \"\\$num_realfft\" > ../$file_realfft\" >> tmp.bash\n");

    //FFF command
    sprintf(FFTfile,"%s%s",datfile,".fft");
    sprintf(datfile,"%s%s",datfile,".dat");
    fprintf(outfile,"echo \"realfft %s -mem -fwd\" >> tmp.bash \n", datfile);

    //realfft--, tmp_realfft.txt
    fprintf(outfile,"echo \"num_realfft=\\`cat ../$file_realfft\\`\" >> tmp.bash\n");
    fprintf(outfile,"echo \"((num_realfft--))\" >> tmp.bash\n");
    fprintf(outfile,"echo \"echo \"\\$num_realfft\" > ../$file_realfft\" >> tmp.bash\n");

    //accelsearch++, tmp_accelsearch.txt
    fprintf(outfile,"echo \"num_accelsearch=\\`cat ../$file_accelsearch\\`\" >> tmp.bash\n");
    fprintf(outfile,"echo \"((num_accelsearch++))\" >> tmp.bash\n");
    fprintf(outfile,"echo \"echo \"\\$num_accelsearch\" > ../$file_accelsearch\" >> tmp.bash\n");

    //acceleration search command
    fprintf(outfile,"echo \"accelsearch -inmem -zmax %d -numharm 16 %s\" >> tmp.bash \n", accelnum, FFTfile);
    //moving result files out of the folder
    fprintf(outfile,"echo \"mv ./*_ACCEL* ../\" >> tmp.bash \n");
    fprintf(outfile,"echo \"mv ./*.inf ../\" >> tmp.bash \n");

    //accelsearch++, tmp_accelsearch.txt
    fprintf(outfile,"echo \"num_accelsearch=\\`cat ../$file_accelsearch\\`\" >> tmp.bash\n");
    fprintf(outfile,"echo \"((num_accelsearch--))\" >> tmp.bash\n");
    fprintf(outfile,"echo \"echo \"\\$num_accelsearch\" > ../$file_accelsearch\" >> tmp.bash\n");

    //run the script
    //fprintf(outfile,"chmod 777 tmp.bash\n");
    //fprintf(outfile,"bash tmp.bash >> /dev/null &\n");
    //exit from the folder
    fprintf(outfile,"cd ..\n\n");
    //remove the folder
    //fprintf(outfile,"rm -rdf %d\n\n", i);
  }

  fprintf(outfile,"echo Dedispersion, FFT, and acceleration search start.\n\n", totalround);

//REPEATS THIS FOR DM TRIALS AND SEARCHES
  for(i=0;i<totalround;i++){
//1, use a while-loop to read the process number, i the number is large, wait for 1 second and then repeat
    //fprintf(outfile,"a=`ps augx | grep $username | grep prepdata | wc -l`\n");
    //fprintf(outfile,"b=`ps augx | grep $username | grep realfft | wc -l`\n");
    //fprintf(outfile,"c=`ps augx | grep $username | grep accelsearch | wc -l`\n");
    //fprintf(outfile,"d=`ps augx | grep $username | grep single_pulse_search | wc -l`\n");
    //fprintf(outfile,"((e=a+b+c+d)) \n");
    fprintf(outfile,"a=`cat $file_prepdata`\n");
    fprintf(outfile,"b=`cat $file_realfft`\n");
    fprintf(outfile,"c=`cat $file_accelsearch`\n");
    //fprintf(outfile,"  ((a--))\n");
    //fprintf(outfile,"  ((b--))\n");
    //fprintf(outfile,"  ((c--))\n");
    fprintf(outfile,"((e=a+b+c)) \n");
    fprintf(outfile,"while(( $e >= %d ))\n", para_num);
    fprintf(outfile,"do \n");
    //fprintf(outfile,"  ((e=e-4))\n");
    //fprintf(outfile,"  ((a--))\n");
    //fprintf(outfile,"  ((b--))\n");
    //fprintf(outfile,"  ((c--))\n");
    //fprintf(outfile,"  ((d--))\n");
    //fprintf(outfile,"  echo \"$e threads left: prepdata $a, realfft $b, accelesearch $c, single-pulse $d. %d trials done or running, %.0f left, %.0f in total.\"\n",i, totalround-i, totalround);
    fprintf(outfile,"  echo \"$e threads left: prepdata $a, realfft $b, accelesearch $c. %d trials done or running, %.0f left, %.0f in total.\"\n",i, totalround-i, totalround);
    fprintf(outfile,"  sleep 0.1 \n");
    //fprintf(outfile,"  a=`ps augx | grep $username | grep prepdata | wc -l`\n");
    //fprintf(outfile,"  b=`ps augx | grep $username | grep realfft | wc -l`\n");
    //fprintf(outfile,"  c=`ps augx | grep $username | grep accelsearch | wc -l`\n");
    //fprintf(outfile,"  d=`ps augx | grep $username | grep single_pulse_search | wc -l`\n");
    //fprintf(outfile,"((e=a+b+c+d)) \n");
    //fprintf(outfile,"((e=a+b+c+d)) \n");
    fprintf(outfile,"a=`cat $file_prepdata`\n");
    fprintf(outfile,"b=`cat $file_realfft`\n");
    fprintf(outfile,"c=`cat $file_accelsearch`\n");
    //fprintf(outfile,"  ((a--))\n");
    //fprintf(outfile,"  ((b--))\n");
    //fprintf(outfile,"  ((c--))\n");
    fprintf(outfile,"((e=a+b+c)) \n");
    fprintf(outfile,"done \n\n");
//2, start one DM trial and search
    fprintf(outfile,"cd %d\n", i);
    fprintf(outfile,"bash tmp.bash >> /dev/null &\n");
    fprintf(outfile,"cd ..\n");
    //fprintf(outfile,"sleep 0.1\n");
    //fprintf(outfile,"  a=`ps augx | grep $username | grep prepdata | wc -l`\n");
    //fprintf(outfile,"  b=`ps augx | grep $username | grep realfft | wc -l`\n");
    //fprintf(outfile,"  c=`ps augx | grep $username | grep accelsearch | wc -l`\n");
    //fprintf(outfile,"  d=`ps augx | grep $username | grep single_pulse_search | wc -l`\n");
    //fprintf(outfile,"  ((e=a+b+c+d-4)) \n");
    //fprintf(outfile,"  ((a--))\n");
    //fprintf(outfile,"  ((b--))\n");
    //fprintf(outfile,"  ((c--))\n");
    //fprintf(outfile,"  ((d--))\n");
    fprintf(outfile,"a=`cat $file_prepdata`\n");
    fprintf(outfile,"b=`cat $file_realfft`\n");
    fprintf(outfile,"c=`cat $file_accelsearch`\n");
    //fprintf(outfile,"  ((a--))\n");
    //fprintf(outfile,"  ((b--))\n");
    //fprintf(outfile,"  ((c--))\n");
    fprintf(outfile,"((e=a+b+c)) \n");
    //fprintf(outfile,"  echo \"$e threads left: prepdata $a, realfft $b, accelesearch $c, single-pulse $d. %d trials done or running, %.0f left, %.0f in total.\"\n",i, totalround-i, totalround);
    fprintf(outfile,"  echo \"$e threads left: prepdata $a, realfft $b, accelesearch $c. %d trials done or running, %.0f left, %.0f in total.\"\n",i, totalround-i, totalround);
    //fprintf(outfile,"  ((e=a+b+c+d+4)) \n");
    //fprintf(outfile,"echo %d trials done or running, %.0f left, %.0f in total.\n\n",i+1, totalround-i-1, totalround);
//repeat 1 and 2
  }

//wait for all process finish
  //fprintf(outfile,"a=`ps augx | grep $username | grep prepdata | wc -l`\n");
  //fprintf(outfile,"b=`ps augx | grep $username | grep realfft | wc -l`\n");
  //fprintf(outfile,"c=`ps augx | grep $username | grep accelsearch | wc -l`\n");
  //fprintf(outfile,"d=`ps augx | grep $username | grep single_pulse_search | wc -l`\n");
  //fprintf(outfile,"((e=a+b+c+d)) \n");
  fprintf(outfile,"a=`cat $file_prepdata`\n");
  fprintf(outfile,"b=`cat $file_realfft`\n");
  fprintf(outfile,"c=`cat $file_accelsearch`\n");
  //fprintf(outfile,"  ((a--))\n");
  //fprintf(outfile,"  ((b--))\n");
  //fprintf(outfile,"  ((c--))\n");
  fprintf(outfile,"((e=a+b+c)) \n");
  fprintf(outfile,"while(( $e != 0 ))\n");
  fprintf(outfile,"do \n");
  //fprintf(outfile,"  ((e=e-4))\n");
  //fprintf(outfile,"  ((a--))\n");
  //fprintf(outfile,"  ((b--))\n");
  //fprintf(outfile,"  ((c--))\n");
  //fprintf(outfile,"  ((d--))\n");
  //fprintf(outfile,"  echo \"$e threads left: prepdata $a, realfft $b, accelesearch $c, single-pulse $d\"\n");
    //fprintf(outfile,"  echo \"$e threads left: prepdata $a, realfft $b, accelesearch $c, single-pulse $d. %d trials done or running, %.0f left, %.0f in total.\"\n",i, totalround-i, totalround);
    fprintf(outfile,"  echo \"$e threads left: prepdata $a, realfft $b, accelesearch $c. %d trials done or running, %.0f left, %.0f in total.\"\n",i, totalround-i, totalround);
  fprintf(outfile,"  sleep 1 \n");
  //fprintf(outfile,"  a=`ps augx | grep $username | grep prepdata | wc -l`\n");
  //fprintf(outfile,"  b=`ps augx | grep $username | grep realfft | wc -l`\n");
  //fprintf(outfile,"  c=`ps augx | grep $username | grep accelsearch | wc -l`\n");
  //fprintf(outfile,"  d=`ps augx | grep $username | grep single_pulse_search | wc -l`\n");
  //fprintf(outfile,"((e=a+b+c+d)) \n");
  fprintf(outfile,"  a=`cat $file_prepdata`\n");
  fprintf(outfile,"  b=`cat $file_realfft`\n");
  fprintf(outfile,"  c=`cat $file_accelsearch`\n");
  //fprintf(outfile,"  ((a--))\n");
  //fprintf(outfile,"  ((b--))\n");
  //fprintf(outfile,"  ((c--))\n");
  fprintf(outfile,"  ((e=a+b+c)) \n");
  fprintf(outfile,"done \n\n");
  
  fprintf(outfile,"echo All process finished.\necho Removing the %.0f tmp folders.\n",totalround);

  for(i=0;i<totalround;i++){
    fprintf(outfile,"rm -rdf %d\n", i);
  }
  fprintf(outfile, "rm -rdf $file_prepdata\n");
  fprintf(outfile, "rm -rdf $file_realfft\n");
  fprintf(outfile, "rm -rdf $file_accelsearch\n");

  printf("Script creation finished.\n\n");
  fclose(outfile);
  return 0;
}

