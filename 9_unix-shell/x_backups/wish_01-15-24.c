#include "wish.h"
#include <ctype.h>  // isspace
#include <regex.h>  // regcomp, regexec, regfree
#include <stdio.h>  // fopen, fclose, fileno, getline, feof
#include <stdlib.h> // exit
#include <sys/types.h>
#include <sys/wait.h> // waitpid
#include <string.h> 
#include <assert.h> 


int sh_mode = INTERACTIVE_MODE; // 1 - interactive , 2 - batch
FILE * fptr;
void interact();
void proc_batch(char *);
void eat_cl();
void digest_cl(char * );


int main(int argc, char *argv[]) {
  printf("\n");
  assert(argc<=2); // argv[1] = batch-file name

  if(argc==1){
    sh_mode = INTERACTIVE_MODE;
    interact();
  }
  else{
    sh_mode = BATCH_MODE;
    proc_batch(argv[1]);
  }
/*
    fork/wait/execv(cmd) 

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
  while(1){
    printf("wish> ");
    eat_cl();
  }
}

// ===============================================
// ================ BATCH MODE : =================
// ===============================================
void proc_batch(char * fname){
  fptr = fopen(fname , "r");
  
  if(fptr){             // if found, 
    while(1) eat_cl();  // scan the batch 
    fclose(fptr);
  }
}


// ===============================================
void eat_cl(){
  char * cl = NULL;
  size_t len = 0;
  int ln_state = 0;
          
  if(sh_mode==1) ln_state = getline( &cl , &len , stdin ); // read shell line   
  else ln_state = getline( &cl , &len , fptr);           // read batch line
  
  if(ln_state == -1) { // exit prog if INT/EOF 
    exit(0); 
  }
  else {              // in the middle of punching in commands 
    digest_cl(cl);
  }

  free(cl);
}

void digest_cl(char * cl_rem){
  char * token;
  const char delim = " ";

  while ( (token = strsep(&cl_rem, delim)) != NULL) {
    //printf("Token: %s\n", token);
  }
  
    /*
  if( strcmp(cl , "exit")==0 ){
    req_exit = 1;
    }
  */
  //strcmp(cl,"exit");
}
