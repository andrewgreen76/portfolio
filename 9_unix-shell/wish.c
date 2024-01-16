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


int sh_mode = INTERACTIVE_MODE; // 1 - interactive , 2 - batch
FILE * fptr;
char cl[BUFF_SIZE];
void interact();
void use_batch(char *);
void proc_ln();
void check_INT_EOF(char *);
void tokenize(char *);

typedef struct{
} clargs;

// ===============================================================
// =========================== MAIN : ============================
// ===============================================================
int main(int argc, char *argv[]) {
  printf("\n");
  if(argc>2){ 
    printError();
    exit(1);
  }
  // ============== Enter a mode , engage : ===============
  sh_mode = argc;
  if(sh_mode == INTERACTIVE_MODE) interact();
  else use_batch(argv[1]);
/*   fork/wait/ EXECV(cmd) 
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
    proc_ln();
  }
}

// ===============================================
// ================ BATCH MODE : =================
// ===============================================
void use_batch(char * fname){
  fptr = fopen(fname , "r");
  
  if(fptr){       // if found, 
    while(1)      //
      proc_ln();  // scan the batch 
    fclose(fptr);
  }
}

// ====================================================
// Process command : check for blanks, else pass thru :
// ====================================================
void proc_ln() {
  char * ln = NULL;
  char *args[];
  
  catch_INT_EOF(ln);
  tokenize(ln);
  executeCommands(char *args[], int args_num, FILE *out); //  
  free(ln);
}

// =================================================
// ========= Check to see if empty line : ==========
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
// ======= TOKENIZES a line , ACTS on it :  ========
// =================================================
void tokenize(char * cl_rem){  
  char * token; 
  const char delim = " ";
  int built_in = 0;

  while ( (token = strsep(&cl_rem, delim)) != NULL) {
    //printf("Token: %s\n", token);
  }

  return NULL;
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

// =================================================
// =========== Launch processes here : =============
// =================================================
void executeCommands(char *args[], int args_num, FILE *out) {
}



////////////////////////////////////////////////////
char *trim(char *) {return NULL;}
void clean(void) {}
// == (requires multi-threading - not in instr) : ==
void * parseInput(void * arg) {return NULL;}
