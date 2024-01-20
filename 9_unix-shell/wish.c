/*
  wish.c - code by Andrew Green 
*/

#define  _GNU_SOURCE
//#define  _POSIX_C_SOURCE 200809L
#define CST_CMD 1
#define FS_CMD 0

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
char path[BUFF_SIZE] = "/bin";
int expose = 1;
// made global for the clean-up function in the header : 
FILE * bat_ptr;
char * ln = NULL;

// function definitions : 
void interact();
void use_batch(char *);
void proc_ln();
int check_end();
void proc_cl();
void test_cmd(char *);
int eval_cmd(char *);
void exec_cstcmd(char * [] , int); 
void exec_fscmd(char * [] , int , FILE * ); 

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

  ln = trim(ln);
  if(expose) printf("ERODED cl : \"%s\" \n" , ln);
  
  // EXTRACT AND RUN COMMANDS NOW TO NOT WASTE CPU TIME : 
  while ( (cmd = strsep(&ln, delim_amp)) != NULL ) {   // removes term'ing whspc , appends "\n" , rets cmd_addr    
    cid = fork();
    
    if(cid==0) {     // CHILD 
      test_cmd(cmd);
      exit(0); 
    }
    else if(cid==-1) { // FAIL
      printError();
    }                  // PARENT continues
    
    strsep(&ln, delim_space); // after "&" eat " " (to help with next cmd)    
  }
  
  // WAIT FOR CHILDREN TO TERMINATE :
  int status = 0;    /* <- ??? 
                         + WIFEXITED() - how child did   
                         + WEXITSTATUS() - exit code  */ 
  while (waitpid(-1, &status, 0) > 0) {
    if ( WIFEXITED(status) && WEXITSTATUS(status)==0 ) {
      // child came back and returned its exit code 
    }
    else if( !WIFEXITED(status) ) {
      printError();
    }
  }
  
  if(expose) printf("All children came back\n");
} 

// =================================================
// =============== Split and run :  ================
// =================================================
void test_cmd(char * cmd) {
  char * args[BUFF_SIZE];
  char * token;
  const char * delim = " ";
  int args_num = 0;
  // Conflict of file operations to pass to execv() : 
  FILE * out = NULL;
  //int outputFile = 0; 
    
  cmd = trim(cmd);
  if(expose) printf("INDIV. CMD STREAM : \"%s\" \n" , cmd);

  // Collect tokens : 
  while ( (token = strsep(&cmd, delim)) != NULL ) {
    // checking on REDIRECTION ,          ensuring OUTLOG name                           
    if(    strcmp(token,">")==0    &&    (token = strsep(&cmd, delim)) != NULL   ) {
      out = fopen(token,"w");
      //outputFile = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    }
    
    // LOAD *args[] , COUNT args_num         
    args[args_num] = token;
    if(expose) printf("args[%d] : \"%s\" \n" , args_num , args[args_num]);
    args_num++;
  }

  // built-in() -or- execv(path to cmd, args to proc) : 
  executeCommands(args , args_num , out); // per CMDLN or CMD ??
  
  if(expose) printf("Command processed\n");
}

// =================================================
// ========== INTERPRET INDIV. COMMAND : ===========
// =================================================
void executeCommands(char *args[], int args_num, FILE *out) {
  
  // CUSTOM CMDS - NO REDIRECT per the project 
  if(eval_cmd(args[0]) == CST_CMD) {
    exec_cstcmd(args , args_num);    
  }  
  // FILESYSTEM CMDS - allow REDIRECTION per the project 
  else {
    exec_fscmd(args , args_num , out);    
  }
}

// =================================================
// ============ FILESYSTEM commands :  =============
// =================================================
void exec_fscmd(char *args[] , int args_num , FILE * out) {
  int out_fileno = 0 , path_i = 0;
  char cmd_path[BUFF_SIZE];
  char * dir_path = NULL;

  // Find given cmd in a path_dir : 
  if( (path_i = searchPath(path , args[0]))==-1 ) {
    printError(); 
    exit(1);
  }

  // If cmd is in a path_dir , ...  
  char * shrink_buff = strdup(path);  
  trim(shrink_buff);
  for( int i=0 ; i<=path_i ; i++){
    dir_path = strsep(&shrink_buff, " ");
  }
  strcpy(cmd_path , dir_path); 
  strcat(cmd_path, "/");
  strcat(cmd_path, args[0]);
  if(expose) printf("cmd_path : \"%s\"\n" , cmd_path);
  free(shrink_buff);

  // ??? redirect() ??? 
  // Tying STDOUT to the redirection file (if applicable)  
  if(out){
    out_fileno = fileno(out);
    dup2(out_fileno, STDOUT_FILENO);
    close(out_fileno);
  }

  // Disappear into execv() :
  /*
    cmd_path
    cmd_name : args[0]
    args[] = { cmd_name , "-flag" , NULL }

    if ( execv(cmd_path , args)==-1) {} 
  */
  if ( execv(cmd_path, args) == -1 ) {
    printError();;
    exit(1);
  }
  exit(0);
}

// =================================================
// =============== CUSTOM commands : ===============
// =================================================
void exec_cstcmd(char * args[] , int args_num){
  //if(expose) printf("CUSTOM COMMAND : \"%s\"\n" , args[0]);
  if(expose) printf("ARGS_NUM : %d\n" , args_num);
  if( strcmp(args[0] , "exit")==0 ){
    if( args_num>1 ) {
      printError();
      exit(1);
    }
    exit(0);
  }
  else if( strcmp(args[0] , "cd")==0 ){
    if(args_num!=2) {
      printError();
      exit(1);
    }
    if( chdir(args[1])==-1 ) {   // """/home""" , etc.  
      printError();
      exit(1);
    }
  }
  else { // "path"
    strcpy(path , "");
    for( int i=1 ; i != args_num ; i++ ){
      if(i!=1) strcat(path , " ");
      strcat(path , args[i]);
    }
    printf("path = \"%s\"\n" , path);
  }
}

// =================================================
// ==== External/sys commands (NOT BUILT-IN) :  ====
// =================================================
//                                 args[0]
int searchPath(char shrink_buff[], char *firstArg) {
  int path_i = 0;          // ret dir_cmd index 
  const char * delim = " ";
  char * token = NULL;     // for stripping 
  char dir_cmd[BUFF_SIZE]; // for construction 

  while( (token = strsep(&shrink_buff, delim))!=NULL ) {
    strcpy(dir_cmd , token); 
    strcat(dir_cmd, "/");
    strcat(dir_cmd, firstArg);

    if(expose) printf("Searching dir_cmd : \"%s\" ...\n" , dir_cmd);
    
    if( access(dir_cmd, X_OK)==0 ){
      if(expose) printf("External system command FOUND in path\n");
      if(expose) printf("@ \"%s\"\n" , dir_cmd);
      return path_i;
    }
    path_i++;
  }

  if(expose) printf("External system command NOT found in path\n");
  return -1;
}

// =================================================
// ============== Classify command :  ==============
// =================================================
int eval_cmd(char * cmd) {
  if( strcmp(cmd,"exit")==0 || strcmp(cmd,"cd")==0 || strcmp(cmd,"path")==0 ) return CST_CMD;
  else return FS_CMD;
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
// =============== > REDIRECTION : =================
// =================================================
void redirect(FILE *out) {
}

// =================================================
// ============== Cmd_str doctoring : ==============
// =================================================
char *trim(char * iostr) {
  int walked_alphanum = 0 , walked_wsp = 0 , walked_first_word = 0;
  int left = 0 , right = 0;

  while( iostr[right] != '\0' ){

    // ALPHANUM 
    if( iostr[right]!= ' ' && iostr[right]!= '\t' && iostr[right]!= '\n' ){
      if(!walked_alphanum && !walked_wsp){ // occurs once : very 1st char = alphanum ; first word
	left++;
	walked_alphanum = 1;
	walked_wsp = 0;
	walked_first_word = 1;
      }
      else if(walked_alphanum && !walked_wsp){ // 2+nd alphanum 
	iostr[left] = iostr[right]; // general case 
	left++;
      }
      else if(!walked_alphanum && walked_wsp){
	walked_alphanum = 1;
	walked_wsp = 0;

	// Tricky case of whitespace before first word : 
	if(walked_first_word){
	  iostr[left] = ' ';
	  left++;
	}
	if(!walked_first_word) walked_first_word = 1;
	
	iostr[left] = iostr[right];
	left++;
      }
    }
    // WHITESPACE
    else{
      if(!walked_wsp && !walked_alphanum){ // occurs once : very 1st char = whitespace ; no first word
	walked_wsp = 1;
	walked_alphanum = 0;
	// left - stay ; right - do nothing , move on 
      }
      else if(!walked_wsp && walked_alphanum) {
	walked_wsp = 1;
	walked_alphanum = 0;
	// Stay , make sure this isn't end of commands. 
      }
      else if(walked_wsp && !walked_alphanum){
	// 2nd wsp in a row - do nothing , move on 
      }
    }
    
    right++;
  }

  // wipe right to left (end to mid-mark) 
  right--;
  while(right>=left){
    iostr[right] = '\0';
    right--;
  }

  //printf("iostr : \"%s\"\n" , iostr);

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
