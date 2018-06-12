#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){

  char* filename = argv[1];
  FILE* fp;
  char buffr[255];
  printf("%s\n", "Bot attack with such parametrs: ");
  fp = fopen(filename, "r");
  while(fgets(buffr, 255, (FILE*) fp)) {
      printf("%s", buffr);
  }

  fclose(fp);
  return 0;
}
