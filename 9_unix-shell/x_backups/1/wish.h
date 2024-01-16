#include <pthread.h> // pthread_create, pthread_join
#include <regex.h>   // regcomp, regexec, regfree
#include <stdio.h>   // fopen, fclose, fileno, getline, feof
#include <string.h>  // strlen, strsep, strcat, strdup, strcmp
#include <unistd.h>  // STDERR_FILENO, fork, exec, access, exit, chdir

#ifdef REG_ENHANCED  // macOS: man re_format
#  define REG_CFLAGS REG_EXTENDED | REG_NOSUB | REG_ENHANCED
#else
#  define REG_CFLAGS REG_EXTENDED | REG_NOSUB
#endif

#define INTERACTIVE_MODE 1
#define BATCH_MODE 2
#define BUFF_SIZE 256

// thread args 
struct function_args {
  pthread_t thread;
  char *command;
};

// ===============================================================
// ======================== Functions : ==========================
// ===============================================================
void printError() {
  char error_message[30] = "An error has occurred\n";
  write(STDERR_FILENO, error_message, strlen(error_message));
}

void *parseInput(void *arg);  // ------------------------------- thread / basic shell
int searchPath(char path[], char *firstArg); // ----------------- paths
void redirect(FILE *out); // ------------------------------------redirection 
void executeCommands(char *args[], int args_num, FILE *out); // thread / parallel commands 
char *trim(char *); // ? trim the read line token by token ? 
void clean(void);   // ? free the buffer and close the file before exit ? 
