#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc , char *argv[]) {
  printf("argc : %d\n" , argc);
  
  /*
  char * args[] = {"/bin/ls",  NULL};
  int status;
  pid_t x = fork();

  if(x==0){
    printf("Child\n");
    execv("/bin/ls", args);
    //printf("Child finished\n");
    //exit(0);
  }
  if(x==-1){
    printf("Fail\n");    
  }
  else{
    printf("Parent\n");
    wait(&status);
    printf("Parent woke up\n");
  }
  
  printf("Finish line\n");    
  */  
  return 0;
}
