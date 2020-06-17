//input the coordinate of the beginning of the drift of the observation
//out put the parameter file for the modify_FASTpsrfits.py to change the coordinates of the beams

#include<stdio.h>
#include<string.h>
int main(int argc, char * argv[]){
  //read the input as floats, in unit of degree
  //RA first, then DEC, subints and length of subints in the end
  float ra=0.0,dec=0.0;
  int subint=0;
  float subint_length=0.0;
  sscanf(argv[1],"%f",&ra);
  sscanf(argv[2],"%f",&dec);
  sscanf(argv[3],"%d",&subint);
  sscanf(argv[4],"%f",&subint_length);
  printf("Original RA %f, DEC %f.\n Beam starts at subint %d, subint length %f\n", ra, dec, subint, subint_length);
  FILE *outfile;
  if((outfile=fopen("coordinate_beam.txt","w"))==NULL){
    printf("\nI can't create output file!\n");
    return 1;
  }
  //the format of the file, coordinate_beam.txt
  //RA 11:22:33.4455
  //DEC 22:33:44.5566
  ra=ra+subint*subint_length*15.0/3600.0/((23.0*3600.0+56.0*60.0+4.0)/(24.0*3600.0));
  while(ra>=360){
    ra=ra-360.0;
  }
  int ra_h=int(ra/15.0);
/*  if(ra_h >= 24){
    ra_h=ra_h-24;
  }*/
  int ra_m=int((ra-ra_h*15.0)*60.0/15.0);
  float ra_s=ra*3600.0/15.0-ra_h*3600.0-ra_m*60.0;
  int dec_d=0;
  int dec_m=0;
  float dec_s=0.0;
  char sign[]="+";
  if(dec>=0.0){
    strcpy(sign,"+");
    dec_d=int(dec);
    dec_m=int((dec-dec_d)*60.0);
    dec_s=dec*3600.0-dec_d*3600.0-dec_m*60.0;
  }
  if(dec<0){
    strcpy(sign,"-");
    dec=dec*(-1.0);
    dec_d=int(dec);
    dec_m=int((dec-dec_d)*60.0);
    dec_s=dec*3600.0-dec_d*3600.0-dec_m*60.0;
  }
  fprintf(outfile,"RA %02d:%02d:%05.2f\n",ra_h,ra_m,ra_s);
  fprintf(outfile,"DEC %s%02d:%02d:%05.2f\n",sign,dec_d,dec_m,dec_s);
  printf("Modified RA %02d:%02d:%05.2f\n",ra_h,ra_m,ra_s);
  printf("Modified DEC %s%02d:%02d:%05.2f\n",sign,dec_d,dec_m,dec_s);
  printf("Modified RA in degree %05.2f\n",ra);
  printf("Modified DEC in degree %05.2f\n", dec);
  fclose(outfile);
  return 0;
}

