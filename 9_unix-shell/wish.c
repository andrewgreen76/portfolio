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


int sh_mode = 0; // 0 - interactive , 1 - batch
int req_exit = 0;
FILE * fptr;
void interact();
void proc_batch(char *);
void eat_cl();


int main(int argc, char *argv[]) {
  printf("\n");
  assert(argc<=2); // argv[1] = batch-file name

  if(argc==1){
    sh_mode = 0;
    interact();
  }
  else{
    sh_mode = 1;
    proc_batch(argv[1]);
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

// ===============================================
// ============= INTERACTIVE MODE : ==============
// ===============================================
void interact(){
  while(!req_exit){
    printf("wish> ");
    eat_cl();
  }
}

// ===============================================
// ================ BATCH MODE : =================
// ===============================================
void proc_batch(char * fname){
  fptr = fopen(fname , "r");
  
  if(fptr){
    while(!req_exit) eat_cl();
    fclose(fptr);
  }
  else{
    //... do nothing ... 
  }
}


// ===============================================
void eat_cl(){
  char ln[100];
  void * ln_state;
  
  if(sh_mode) { // from batch file
    ln_state = (void *) fgets(ln, sizeof(ln), fptr); 
  }
  else {        // from shell
    ln_state = (void *) fgets(ln, sizeof(ln), stdin); // read shell line 
    if(ln_state) {                  // if not enter blank 
      sscanf(cl , "%s" , cl);       // extracts the 1st word
    }
  }

  
  
  
  if( strcmp(shln , "exit")==0 ){
    req_exit = 1;
  }
  //strcmp(cl,"exit");
}

