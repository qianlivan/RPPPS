#include <stdio.h>
#include <string.h>
#include <math.h>
#include <malloc.h>
#include <stdlib.h>

//to read the 32 bits little_endian int from the file, written by hkuclion
//another function is fgetw(FILE *fp) maybe have the same operation
int read_int(FILE *file, int offset){
	int int_1=0;
	fseek(file,offset,0);
	int_1=fgetc(file)<<0 | fgetc(file)<<8 | fgetc(file)<<16 | fgetc(file)<<24;
	return int_1;
}

//to calculate the beam size, return value is in the unit of areminite
//input frequency is in the unit of MHz
float fast_beamsize(float freq){
  return (3.0/(freq/100.0))/300.0*180.0/3.1416*60;
}

//start the process
int main(int argc, char *argv[]){

  //show some welcome information :-)
  printf("\nFAST-dirft-pulsar-search data processing tools.\n");
  printf("Cutting one filterbank datafile to beams.\n");
  printf("Last modified: 20160819\n\n");

  //define input and output files
  FILE *in_fil,*out_fil;
  int i=0, j=0;

  //if there is no input parameter, just show the help infomation:
  if(!(argc>1)){
    printf("Usage: filterbank_cut (filterbank file)\n");
    printf("Will automatically cut filterbank files as FAST drift scan size.\n");
    return 0;
  }

  //if with more than one input file, show the warning:
  if (!(argc>=2)){
    printf("WARNING: ONLY process the first file!!!\n");
  }

  //if with input file, open it
  if((in_fil=fopen(argv[1],"rb"))==NULL){
    printf("The file %s do not exist!\n",argv[1]);
    return 1;
  }

  //read header information:
  int in_fil_offset=0;
  int hd_start_length=read_int(in_fil,in_fil_offset);
  //check if this is a filterbank datat (start with a 32 bit integer 0C 00 00 00 and HEADER_START)
  if(hd_start_length!=0x0c){
    printf("The input file %s is not a filterbank data!\n", argv[1]);
    fclose(in_fil);
    return 1;
  }
  printf("Header check Ok, start value is %d, correct.\n\n",hd_start_length);

  //let's read and shwo the information in the header:
  printf("The information of this file %s: \n", argv[1]);

  //source filename:
  //set the offset
  in_fil_offset=in_fil_offset+4+hd_start_length;
  //move the point
  fseek(in_fil,in_fil_offset, SEEK_SET);
  //read the length
  int src_string_length=read_int(in_fil, in_fil_offset);
  //read the filename
  char src_filename[50]={'\0'};
  //fread(src_filename, src_file_length, 1, in_fil);
  //printf("Converted from: %d, %s\n\n", src_file_length, src_filename);
  //read the filename length
  in_fil_offset=in_fil_offset+4+src_string_length;
  fseek(in_fil,in_fil_offset, SEEK_SET);
  int src_filename_length=read_int(in_fil, in_fil_offset);
  fread(src_filename, src_filename_length, 1, in_fil);
  printf("Converted from: %s\n", src_filename);

  //read the source name
  in_fil_offset=in_fil_offset+4+src_filename_length;
  fseek(in_fil, in_fil_offset, SEEK_SET);
  int src_length=read_int(in_fil, in_fil_offset);
  char src_name[50]={'\0'};
  //fread(src_name, src_length, 1, in_fil);
  in_fil_offset=in_fil_offset+4+src_length;
  fseek(in_fil, in_fil_offset, SEEK_SET);
  int src_name_length=read_int(in_fil, in_fil_offset);
  fread(src_name, src_name_length, 1, in_fil);
  printf("Source name: %s\n", src_name);

  //read the machine ID
  in_fil_offset=in_fil_offset+4+src_name_length;
  //fseek(in_fil, in_fil_offset, SEEK_SET);
  int machineID_length=read_int(in_fil, in_fil_offset);
  unsigned int machineID=0;
  //fread(src_name, src_length, 1, in_fil);
  in_fil_offset=in_fil_offset+4+machineID_length;
  fseek(in_fil, in_fil_offset, SEEK_SET);
  machineID=read_int(in_fil, in_fil_offset);
  //fread(src_name, src_length, 1, in_fil);
  printf("Machine ID: 0x%08X\n", machineID);

  //read the telescope ID
  in_fil_offset=in_fil_offset+4;
  //fseek(in_fil, in_fil_offset, SEEK_SET);
  int telescopeID_length=read_int(in_fil, in_fil_offset);
  unsigned int telescopeID=0;
  //fread(src_name, src_length, 1, in_fil);
  in_fil_offset=in_fil_offset+4+telescopeID_length;
  fseek(in_fil, in_fil_offset, SEEK_SET);
  telescopeID=read_int(in_fil, in_fil_offset);
  //fread(src_name, src_length, 1, in_fil);
  printf("Telescope ID: %d\n", telescopeID);

  //read the source RA J2000
  in_fil_offset=in_fil_offset+4;
  //fseek(in_fil, in_fil_offset, SEEK_SET);
  int raj_length=read_int(in_fil, in_fil_offset);
  int raj1, raj2;
  double raj=0.0;
  //fread(src_name, src_length, 1, in_fil);
  in_fil_offset=in_fil_offset+4+raj_length;
  fseek(in_fil, in_fil_offset, SEEK_SET);
  fread(&raj, 8, 1, in_fil);
  //for(i=0;i<8;i++){
  //fscanf(in_fil, "%16x", &raj);
  //raj=fgetc(in_fil)<<0 | fgetc(in_fil)<<8 | fgetc(in_fil)<<16 | fgetc(in_fil)<<24 | fgetc(in_fil)<<32 | fgetc(in_fil)<<40 | fgetc(in_fil)<<48 | fgetc(in_fil)<<56;
  //raj1=read_int(in_fil, in_fil_offset);
  //raj2=read_int(in_fil, in_fil_offset+4);
  //  raj=raj<8;
  //  fseek(in_fil, 2, SEEK_CUR);
  //}
  //raj=raj>>8;
  //fread(src_name, src_length, 1, in_fil);
  int ra_hour=int(raj/10000.0);
  int ra_min=int((raj-ra_hour*10000)/100.0);
  float ra_second=raj-ra_hour*10000.0-ra_min*100.0;
  printf("RA  J2000: %02dh%02dm%02.2fs\n", ra_hour, ra_min, ra_second);

  //read the source DEC J2000
  in_fil_offset=in_fil_offset+8;
  int decj_length=read_int(in_fil,in_fil_offset);
  in_fil_offset=in_fil_offset+4+decj_length;
  //int decj1, decj2;
  //decj1=read_int(in_fil,in_fil_offset);
  //decj2=read_int(in_fil,in_fil_offset+4);
  double decj=0.0;
  fseek(in_fil, in_fil_offset, SEEK_SET);
  fread(&decj, 8, 1, in_fil);
  int dec_deg=int(decj/10000.0);
  int dec_min=int((decj-dec_deg*10000.0)/100.0);
  float dec_second=decj-dec_deg*10000.0-dec_min*100.0;
  if(dec_min<0) dec_min=dec_min*(-1);
  if(dec_second<0) dec_second=dec_second*(-1);
  printf("DEC J2000: %02dd%02dm%02.2fs\n", dec_deg, dec_min, dec_second);

  //read the az_start
  in_fil_offset=in_fil_offset+8;
  int az_start_length=read_int(in_fil,in_fil_offset);
  in_fil_offset=in_fil_offset+4+az_start_length;
  //int az_start1, az_start2;
  //az_start1=read_int(in_fil,in_fil_offset);
  //az_start2=read_int(in_fil,in_fil_offset+4);
  double az_start=0.0;
  fseek(in_fil, in_fil_offset, SEEK_SET);
  fread(&az_start, 8, 1, in_fil);
  printf("AZ start: %.5f\n", az_start);

  //read the za_start
  in_fil_offset=in_fil_offset+8;
  int za_start_length=read_int(in_fil,in_fil_offset);
  in_fil_offset=in_fil_offset+4+za_start_length;
  //int za_start1, za_start2;
  //za_start1=read_int(in_fil,in_fil_offset);
  //za_start2=read_int(in_fil,in_fil_offset+4);
  double za_start=0.0;
  fseek(in_fil, in_fil_offset, SEEK_SET);
  fread(&za_start, 8, 1, in_fil);
  printf("ZA start: %.5f\n", za_start);

  //read the data type
  in_fil_offset=in_fil_offset+8;
  int data_type_length=read_int(in_fil,in_fil_offset);
  in_fil_offset=in_fil_offset+4+data_type_length;
  int data_type;
  data_type=read_int(in_fil,in_fil_offset);
  //decj2=read_int(in_fil,in_fil_offset+4);
  printf("Data type: %d\n", data_type);

  //read the fch1
  in_fil_offset=in_fil_offset+4;
  int fch1_length=read_int(in_fil,in_fil_offset);
  in_fil_offset=in_fil_offset+4+fch1_length;
  //int fch11, fch12;
  //fch11=read_int(in_fil,in_fil_offset);
  //fch12=read_int(in_fil,in_fil_offset+4);
  double fch1=0.0;
  fseek(in_fil, in_fil_offset, SEEK_SET);
  fread(&fch1, 8, 1, in_fil);
  printf("Frequency of channel 1 (MHz): %.3f\n", fch1);

  //read the foff
  in_fil_offset=in_fil_offset+8;
  int foff_length=read_int(in_fil,in_fil_offset);
  in_fil_offset=in_fil_offset+4+foff_length;
  //int foff1, foff2;
  //foff1=read_int(in_fil,in_fil_offset);
  //foff2=read_int(in_fil,in_fil_offset+4);
  double foff=0.0;
  fseek(in_fil, in_fil_offset, SEEK_SET);
  fread(&foff, 8, 1, in_fil);
  printf("Frequency offset of each channel (MHz): %.3f\n", foff);

  //read the channel number
  in_fil_offset=in_fil_offset+8;
  int nchans_length=read_int(in_fil,in_fil_offset);
  in_fil_offset=in_fil_offset+4+nchans_length;
  int nchans;
  nchans=read_int(in_fil,in_fil_offset);
  //decj2=read_int(in_fil,in_fil_offset+4);
  printf("Number of channels: %d\n", nchans);

  //read the beam number
  in_fil_offset=in_fil_offset+4;
  int nbeam_length=read_int(in_fil,in_fil_offset);
  in_fil_offset=in_fil_offset+4+nbeam_length;
  int nbeam;
  nbeam=read_int(in_fil,in_fil_offset);
  //decj2=read_int(in_fil,in_fil_offset+4);
  printf("Number of the beam: %d\n", nbeam);

  //read the ibeam
  in_fil_offset=in_fil_offset+4;
  int ibeam_length=read_int(in_fil,in_fil_offset);
  in_fil_offset=in_fil_offset+4+ibeam_length;
  int ibeam;
  ibeam=read_int(in_fil,in_fil_offset);
  //decj2=read_int(in_fil,in_fil_offset+4);
  printf("ibeam: %d\n", ibeam);

  //read the number of sampling bits
  in_fil_offset=in_fil_offset+4;
  int nbits_length=read_int(in_fil,in_fil_offset);
  in_fil_offset=in_fil_offset+4+nbits_length;
  int nbits;
  nbits=read_int(in_fil,in_fil_offset);
  //decj2=read_int(in_fil,in_fil_offset+4);
  printf("Sampling bits: %d\n", nbits);

  //read the start time
  in_fil_offset=in_fil_offset+4;
  int tstart_length=read_int(in_fil,in_fil_offset);
  in_fil_offset=in_fil_offset+4+tstart_length;
  //int tstart1, tstart2;
  //tstart1=read_int(in_fil,in_fil_offset);
  //tstart2=read_int(in_fil,in_fil_offset+4);
  double tstart=0.0;
  fseek(in_fil, in_fil_offset, SEEK_SET);
  fread(&tstart, 8, 1, in_fil);
  printf("Start time (MJD): %.10f\n", tstart);

  //read the sampling time
  in_fil_offset=in_fil_offset+8;
  int tsamp_length=read_int(in_fil,in_fil_offset);
  in_fil_offset=in_fil_offset+4+tsamp_length;
  //int tsamp1, tsamp2;
  //tsamp1=read_int(in_fil,in_fil_offset);
  //tsamp2=read_int(in_fil,in_fil_offset+4);
  double tsamp=0.0;
  fseek(in_fil, in_fil_offset, SEEK_SET);
  fread(&tsamp, 8, 1, in_fil);
  printf("Sampling time (us): %.3f\n", tsamp*1000.0*1000.0);

  //read the nifs
  in_fil_offset=in_fil_offset+8;
  int nifs_length=read_int(in_fil,in_fil_offset);
  in_fil_offset=in_fil_offset+4+nifs_length;
  int nifs;
  nifs=read_int(in_fil,in_fil_offset);
  //decj2=read_int(in_fil,in_fil_offset+4);
  printf("nifs: %d\n", nifs);

  //read the HEADER_END
  in_fil_offset=in_fil_offset+4;
  int hd_end_length=read_int(in_fil,in_fil_offset);
  in_fil_offset=in_fil_offset+4+hd_end_length;
  //int decj1, decj2;
  //decj1=read_int(in_fil,in_fil_offset);
  //decj2=read_int(in_fil,in_fil_offset+4);
  //printf("DEC J2000: %016x, %016x\n", decj1, decj2);

  //calculate the lenght of this file
  int data_offset=in_fil_offset;
  fseek(in_fil, 0, SEEK_END);
  int data_length=ftell(in_fil)-data_offset;
  printf("Data section length: 0x%X\n", data_length);
  int samples=data_length/nchans*8/nbits;
  printf("Samples: %d\n", samples);
  float int_time=data_length/nchans*8/nbits*tsamp;
  printf("Intergration time (second): %f\n", int_time);

  //start cutting......
  printf("\nStart to cut the file %s into beams......\n\n", argv[1]);
  //how many files will be created:
  //the frequencies of seach channel:
  float freq_channels[nchans];
  for(i=0;i<nchans;i++){
    freq_channels[i]=fch1+i*foff;
  }
  //the highest and lowest frequencies are:
  float freq_low=freq_channels[0], freq_high=freq_channels[nchans-1], freq_tmp=0;;
  if(freq_low>freq_high){
    freq_tmp=freq_low;
    freq_low=freq_high;
    freq_high=freq_tmp;
  }
  printf("The lowest frequency is (MHz): %f (beam size %.2f arcminite).\n", freq_low, fast_beamsize(freq_low));
  printf("The highest frequency is (MHz): %f (beam size %.2f arcminite).\n", freq_high, fast_beamsize(freq_high));

  float beam_separation=1.0/3.0, beam_cut=fast_beamsize(freq_low);
  float rotation_earth=15.0;//15 arcsecond per second
  float beam_cut_time=beam_cut*60.0/rotation_earth;
  int files_output=int(int_time/(beam_cut_time*beam_separation))-int(1.0/beam_separation+1)+1;
  printf("Will create %d files, the lenght of each file is %.3f seconds, beam separtaion is %.2f.\n\n", files_output, beam_cut_time, beam_separation);
  
  //loop for cutting
  char out_fil_name[50]={'\0'};
  int cut_data_length=int((beam_cut_time/tsamp)*nchans*nbits/8.0)+1;//in byte
  printf("Cut %d byte data.\n", cut_data_length);
  char *cut_data;
  cut_data=(char*)malloc(cut_data_length);
  double tstart_offset=0.0;
  //files_output=5;
  char ch;
  for(i=1;i<=files_output;i++){
    sscanf(argv[1], "%s", &out_fil_name);
    sprintf(out_fil_name, "%s_%d.fil", out_fil_name, i);
    printf("Creating file %s, %d in %d.\n",out_fil_name, i, files_output);
    //open the file
    if((out_fil=fopen(out_fil_name, "wb"))==NULL){
      printf("Creating file %s error, please check the disk.\n", out_fil_name);
      return 1;
    }

    //the data offset
    int in_fil_offset_data=in_fil_offset+(i-1)*int((cut_data_length*beam_separation));
    double time_offset=(tsamp*(in_fil_offset_data-in_fil_offset)*8/nbits/nchans)/3600.0/24.0;//in days
    //double raj_offset_arcsecond=time_offset*rotation_earth;//in arcsecond
    double ra_hour_offset=int(time_offset*24.0);
    ra_hour=ra_hour+ra_hour_offset;
    double ra_min_offset=int((time_offset*24.0-ra_hour_offset*24.0)*60);
    ra_min=ra_min+ra_min_offset;
    double ra_second_offset=time_offset*24.0*60.0*60.0-ra_hour_offset*24.0*60.0*60.0-ra_min_offset*60.0;
    ra_second=ra_second+ra_second_offset;
    double raj_offset=ra_hour*10000+ra_min*100+ra_second;
    //printf("%.10f, %f\n", time_offset, raj_offset);

    //write header
    fwrite(&hd_start_length, 4, 1, out_fil);
    fprintf(out_fil, "HEADER_START");
    
    fwrite(&src_string_length, 4, 1, out_fil);
    fprintf(out_fil, "rawdatafile");
    //printf("%d\n",src_length);
    
    fwrite(&src_filename_length, 4, 1, out_fil);
    fprintf(out_fil, "%s", src_filename);

    fwrite(&src_length, 4, 1, out_fil);
    fprintf(out_fil, "source_name");

    fwrite(&src_name_length, 4, 1, out_fil);
    fprintf(out_fil, "%s", src_name);

    fwrite(&machineID_length, 4, 1, out_fil);
    fprintf(out_fil, "machine_id");
    fwrite(&machineID, 4, 1, out_fil);
    
    fwrite(&telescopeID_length, 4, 1, out_fil);
    fprintf(out_fil, "telescope_id");
    fwrite(&telescopeID, 4, 1, out_fil);

    fwrite(&raj_length, 4, 1, out_fil);
    
    fprintf(out_fil, "src_raj");
    fwrite(&raj_offset, 8, 1, out_fil);

    fwrite(&decj_length, 4, 1, out_fil);
    fprintf(out_fil, "src_dej");
    fwrite(&decj, 8, 1, out_fil);

    fwrite(&az_start_length, 4, 1, out_fil);
    fprintf(out_fil, "az_start");
    fwrite(&az_start, 8, 1, out_fil);

    fwrite(&za_start_length, 4, 1, out_fil);
    fprintf(out_fil, "za_start");
    fwrite(&za_start, 8, 1, out_fil);

    fwrite(&data_type_length, 4, 1, out_fil);
    fprintf(out_fil, "data_type");
    fwrite(&data_type, 4, 1, out_fil);

    fwrite(&fch1_length, 4, 1, out_fil);
    fprintf(out_fil, "fch1");
    fwrite(&fch1, 8, 1, out_fil);

    fwrite(&foff_length, 4, 1, out_fil);
    fprintf(out_fil, "foff");
    fwrite(&foff, 8, 1, out_fil);

    fwrite(&nchans_length, 4, 1, out_fil);
    fprintf(out_fil, "nchans");
    fwrite(&nchans, 4, 1, out_fil);

    fwrite(&nbeam_length, 4, 1, out_fil);
    fprintf(out_fil, "nbeams");
    fwrite(&nbeam, 4, 1, out_fil);

    fwrite(&ibeam_length, 4, 1, out_fil);
    fprintf(out_fil, "ibeam");
    fwrite(&ibeam, 4, 1, out_fil);

    fwrite(&nbits_length, 4, 1, out_fil);
    fprintf(out_fil, "nbits");
    fwrite(&nbits, 4, 1, out_fil);

    fwrite(&tstart_length, 4, 1, out_fil);
    tstart_offset=tstart+time_offset;
    fprintf(out_fil, "tstart");
    fwrite(&tstart_offset, 8, 1, out_fil);

    fwrite(&tsamp_length, 4, 1, out_fil);
    fprintf(out_fil, "tsamp");
    fwrite(&tsamp, 8, 1, out_fil);

    fwrite(&nifs_length, 4, 1, out_fil);
    fprintf(out_fil, "nifs");
    fwrite(&nifs, 4, 1, out_fil);

    fwrite(&hd_end_length, 4, 1, out_fil);
    fprintf(out_fil, "HEADER_END");
    //fwrite(&, 8, 1, out_fil);

    //write data
    fseek(in_fil, in_fil_offset_data, SEEK_SET);
    //to save the memory, we use:
    for(j=0;j<cut_data_length;j++){
      ch=fgetc(in_fil);
      fputc(ch, out_fil);
    }
    //fread(&cut_data, cut_data_length, 1, in_fil);
    //fwrite(&cut_data, cut_data_length, 1, out_fil);

    //close the file
    fclose(out_fil);
  }

  
  //finally, close the input and output file
  fclose(in_fil);
  //fclose(out_fil);

  printf("\n");
  return 0;
}






