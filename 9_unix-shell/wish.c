#include "wish.h"
#include <ctype.h>  // isspace
#include <regex.h>  // regcomp, regexec, regfree
#include <stdio.h>  // fopen, fclose, fileno, getline, feof
#include <stdlib.h> // exit
#include <sys/types.h>
#include <sys/wait.h> // waitpid


int main(int argc, char *argv[]) {

/*
  cmd = 0

while(1) 
    prompt "> "
    get line  
    parse line 
    extract cmd
    
    if(cmd=="exit") break

    fork/wait/exec(cmd) 

/////////// CHILD ///////////
 -> shell response	  
    -> execute the command 
    -> child finishes
 -> parent resumes , prompts again 
*/
  
  return 0;
}
