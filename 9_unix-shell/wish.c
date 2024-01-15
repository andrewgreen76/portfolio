//#include "wish.h"
#include <ctype.h>  // isspace
#include <regex.h>  // regcomp, regexec, regfree
#include <stdio.h>  // fopen, fclose, fileno, getline, feof
#include <stdlib.h> // exit
#include <sys/types.h>
#include <sys/wait.h> // waitpid
//
#include <assert.h>  
#include <string.h>  


int mode = 0; // 0 - interactive , 1 - batch


int main(int argc, char *argv[]) {
  char cl[100];
  printf("\n");
  assert(argc<=2); // argv[1] = batch-file name

  while(1){
    printf("wish> ");
    fgets(cl, sizeof(cl), stdin); // captures the line
    sscanf(cl , "%s" , cl);       // extracts the 1st word 
    if( !strcmp(cl,"exit") ) break;
  }
  
/*
........
    fork/wait/exec(cmd) 

/////////// CHILD ///////////
 -> shell response	  
    -> execute the command 
    -> child finishes
 -> parent resumes , prompts again 
*/
  
  printf("\n");
  
  return 0;
}
