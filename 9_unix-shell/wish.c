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
int eat_shln();
void * eat_batln();


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
    eat_shln();
  }
}

// ===============================================
void eat_shln(){
  char shln[100];
  void * shln_state;
  
  shln_state = (void *) fgets(shln, sizeof(shln), stdin); // captures cmdln from shell 
  if(shln_state) sscanf(shln , "%s" , shln);       // extracts the 1st word  
  
  if( strcmp(shln , "exit")==0 ){
    req_exit = 1;
  }
}
// ===============================================
// ================ BATCH MODE : =================
// ===============================================
void proc_batch(char * fname){
  fptr = fopen(fname , "r");
  
  if(fptr){
    while( eat_batln(fptr) != NULL );
    fclose(fptr);
  }
  else{
    //... do nothing ... 
  }
}

// ===============================================
void * eat_batln(){
  char batln[100];
  void * batln_state;
  
  batln_state = fgets(cl, sizeof(cl), fptr); // captures cmdln from shell
  if(batln_state) sscanf(cl , "%s" , cl);       // extracts the 1st word
    
  //strcmp(cl,"exit");
  return batln_state;
}
