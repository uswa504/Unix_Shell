#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LEN 512
#define MAXARGS 10
#define ARGLEN 30
int execute(char* arglist[], char*,char*);
char** tokenize(char* cmdline);
char* read_cmd(char*, FILE*);
int main(){
   char *output_file;
   char *input_file;
   char *cmdline;
   char** arglist;
   char cwd[1024];
   char str[80];
   if(signal(SIGINT, sig_handler) == SIG_ERR)
      printf("\nSignal not catched\n");
   getcwd(cwd, sizeof(cwd));
   strcpy(str,"myshell@");
   strcat(str, cwd);
   strcat(str, ":>>>");
   while((cmdline = read_cmd(str,stdin)) != NULL){
      if((arglist = tokenize(cmdline)) != NULL){
            execute(arglist);
         for(int j=0; j < MAXARGS+1; j++)
	         free(arglist[j]);
         free(arglist);
         free(cmdline);
      }
  }
  printf("\n");
  return 0;
}
int execute(char* arglist[], char* input_file, char* output_file){
   int status;
   int result;
   int cpid = fork();
   switch(cpid){
      case -1:
         perror("fork failed");
	 exit(1);
      case 0:
	      fd_in = open(input_file, O_RDONLY);
     	      close(0);
	      dup(fd_in);
	      close(fd_in);
	      fd_out=creat(output_file, 0644);
	      close(1);
	      dup(fd_out);
	      close(fd_out);
              execvp(arglist[0], arglist);
 	      perror("Command not found...");
	      exit(1);
      default:
	 waitpid(cpid, &status, 0);
         return 0;
   }
}
char** tokenize(char* cmdline){
//allocate memory
   char** arglist = (char**)malloc(sizeof(char*)* (MAXARGS+1));
   for(int j=0; j < MAXARGS+1; j++){
	   arglist[j] = (char*)malloc(sizeof(char)* ARGLEN);
      bzero(arglist[j],ARGLEN);
    }
   if(cmdline[0] == '\0')//if user has entered nothing and pressed enter key
      return NULL;
   int argnum = 0; //slots used
   char*cp = cmdline; // pos in string
   char*start;
   int len;
   while(*cp != '\0'){
      while(*cp == ' ' || *cp == '\t') //skip leading spaces
          cp++;
      start = cp; //start of the word
      len = 1;
      //find the end of the word
      while(*++cp != '\0' && !(*cp ==' ' || *cp == '\t'))
         len++;
      strncpy(arglist[argnum], start, len);
      arglist[argnum][len] = '\0';
      argnum++;
   }
   arglist[argnum] = NULL;
   return arglist;
}
char* read_cmd(char* prompt, FILE* fp){
  printf("%s", prompt);
  int c; //input character
  int pos = 0; //position of character in cmdline
  char* cmdline = (char*) malloc(sizeof(char)*MAX_LEN);
  while((c = getc(fp)) != EOF){
       if(c == '\n')
	  break;
       cmdline[pos++] = c;
   }
//these two lines are added, in case user press ctrl+d to exit the shell
   if(c == EOF && pos == 0) 
      return NULL;
   cmdline[pos] = '\0';
   return cmdline;
}

