#define  _GNU_SOURCE
//#define  _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc , char *argv[]){
  char * line = NULL;
  size_t len = 0;

  while(getline(&line , &len , stdin) != -1) {
    printf(": %s" , line);
  }
  free(line);

/*
  char * line = NULL;
  size_t len = 0;
  char a[] = "hello";
  char * rest = a;
  const char * delimiter = " ";

  getline(&line , &len , stdin);
  free(line);

  strsep(&rest , delimiter);
*/
}
