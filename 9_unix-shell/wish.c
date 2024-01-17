#define  _GNU_SOURCE
//#define  _POSIX_C_SOURCE 200809L
#include "wish.h"
#include <ctype.h>  // isspace
#include <regex.h>  // regcomp, regexec, regfree
#include <stdio.h>  // fopen, fclose, fileno, getline, feof
#include <stdlib.h> // exit
#include <sys/types.h>
#include <sys/wait.h> // waitpid
#include <string.h> 
#include <assert.h> 

// SHELL GETS LAUNCHED ONCE , IN ONE MODE. 
int sh_mode = INTERACTIVE_MODE; // 1 - interactive , 2 - batch
FILE * fptr;
char cl[BUFF_SIZE];
void interact();
void use_batch(char *);
void proc_ln();
void check_INT_EOF(char *);
void get_cmds(char * [] , int * , char *);

/*  
    fork/wait/ EXECV(cmd) 
       -> execute the command 
       -> child finishes
    -> parent resumes , prompts again 
*/

// ===============================================================
// =========================== MAIN : ============================
// ===============================================================
int main(int argc, char *argv[]) {
  if(argc>2){ 
    printError();
    exit(1);
  }
  // ============== Enter a mode , engage : ===============
  sh_mode = argc;
  if(sh_mode == INTERACTIVE_MODE) interact();
  else use_batch(argv[1]);
  /////////////////////////////////////////////////////////
  printf("\n");
  return 0;
}

// ===============================================
// ============= INTERACTIVE MODE : ==============
// ===============================================
void interact(){
  while(1){
    printf("wish> ");
    proc_ln();
  }
}

// ===============================================
// ================ BATCH MODE : =================
// ===============================================
void use_batch(char * fname){
  fptr = fopen(fname , "r");
  
  if(fptr){       // if found, 
    while(1) {    //
      proc_ln();  // scan the batch
    }
    fclose(fptr);
  }
}

// ====================================================
// Process command : check for blanks, else pass thru :
// ====================================================
void proc_ln() {
  char * ln = NULL;
  char *cmds[BUFF_SIZE]; // 256 ptrs 
  int cmds_num = 0;
  
  check_INT_EOF(ln);                // check if end 
  if( strcmp(ln,"") ) {             // check if blank line
    get_cmds(cmds , &cmds_num , ln); // break up into commands : return addresses of cmds , num of cmds
    //executeCommands(cmds , cmds_num , fptr); // per CMDLN or CMD ??
  }
  free(ln);
}

// =================================================
// ===== Breaks the line up into indiv. cmds : =====
// =================================================
void get_cmds(char * cmds[] , int * cmds_num, char * cl_rem){  
  const char * delim = "&";
  
  while ( (cmds[*cmds_num] = strsep(&cl_rem, delim)) != NULL ) {
    *cmds_num++; 
  }
}

// =================================================
// ====== Check to see if end of requests :  =======
// =================================================
void check_INT_EOF(char * cl) {
  size_t len = 0;
  int cl_state = 0;
  
  if(sh_mode == INTERACTIVE_MODE)
    cl_state = getline( &cl , &len , stdin );  // read shell line   
  else cl_state = getline( &cl , &len , fptr); // read batch line
  
  if(cl_state == -1) { // exit prog if INT/EOF // quit routine : 
    if(sh_mode == BATCH_MODE) fclose(fptr);    // takes care of the file 
    free(cl);                                  // takes care of the heap 
    exit(0); 
  }
}

// =================================================
// =========== Launch processes here : =============
// =================================================
void executeCommands(char *args[], int args_num, FILE *out) {
  pid_t children[BUFF_SIZE];
  
  for(int c=0 ; c<args_num ; c++) {
    // LOOP TO FORK COMMAND , THEN WAIT.
    children[c] = fork();
  }

  for(int c=0 ; c<args_num ; c++) {
    // LOOP TO FORK COMMAND , THEN WAIT.
    //wait();
  }
}

// =================================================
// ===================== ??? : =====================
// =================================================
int searchPath(char path[], char *firstArg) {
  return 0;
}

// =================================================
// =============== > REDIRECTION : =================
// =================================================
void redirect(FILE *out) {
}



////////////////////////////////////////////////////
char *trim(char * a) {return a;}
void clean(void) {}
// == (requires multi-threading - not in instr) : ==
void * parseInput(void * arg) {return NULL;}
