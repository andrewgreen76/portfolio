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
// global for the clean-up function in the header : 
FILE * bat_ptr;
char * ln = NULL;
char path[BUFF_SIZE] = "/bin";
int expose = 1;

// function definitions : 
void interact();
void use_batch(char *);
void proc_ln();
int check_end();
void proc_cl();
void test_cmd(char *);
int eval_cmd(char *);

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
  bat_ptr = fopen(fname , "r");
  
  if(bat_ptr){     
    while(1) {    
      proc_ln();  // scan the batch
    }
  }
}

// =============================================================
// Process command : check for INT/EOF/blanks, else pass thru :
// =============================================================
void proc_ln() {

  // END OF COMMANDS (terminal code path) : 
  if( check_end() == -1) {
    clean();
    exit(0); 
  }

  // ignore blank lines 
  if( strcmp(ln,"\n")!=0 ) {
    proc_cl(); 
  }
  
  free(ln);
}

// =================================================
// ===== Breaks the line up into indiv. cmds : =====
// =================================================
void proc_cl() {
  //  char * new_cl = NULL;
  const char * delim_amp = "&";   // split into commands 
  const char * delim_space = " "; // strip off leading whitespace 
  char * cmd = NULL;              
  pid_t cid;

  if(expose) printf("Processing command line (composite)\n");
  ln = trim(ln);
  if(expose) printf("LINE : \"%s\" \n" , ln);
  
  // EXTRACT AND RUN COMMANDS NOW TO NOT WASTE CPU TIME : 
  while ( (cmd = strsep(&ln, delim_amp)) != NULL ) {   // removes term'ing whspc , appends "\n" , rets cmd_addr    
    cid = fork(); 
    if(cid==0) test_cmd(cmd); // TERMINAL CHILD
    else if(cid==-1) exit(0); // TERMINAL FAIL
    strsep(&ln, delim_space); // after "&" eat " " (to help with next cmd)    
  }
  
  // WAIT FOR ALL CHILDREN TO TERMINATE :
  int status = 0;
  while (waitpid(-1, &status, 0) > 0) {
    if ( !WIFEXITED(status) ) {
      printError();
      //printf("CHILD_TERM_ERR\n");
    }
  }
  printf("All children finished\n");
} 

// =================================================
// =============== Split and run :  ================
// =================================================
void test_cmd(char * cmd) {
  char * args[BUFF_SIZE];
  char * str;
  const char * delim = " ";
  int args_num = 0;
  FILE * out = NULL;
  
  cmd = trim(cmd);
  if(expose) printf("CMD : \"%s\" \n" , cmd);

  while ( (str = strsep(&cmd, delim)) != NULL ) {
    // checking on REDIRECTION , seeking OUTLOG : 
    if(    strcmp(str,">")==0    &&    (str = strsep(&cmd, delim)) != NULL    &&    (out = fopen(str,"w"))!=NULL ) {break;}
    // LOAD *args[] , COUNT args_num         
    args[args_num] = str;
    if(expose) printf("args[%d] : \"%s\" \n" , args_num , args[args_num]);
    args_num++;
  }

  // built-in() -or- execv(path to cmd, args to proc) : 
  executeCommands(args , args_num , out); // per CMDLN or CMD ??

  if ( (str = strsep(&cmd, delim)) != NULL ) {
    printError();
  }
  
  printf("Command processed\n");
  exit(0); 
}

// =================================================
// =========== Launch processes here : =============
// =================================================
void executeCommands(char *args[], int args_num, FILE *out) {
  eval_cmd(args[0]);

  if(out) fclose(out);
}

// =================================================
// ============== Classify command :  ==============
// =================================================
int eval_cmd(char * cmd) {
  if( !strcmp(cmd,"exit") || !strcmp(cmd,"cd") || !strcmp(cmd,"path") ) return 1;
  else return 0;
}

// =================================================
// ====== Check to see if end of requests :  =======
// =================================================
int check_end() {
  size_t len = 0;
  int cl_state = 0;
  
  if(sh_mode == INTERACTIVE_MODE)  // ----------- READ LINE 
    cl_state = getline( &ln , &len , stdin );     // load shell line ; ret 1 for blank , -1 for eof  
  else cl_state = getline( &ln , &len , bat_ptr); // load batch line ; ret 1 for blank , -1 for eof 
  
  return cl_state;
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
// ============== Cmd_str doctoring : ==============
// =================================================
char *trim(char * iostr) {
  char * substr = NULL; 
  char outstr[BUFF_SIZE] = "";
  const char * delim = " ";

  if(expose) {
    printf("Trimming input string : ");
    printf("\"%s\"" , iostr);
  }
  
  while( (substr = strsep(&iostr, delim)) != NULL ) {
    if(expose) {
      printf("Processing sub-string : ");
      printf("\"%s\"\n" , substr);
    }
    if( strcmp(substr," ")!=0 && strcmp(substr,"\n")!=0 && strcmp(substr,"\t")!=0) {
      if(expose) {
	printf("APPROVED STRING\n");
      }
      if(expose) printf("Last char : %d\n" , substr[strlen(substr)-1] );
      if( substr[strlen(substr)-1]=='\n' ) substr[strlen(substr)-1] = '\0';
      if(expose) printf("NOW : %d\n" , substr[strlen(substr)-1] );
      strcat(outstr , substr);
      strcat(outstr , " ");
    }
  }
  
  if(expose) {
    printf("outstr : ");
    printf("\"%s\"" , outstr);
  }

  //outstr[strlen(outstr)-1] = '\0';
  strcpy(iostr , outstr);
  if(expose) {
    printf("DOCTORED : ");
    printf("\"%s\"\n" , iostr);
  }
  
  //int end = strlen(in_str)-1; // [0:c 1:a 2:t 3:\n]=4 ; 

  /*
  while(in_str[end]==' ' || in_str[end]=='\n' || in_str[end]=='\t') {
    in_str[end]='\0';
    end--;
  }
  */
  
  return iostr;
}

// =================================================
// ======== Close and release everything : =========
// =================================================
void clean(void) {
  if(sh_mode == BATCH_MODE) fclose(bat_ptr);    // takes care of the file 
  free(ln);                                     // takes care of the heap   
}
////////////////////////////////////////////////////
////////////////////////////////////////////////////
// == (requires multi-threading - not in instr) : ==
void * parseInput(void * arg) {return NULL;}
