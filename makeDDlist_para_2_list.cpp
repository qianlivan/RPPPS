#include <stdio.h>
#include <string.h>
#include "DDplan.h"

void init(){
//initial display
  printf("\nThis is a part of RPPPS package.\n");
  printf("This is the code used for creating a shell script for parallel running PRESTO.\n");
  printf("This also saves disk space!\n");
  printf("NOTICT: modifying the source before using it.\n");
  printf("Last modified 2016-11-07.\n\n");
}

int main(int argc, char *argv[]){

  float totalround=0,roundcount=0;

  init();

//input checking
//example: makeDDlist_para (filename) (total numout) (paranumber) (accelnum)
  if(argc<3){
    printf("Not enough parameters.\n");
    printf("Here is an example: \n");
    printf("usage: ().linux filename numout para_num accle_num\n");
    printf("example: makeDDlist_para.linux PM0001_00011 8400896 (8) (0) (ext)\n\n");
    return 1;
  }

  //if the acceleration number is set, read it
  int accelnum=0;
  if(argc>=5){
    sscanf(argv[4],"%d",&accelnum);
  } else {
    printf("Warning: no acceleration number set, use 0 as default.\n\n");
  }

  //if the parallel number is set, read it
  int para_num=8;
  if(argc>=4){
    sscanf(argv[3],"%d",&para_num);
  } else {
    printf("Warning: no parallel threads number set, use 8 as default.\n\n");
  }
  //printf("%d\n", para_num);

//read numout
  int numout=0;
  sscanf(argv[2],"%d",&numout);

  int a=0;
  for(a=0;a<numcall;a++){
    totalround=totalround+numDMs[a];
  }
  //totalround++;

  FILE *outfile;
  static char outfilename[100]={'\0'}, maskfile[100]={'\0'}, rawfile[100]={'\0'};
  char DDpara[para_num][100], FFTpara[para_num][100];
  int i=0, j=0, k=0;
  int tmp=0, tmp1=0, tmp2=0;//for counting repeats
  static char datfile[100]={'\0'}, FFTfile[100]={'\0'}, DDout[100]={'\0'};
  float DDlist[10000]={0};
  long int Downsamplist[10000]={0};

//create DDlist
  printf("Calculating dedispersion list.....\n");
  for(i=0;i<numcall;i++){
    for(j=0;j<numDMs[i];j++){
      DDlist[tmp]=startDM[i]+DMstep[i]*j;
      Downsamplist[tmp]=downsamp[i];
      tmp++;
    }
  }

//create output file
  printf("Creating output files......");
  sprintf(outfilename,"%s%s",argv[1],"_DDpara_2_list.bash");
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

//script example:
//mkdir 1
//cd 1
//echo "prepdata -mask ../() -numout () -dm () -downsamp () -o () ../()" >> tmp.bash
//echo "realfft () -fwd" >> tmp.bash
//echo "accelsearch -zmax 0 -numharm 16 ()" >> tmp.bash
//echo "mv ./*_ACCEL* ../" >> tmp.bash
//echo "cd .." >> tmp.bash
//echo "rm -rdf 1" >> tmp.bash
//chmod 777 ./tmp.bash
//bash ./tmp.bash &
//

  fprintf(outfile,"echo \"Parallely dedispersing, FFT and acceleration searching scripts will be run.\"\n");
  fprintf(outfile,"echo \"Total round %.0f.\"\n",totalround);
  fprintf(outfile,"echo \"Parallel %d.\"\n",para_num);
  fprintf(outfile,"echo \"\"\n\n",para_num);
  fprintf(outfile,"username=`whoami`\n\n");

  for(i=1;i<=tmp;i++){
    sprintf(datfile,"%s%s%.2f",argv[1],"_DM",DDlist[i-1]);
    fprintf(outfile,"mkdir %d\n",tmp1);
    fprintf(outfile,"cd %d\n", tmp1);
    //fprintf(outfile,"echo \"prepdata -mask ../%s -numout %d -dm %.2f -downsamp %d -o %s ../%s\" >> tmp.bash \n", maskfile, numout/Downsamplist[i-1], DDlist[i-1], Downsamplist[i-1], datfile, rawfile);
    fprintf(outfile,"echo \"prepdata -mask ../%s -dm %.2f -downsamp %d -o %s ../../*.fits\" >> tmp.bash \n", maskfile, DDlist[i-1], Downsamplist[i-1], datfile);
    sprintf(FFTfile,"%s%s",datfile,".fft");
    sprintf(datfile,"%s%s",datfile,".dat");
    fprintf(outfile,"echo \"realfft %s -mem -fwd\" >> tmp.bash \n", datfile);
    fprintf(outfile,"echo \"accelsearch -inmem -zmax %d -numharm 16 %s\" >> tmp.bash \n", accelnum, FFTfile);

    //added single pulse search, 2015-08-11
    //as we will use something else, such as heimdall for single pulse search,
    //this part in PRESTO will be removed. 2016-11-07
    //fprintf(outfile,"echo \"single_pulse_search.py -b *.dat\" >> tmp.bash \n");
    //move single pulse serach result file
    //fprintf(outfile,"echo \"mv ./*.singlepulse ../\" >> tmp.bash \n");

    
    fprintf(outfile,"echo \"mv ./*.inf ../\" >> tmp.bash \n");
    fprintf(outfile,"echo \"mv ./*_ACCEL* ../\" >> tmp.bash \n");
    //fprintf(outfile,"echo \"sleep 2\" >> tmp.bash");

    //fprintf(outfile,"echo \"cd .. \" >> tmp.bash \n");
    //fprintf(outfile,"echo \"rm -rdf %d\" >> tmp.bash \n", tmp1);
    fprintf(outfile,"chmod 777 tmp.bash\n");
    fprintf(outfile,"bash tmp.bash >> /dev/null &\n");
    fprintf(outfile,"cd ..\n\n");
    tmp1++;
    if(tmp1==para_num){
      tmp1=0;
      fprintf(outfile,"a=`ps augx | grep $username | grep prepdata | wc -l`\n");
      fprintf(outfile,"b=`ps augx | grep $username | grep realfft | wc -l`\n");
      fprintf(outfile,"c=`ps augx | grep $username | grep accelsearch | wc -l`\n");
      fprintf(outfile,"d=`ps augx | grep $username | grep single_pulse_search | wc -l`\n");
      fprintf(outfile,"((e=a+b+c+d)) \n");
      fprintf(outfile,"while(( \"$e\" != \"4\" ))\n");
      fprintf(outfile,"do \n");
      fprintf(outfile,"((e=e-4))\n");
      fprintf(outfile,"((a--))\n");
      fprintf(outfile,"((b--))\n");
      fprintf(outfile,"((c--))\n");
      fprintf(outfile,"((d--))\n");
      fprintf(outfile,"echo \"$e threads left: prepdata $a, realfft $b, accelesearch $c, single-pulse $d\"\n");
      fprintf(outfile,"  sleep 2 \n");
      fprintf(outfile,"  a=`ps augx | grep $username | grep prepdata | wc -l`\n");
      fprintf(outfile,"  b=`ps augx | grep $username | grep realfft | wc -l`\n");
      fprintf(outfile,"  c=`ps augx | grep $username | grep accelsearch | wc -l`\n");
      fprintf(outfile,"  d=`ps augx | grep $username | grep single_pulse_search | wc -l`\n");
      fprintf(outfile,"((e=a+b+c+d)) \n");
      fprintf(outfile,"done \n\n");
      for(j=0;j<para_num;j++){
        fprintf(outfile,"rm -rdf %d \n",j);
      }
      roundcount=roundcount+para_num;
      fprintf(outfile,"echo \"Finish percentage: %.2f\%\"\n",roundcount/totalround*100);
      fprintf(outfile,"\n");
    }
  }
  fprintf(outfile,"a=`ps augx | grep $username | grep prepdata | wc -l`\n");
  fprintf(outfile,"b=`ps augx | grep $username | grep realfft | wc -l`\n");
  fprintf(outfile,"c=`ps augx | grep $username | grep accelsearch | wc -l`\n");
  fprintf(outfile,"d=`ps augx | grep $username | grep single_pulse_search | wc -l`\n");
  fprintf(outfile,"((e=a+b+c+d)) \n");
  fprintf(outfile,"while(( \"$e\" != \"4\" ))\n");
  fprintf(outfile,"do \n");
  fprintf(outfile,"((e=e-4))\n");
  fprintf(outfile,"((a--))\n");
  fprintf(outfile,"((b--))\n");
  fprintf(outfile,"((c--))\n");
  fprintf(outfile,"((d--))\n");
  fprintf(outfile,"echo \"$e threads left: prepdata $a, realfft $b, accelsearch $c, single-pulse $d\"\n");
  fprintf(outfile,"  sleep 2 \n");
  fprintf(outfile,"  a=`ps augx | grep $username | grep prepdata | wc -l`\n");
  fprintf(outfile,"  b=`ps augx | grep $username | grep realfft | wc -l`\n");
  fprintf(outfile,"  c=`ps augx | grep $username | grep accelsearch | wc -l`\n");
  fprintf(outfile,"  d=`ps augx | grep $username | grep single_pulse_search | wc -l`\n");
  fprintf(outfile,"((e=a+b+c+d)) \n");
  fprintf(outfile,"done \n\n");
  for(j=0;j<tmp1;j++){
    fprintf(outfile,"rm -rdf %d\n",j);
  }
  fprintf(outfile,"echo \"Scripts Finished.\"\n");
  fprintf(outfile,"echo\n");



  fclose(outfile);
  return 0;
}
