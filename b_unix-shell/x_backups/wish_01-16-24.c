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
void proc_cl();
void catch_INT_EOF();

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
/*   fork/wait/execv(cmd) 
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
    proc_cl();
  }
}

// ===============================================
// ================ BATCH MODE : =================
// ===============================================
void use_batch(char * fname){
  fptr = fopen(fname , "r");
  
  if(fptr){       // if found, 
    while(1)      //
      proc_cl();  // scan the batch 
    fclose(fptr);
  }
}

// ====================================================
// Process command : check for blanks, else pass thru :
// ====================================================
void proc_cl() {
  catch_INT_EOF();
  parseInput(cl);  
}

// =================================================
// ========= Check to see if empty line : ==========
// =================================================
void catch_INT_EOF() {
  char * cl = NULL;
  size_t len = 0;
  int ln_state = 0;

  if(sh_mode == INTERACTIVE_MODE)
    ln_state = getline( &cl , &len , stdin );  // read shell line   
  else ln_state = getline( &cl , &len , fptr); // read batch line
  
  if(ln_state == -1) { // exit prog if INT/EOF // quit routine : 
    if(sh_mode == BATCH_MODE) fclose(fptr);    // takes care of the file 
    free(cl);                                  // takes care of the heap 
    exit(0); 
  }
}

// =================================================
// ======= TOKENIZES a line , ACTS on it :  ========
// =================================================
void * parseInput(char * cl_rem){
  char * token;
  const char delim = " ";

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
// =============== THREADING HERE : =================
// =================================================
void executeCommands(char *args[], int args_num, FILE *out) {
}

char *trim(char *) {
  return NULL;
}

void clean(void) {
}
