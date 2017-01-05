#include <stdio.h>
//to create a script for copying all the files from PRESTO prepfold from sub-directories
int main(int argc, char * argv[]){
  int foldernum=0;
  sscanf(argv[1],"%d",&foldernum);
  //printf("%d\n",foldernum);
  FILE *outfile;
  if((outfile=fopen("CpCand.bash","w"))==NULL){
    printf("\nI can't create output file!\n");
    return 1;
  }
  int i=0;
  fprintf(outfile, "echo Copying %d folding results.\n", foldernum);
  for(i=1;i<=foldernum;i++){
    //cd (num)
    //cp *.ps ..
    //cd ..
    //rm -rdf (num)
    fprintf(outfile,"cd %d\n",i);
    fprintf(outfile,"mv *.ps ..\n");
    fprintf(outfile,"mv *.pfd ..\n");
    fprintf(outfile,"cd ..\n");
    fprintf(outfile,"rm -rdf %d\n\n",i);
  }
  fprintf(outfile, "echo Copy finished.\n");
  fclose(outfile);
  return 0;
}

