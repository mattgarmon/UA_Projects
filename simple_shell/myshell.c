// ACADEMIC INTEGRITY PLEDGE
//
// - I have not used source code obtained from another student nor
//   any other unauthorized source, either modified or unmodified.
//
// - All source code and documentation used in my program is either
//   my original work or was derived by me from the source code
//   published in the textbook for this course or presented in
//   class.
//
// - I have not discussed coding details about this project with
//   anyone other than my instructor. I understand that I may discuss
//   the concepts of this program with other students and that another
//   student may help me debug my program so long as neither of us
//   writes anything during the discussion or modifies any computer
//   file during the discussion.
//
// - I have violated neither the spirit nor letter of these restrictions.
//
//
//
// Signed:__Matt_Garmon_______________________________ Date:__4/24/18_____

// 3460:426 Lab 4A - Basic C shell

/* Basic shell */

/*
 * This is a very minimal shell. It finds an executable in the
 * PATH, then loads it and executes it (using execv). Since
 * it uses "." (dot) as a separator, it cannot handle file
 * names like "minishell.h"
 *
 * The focus on this exercise is to use fork, PATH variables,
 * and execv.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_ARGS	64
#define MAX_ARG_LEN	16
#define MAX_LINE_LEN	80
#define WHITESPACE	" ,\t\n"

struct command_t {
   char *name;
   int argc;
   char *argv[MAX_ARGS];
};

/* Function prototypes */
int parseCommand(char *, struct command_t *);
void printPrompt();
void readCommand(char *);
void error(int);
void displayHelp();

int main(int argc, char *argv[]) {
   int pid;
   int status;
   int i, bg = 0;
   char cmdLine[MAX_LINE_LEN];
   struct command_t command;

   while(1) {
      printPrompt();
      /* Read the command line and parse it */
      readCommand(cmdLine);
      parseCommand(cmdLine, &command);
      command.argv[command.argc] = NULL;

      /* Interpret command */
      if(strcmp(command.name, "") == 0) continue;
      if(strcmp(command.name, "C") == 0){ //copy
          if(command.argc != 3){
              error(0);
              continue;
          }
          command.name = (char*)"cp";
      }
      else if(strcmp(command.name, "D") == 0){ //delete
          if(command.argc != 2){
              error(0);
              continue;
          }
          command.name = (char*)"rm";
      }
      else if(strcmp(command.name, "E") == 0){ //echo
          if(command.argc != 1){
            for(i = 1; i < command.argc; ++i){
              printf("%s ", command.argv[i]);
            }
            printf("\n");
          }
          continue;
      }
      else if(strcmp(command.name, "H") == 0){ //help
         if(command.argc != 1){
              error(0);
              continue;
          }
        displayHelp();
        continue;
      }
      else if(strcmp(command.name, "L") == 0){ //list
          if(command.argc != 1){
              error(0);
              continue;
          }
          if ((pid = fork()) == 0){
              parseCommand((char*)"pwd", &command);
              execvp(command.name, command.argv);
          }
          wait(&status);
          if ((pid = fork()) == 0){
              parseCommand((char*)"ls", &command);
              command.argv[1] = (char*)"-l";
              command.argv[2] = NULL;
              execvp(command.name, command.argv);
          }
          wait(&status);
          continue;
      }
      else if(strcmp(command.name, "M") == 0){ //make
          if(command.argc != 2){
              error(0);
              continue;
          }
          command.name = (char*)"nano";
      }
      else if(strcmp(command.name, "P") == 0){ //print
          if(command.argc != 2){
              error(0);
              continue;
          }
          command.name = (char*)"more";
      }
      else if(strcmp(command.name, "Q") == 0) break; //quit
      else if(strcmp(command.name, "S") == 0){ //surf
          if(command.argc != 1){
              error(0);
              continue;
          }
          command.name = (char*)"firefox";
          bg = 1; //background process
      }
      else if(strcmp(command.name, "W") == 0){ //wipe
          if(command.argc != 1){
              error(0);
              continue;
          }
          command.name = (char*)"clear";
      }
      else if(strcmp(command.name, "X") == 0){ //execute
          char pr[MAX_ARG_LEN];
          pr[0] = '.';
          pr[1] = '/';
          for(i = 2; i < sizeof(command.argv[1])/sizeof(char); ++i)
            pr[i] = command.argv[1][i-2];
          command.name = (char*)pr;
      }
      command.argv[0] = command.name;

      /* Create a child process to execute the command */
      if ((pid = fork()) == 0) {
         /* Child executing command */
         execvp(command.name, command.argv);
      }
      /* Wait for the child to terminate*/
      else if(pid > 0) {
          if(bg) continue; //background process
          else wait(&status);
      }
      else{
          error(1); //fork error
          return 1;
      }
   }

   /* Shell termination */
   printf("shell: Terminating successfully\n");
   return 0;
}

/* End basic shell */

/* Parse Command function */

/* Determine command name and construct the parameter list.
 * This function will build argv[] and set the argc value.
 * argc is the number of "tokens" or words on the command line
 * argv[] is an array of strings (pointers to char *). The last
 * element in argv[] must be NULL. As we scan the command line
 * from the left, the first token goes in argv[0], the second in
 * argv[1], and so on. Each time we add a token to argv[],
 * we increment argc.
 */
int parseCommand(char *cLine, struct command_t *cmd) {
   int argc;
   char **clPtr;
   /* Initialization */
   clPtr = &cLine;	/* cLine is the command line */
   argc = 0;
   cmd->argv[argc] = (char *) malloc(MAX_ARG_LEN);
   /* Fill argv[] */
   while ((cmd->argv[argc] = strsep(clPtr, WHITESPACE)) != NULL) {
      cmd->argv[++argc] = (char *) malloc(MAX_ARG_LEN);
   }

   /* Set the command name and argc */
   cmd->argc = argc-1;
   cmd->name = (char *) malloc(sizeof(cmd->argv[0]));
   strcpy(cmd->name, cmd->argv[0]);
   return 1;
}

/* End parseCommand function */

/* Print prompt and read command functions - pp. 79-80 */

void printPrompt() {
   /* Build the prompt string to have the machine name,
    * current directory, or other desired information
    */
   char promptString[] = "linux (mjg119)|> ";
   printf("%s ", promptString);
}

void readCommand(char *buffer) {
   /* This code uses any set of I/O functions, such as those in
    * the stdio library to read the entire command line into
    * the buffer. This implementation is greatly simplified,
    * but it does the job.
    */
   fgets(buffer, 80, stdin);
}

/* End printPrompt and readCommand */

void error(int errorCode) {
    switch(errorCode){
        case 0: printf("Insufficient parameters.\n"); break;
        case 1: printf("Fork error.\n"); break;
        default: printf("General error.\n");
    }
}

void displayHelp() {
    printf("COMMAND         DESCRIPTION\n");
    printf("C file1 file2   Copy; create file2, copy file1 to file2 without deleting file1.\n");
    printf("D file          Delete the named file.\n");
    printf("E comment       Echo; display comment on screen.\n");
    printf("H               Help; display this user manual.\n");
    printf("M file          Make; create the named text file.\n");
    printf("P file          Print; display the contents of the named file on screen.\n");
    printf("Q               Quit the shell.\n");
    printf("S               Surf the web by launching a browser.\n");
    printf("W               Wipe; clear the screen.\n");
    printf("X program       Execute the named program.\n");
}
