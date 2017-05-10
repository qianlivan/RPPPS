//Read the information from PRESTO prefold output file .pfd
//results from analysis the data in the files

#include <stdio.h>
#include <string.h>
#include <math.h>
//#include <malloc.h>
#include <stdlib.h>

//to read the 32 bits little_endian int from the file, written by hkuclion
//another function is fgetw(FILE *fp) maybe have the same operation
int read_int(FILE *file, int offset){
        int int_1=0;
        fseek(file,offset,0);
        int_1=fgetc(file)<<0 | fgetc(file)<<8 | fgetc(file)<<16 | fgetc(file)<<24;
        return int_1;
}

int main(int argc, char *argv[]){
  //show some welcome information :-)
  printf("\nProcess PRESTO prepfold output file .pfd, out put data in binary data.\n");
  printf("Last modified: 20170420\n\n");

  //define input and output files
  FILE *in_file,*out_file;
  int i=0, j=0;
  double unknown=0.0;
  float unknown1=0.0;

  //if there is no input parameter, just show the help infomation:
  if(!(argc>1)){
    printf("Usage: read_pfd (pfd file)\n");
    printf("No input file.\n");
    return 0;
  }

  //if with more than one input file, show the warning:
  if (!(argc>=2)){
    printf("WARNING: ONLY process the first file!!!\n");
  }

  //if with input file, open it
  if((in_file=fopen(argv[1],"rb"))==NULL){
    printf("The file %s do not exist!\n",argv[1]);
    return 1;
  }
  long int fp=0;
  
  //the first 12 unsigned integers are from the header information
  int header_info[12]={'\0'};
  for(i=0;i<=11;i++){
    header_info[i]=read_int(in_file,fp);
    fp=fp+4;
  }
  
  //obtain the file size
  fseek(in_file, 0, SEEK_END);
  printf("File size is %d, or, 0x%X.\n", ftell(in_file), ftell(in_file));
  fseek(in_file, 0, SEEK_SET);
  
  printf("The header information is: \n");
  /*
  for(i=0;i<=11;i++){
    printf("%d  ",header_info[i]);
  }
  printf("\n");
  */
  printf("Number of DM values for DM search: %d.\n", header_info[0]);
  printf("Number of period values for period search: %d\n", header_info[1]);
  printf("Number of p-dot values for period search: %d\n", header_info[2]);
  printf("Number of channels (nsub): %d\n", header_info[3]);
  printf("Number of sun-ints (npart): %d\n", header_info[4]);
  printf("Number of data points in pulse profile: %d\n", header_info[5]);
  printf("Other values: %d, %d, %d, %d, %d, %d.\n", header_info[6], header_info[7], header_info[8], header_info[9], header_info[10], header_info[11]);
  printf("\n");
  
  //the pulsar data file name
  fseek(in_file, fp, SEEK_SET);
  int filename_length=read_int(in_file,fp);
  char filename[100]={'\0'};
  fread(filename, filename_length, 1, in_file);
  printf("The pulsar data file name is: %s\n", filename);
  fp=fp+filename_length+4;
  
  //the candidates index?
  fseek(in_file, fp, SEEK_SET);
  int cand_index_length=read_int(in_file,fp);
  char cand_index_name[100]={'\0'};
  fread(cand_index_name, cand_index_length, 1, in_file);
  printf("The candidate index name is: %s\n", cand_index_name);
  fp=fp+cand_index_length+4;
  
  //the telescope name?
  fseek(in_file, fp, SEEK_SET);
  int tel_name_length=read_int(in_file,fp);
  char tel_name[100]={'\0'};
  fread(tel_name, tel_name_length, 1, in_file);
  printf("The telescope name is: %s.\n", tel_name);
  fp=fp+tel_name_length+4;
  
  //the ps file name
  fseek(in_file, fp, SEEK_SET);
  int ps_name_length=read_int(in_file,fp);
  char ps_name[200]={'\0'};
  fread(ps_name, ps_name_length, 1, in_file);
  printf("The PS file name is: %s\n", ps_name);
  fp=fp+ps_name_length+4;

  //RA and DEC, the string
  //!!!!!!!!!16 bytes, I won't know if it will changed.
  fseek(in_file, fp, SEEK_SET);
  char ra_string[16]={'\0'}, dec_string[16]={'\0'};
  fread(ra_string, 16, 1, in_file);
  fp=fp+16;
  fseek(in_file, fp, SEEK_SET);
  fread(dec_string, 16, 1, in_file);
  fp=fp+16;
  fseek(in_file, fp, SEEK_SET);
  printf("The RA and DEC are: %s, %s.\n", ra_string, dec_string);
  printf("\n");

  //sampling time (second, 8 bytes)
  double sampling_t=0;
  fread(&sampling_t, sizeof(double), 1, in_file);
  printf("The sampling time is %f second.\n", sampling_t);
  fp=fp+8;
  //file start position, normalized
  fseek(in_file, fp, SEEK_SET);
  double file_start=0;
  fread(&file_start, sizeof(double), 1, in_file);
  printf("File start position, %f.\n", file_start);
  fp=fp+8;
  //file end position, mormarized
  fseek(in_file, fp, SEEK_SET);
  double file_end=0;
  fread(&file_end, sizeof(double), 1, in_file);
  printf("File end position, %f.\n", file_end);
  fp=fp+8;
  //epoch, topo
  fseek(in_file, fp, SEEK_SET);
  double epoch_topo=0;
  fread(&epoch_topo, sizeof(double), 1, in_file);
  printf("The Epoch, topocentric, %f.\n", epoch_topo);
  fp=fp+8;
  //epoch, bary
  fseek(in_file, fp, SEEK_SET);
  double epoch_bary=0;
  fread(&epoch_bary, sizeof(double), 1, in_file);
  printf("The Epoch, barycentric, %f.\n", epoch_bary);
  fp=fp+8;
  
  fseek(in_file, fp, SEEK_SET);
  //double unknown=0;
  fread(&unknown, sizeof(double), 1, in_file);
  printf("Unknown value, %f.\n", unknown);
  fp=fp+8;
  
  fseek(in_file, fp, SEEK_SET);
  double f_low=0;
  fread(&f_low, sizeof(double), 1, in_file);
  printf("Low channel frequency, %f MHz.\n", f_low);
  fp=fp+8;
  
  fseek(in_file, fp, SEEK_SET);
  double bw_1=0;
  fread(&bw_1, sizeof(double), 1, in_file);
  printf("Bandwidth of each channel, %f MHz.\n", bw_1);
  fp=fp+8;
  
  //DM
  fseek(in_file, fp, SEEK_SET);
  double dm=0;
  fread(&dm, sizeof(double), 1, in_file);
  printf("The DM is %.5f.\n", dm);
  fp=fp+8;
  
  fseek(in_file, fp, SEEK_SET);
  //double unknown=0;
  fread(&unknown, sizeof(double), 1, in_file);
  printf("Unknown value, %f.\n", unknown);
  fp=fp+8;
  
  //period, in second, topocentric
  fseek(in_file, fp, SEEK_SET);
  double p0_topo=0;
  fread(&p0_topo, sizeof(double), 1, in_file);
  printf("The topocentric period is %.12f millisecond.\n", p0_topo*1000.0);
  fp=fp+8;
  
  //p1, in second per second, topocentric
  fseek(in_file, fp, SEEK_SET);
  double p1_topo=0;
  fread(&p1_topo, sizeof(double), 1, in_file);
  printf("The topocentric p-dot is %e second per second.\n", p1_topo);
  fp=fp+8;
  
  //may be p2?
  fseek(in_file, fp, SEEK_SET);
  //double unknown=0;
  fread(&unknown, sizeof(double), 1, in_file);
  printf("Unknown value, %e.\n", unknown);
  fp=fp+8;
  
  //may be p3?
  fseek(in_file, fp, SEEK_SET);
  //double unknown=0;
  fread(&unknown, sizeof(double), 1, in_file);
  printf("Unknown value, %e.\n", unknown);
  fp=fp+8;
  
  //period, in second, barycentric
  fseek(in_file, fp, SEEK_SET);
  double p0_bary=0;
  fread(&p0_bary, sizeof(double), 1, in_file);
  printf("The barycentric period is %.12f millisecond.\n", p0_bary*1000.0);
  fp=fp+8;
  
  //p1, in second per second, barycentric
  fseek(in_file, fp, SEEK_SET);
  double p1_bary=0;
  fread(&p1_bary, sizeof(double), 1, in_file);
  printf("The barycentric p-dot is %e second per second.\n", p1_bary);
  fp=fp+8;
  
  //may be p2?
  fseek(in_file, fp, SEEK_SET);
  //double unknown=0;
  fread(&unknown, sizeof(double), 1, in_file);
  printf("Unknown value, %e.\n", unknown);
  fp=fp+8;
  
  //may be p3?
  fseek(in_file, fp, SEEK_SET);
  //double unknown=0;
  fread(&unknown, sizeof(double), 1, in_file);
  printf("Unknown value, %e.\n", unknown);
  fp=fp+8;
  
  //f0
  fseek(in_file, fp, SEEK_SET);
  double f0=0;
  fread(&f0, sizeof(double), 1, in_file);
  printf("The frequency f0 is %.10f.\n", f0);
  fp=fp+8;
  
  //f1
  fseek(in_file, fp, SEEK_SET);
  double f1=0;
  fread(&f1, sizeof(double), 1, in_file);
  printf("The f1 is %.10e.\n", f1);
  fp=fp+8;
  
  //f2
  fseek(in_file, fp, SEEK_SET);
  double f2=0;
  fread(&f2, sizeof(double), 1, in_file);
  printf("The f2 is %.10e\n", 2);
  fp=fp+8;

  fseek(in_file, fp, SEEK_SET);
  //double unknown=0;
  fread(&unknown, sizeof(double), 1, in_file);
  printf("Unknown value, %e in 0x%X.\n", unknown, fp);
  fp=fp+8;
  
  fseek(in_file, fp, SEEK_SET);
  //double unknown=0;
  fread(&unknown, sizeof(double), 1, in_file);
  printf("Unknown value, %e in 0x%X.\n", unknown, fp);
  fp=fp+8;
  
  fseek(in_file, fp, SEEK_SET);
  //double unknown=0;
  fread(&unknown, sizeof(double), 1, in_file);
  printf("Unknown value, %e in 0x%X.\n", unknown, fp);
  fp=fp+8;
  
  fseek(in_file, fp, SEEK_SET);
  //double unknown=0;
  fread(&unknown, sizeof(double), 1, in_file);
  printf("Unknown value, %e in 0x%X.\n", unknown, fp);
  fp=fp+8;
  
  fseek(in_file, fp, SEEK_SET);
  //double unknown=0;
  fread(&unknown, sizeof(double), 1, in_file);
  printf("Unknown value, %e in 0x%X.\n", unknown, fp);
  fp=fp+8;
  
  fseek(in_file, fp, SEEK_SET);
  //double unknown=0;
  fread(&unknown, sizeof(double), 1, in_file);
  printf("Unknown value, %e in 0x%X.\n", unknown, fp);
  fp=fp+8;
  
  fseek(in_file, fp, SEEK_SET);
  //double unknown=0;
  fread(&unknown, sizeof(double), 1, in_file);
  printf("Unknown value, %e in 0x%X.\n", unknown, fp);
  fp=fp+8;
  printf("\n");

  //axis one
  fseek(in_file, fp, SEEK_SET);
  double axis_1_min=0, axis_1_max=0;
  fread(&axis_1_min, sizeof(double), 1, in_file);
  fp=fp+8*(header_info[0]-1);
  fseek(in_file, fp, SEEK_SET);
  fread(&axis_1_max, sizeof(double), 1, in_file);
  fp=fp+8;
  printf("DM vs x2 plot, X axis, DM range, start from %.2f, end at %.2f, %d steps.\n", axis_1_min, axis_1_max, header_info[0]);

  //axis two
  fseek(in_file, fp, SEEK_SET);
  double axis_2_min=0, axis_2_max=0;
  fread(&axis_2_min, sizeof(double), 1, in_file);
  fp=fp+8*(header_info[1]-1);
  fseek(in_file, fp, SEEK_SET);
  fread(&axis_2_max, sizeof(double), 1, in_file);
  fp=fp+8;
  printf("P1 vs p0 plot, X axis, p0 range, start from %.12f millisecond, end at %.12f millisecond, %d steps.\n", axis_2_min*1000.0, axis_2_max*1000.0, header_info[1]);

  //axis three
  fseek(in_file, fp, SEEK_SET);
  double axis_3_min=0, axis_3_max=0;
  fread(&axis_3_min, sizeof(double), 1, in_file);
  fp=fp+8*(header_info[2]-1);
  fseek(in_file, fp, SEEK_SET);
  fread(&axis_3_max, sizeof(double), 1, in_file);
  fp=fp+8;
  printf("P1 vs p0 plot, Y axis, p1 range, start from %.5e second per second, end at %.5e second per second, %d steps.\n", axis_3_min, axis_3_max, header_info[2]);
  printf("\n");

/*
  //axis four
  fseek(in_file, fp, SEEK_SET);
  double axis_4_min=0, axis_4_max=0;
  fread(&axis_4_min, sizeof(double), 1, in_file);
  fp=fp+8*(header_info[5]-1);
  fseek(in_file, fp, SEEK_SET);
  fread(&axis_4_max, sizeof(double), 1, in_file);
  fp=fp+8;
  printf("Axis four, start from %e, end at %e, %d steps.\n", axis_4_min, axis_4_max, header_info[5]);
*/

  if((out_file=fopen("array1.txt", "wb"))==NULL){
    printf("Creating file array1.txt error, please check the disk.\n");
    return 1;
  }

//header_info[3]*header_info[4]*header_info[5]
  long int block1_length=header_info[3]*header_info[4]*header_info[5];
  for(i=1;i<=block1_length;i++){
//  for(i=1;i<=811008;i++){
//  for(i=1;i<=6500;i++){
//  for(i=1;i<=393153;i++){
    fseek(in_file, fp, SEEK_SET);
    //double unknown=0;
    fread(&unknown, sizeof(double), 1, in_file);
    //printf("Value, index %d, %.5f, position 0x%X\n", i, unknown, fp);
    fprintf(out_file, "%.5f\n", unknown);
    fp=fp+8;
  }
  printf("Here is a %d double size values.\n", i-1);
  fclose(out_file);

  if((out_file=fopen("array2.txt", "wb"))==NULL){
    printf("Creating file array2.txt error, please check the disk.\n");
    return 1;
  }

//According to the notes in prepfold.py, 
//this is a foldstats structure.
//It is a group of 7 doubles.
//These 7 numbers are: numdata, data_avg, data_var, num_prof, prof_avg, prof_var, reduced-chi-square
//  for(i=1;i<=172032;i++){
//  for(i=1;i<=100;i++){
  long int block2_length=header_info[3]*header_info[4]*7;
  for(i=1;i<=block2_length;i++){
    fseek(in_file, fp, SEEK_SET);
    //double unknown=0;
    fread(&unknown, sizeof(double), 1, in_file);
//    printf("Value, index %d, %.12e, position 0x%X\n", i, unknown, fp);
    fprintf(out_file, "%.5f\n", unknown);
    fp=fp+8;
  }
  printf("Here is a %d double size values.\n", i-1);
  fclose(out_file);




  return 0;
}
