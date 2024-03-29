#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define HISTORY_COUNT 10
#define MAX_LEN 512
#define MAXARGS 10
#define ARGLEN 30
int execute(char* arglist[], int, int, char*, int, char*);
char** tokenize(char* cmdline);
char* read_cmd(char*, FILE*);
int history(char* hist[], int);
int redirect_input(char* arglist[], char** input_file);
void execute_pipe(char* arglist[], char** piped_args);
int background_process(char *arglist[]);
int check_pipe(char* arglist[], char** piped_args);
int check_semicolons(char* arglist[], char** multiple_args);
int redirect_output(char* arglist[], char** output_file);
int internal_commands(char *arglist[]);
void signal_handler(int signo){
  printf("Signal Caught!! \n");
}
int main(){
   int pipes;
   int block;
   int input;
   int output;
   int var;
   int number = 1;
   char **multiple_args;
   char **piped_args;
   char *output_file;
   char *hist[HISTORY_COUNT];
   char *input_file;
   char *cmdline;
   char** arglist;
   char cwd[1024];
   char str[80];
   for(var = 0; var < HISTORY_COUNT; var++)
     hist[var] = NULL;
   signal(SIGINT, SIG_DFL);
   getcwd(cwd, sizeof(cwd));
   strcpy(str,"cs321shell@");
   strcat(str, cwd);
   strcat(str, ":>");
   signal(SIGINT, signal_handler);
   while((cmdline = read_cmd(str,stdin)) != NULL){
      if((arglist = tokenize(cmdline)) != NULL){
       free(hist[number]);
       hist[number]= strdup(cmdline);
       number = (number + 1) % HISTORY_COUNT;
       if(arglist[0][0] == '!'){
         int opt = arglist[0][1] - '0';
         history(hist, opt);
         continue;
       }
       if(check_pipe(arglist, piped_args)){
         execute_pipe(arglist, piped_args);
         exit(0);
       }
       if(internal_commands(arglist))
         continue;
     //if(background_process(arglist))
        //continue;
    input = redirect_input(arglist, &input_file);
    switch(input){
     case -1:
       printf("Error in input redirection \n");
       continue;
       break;
     case 0:
       break;
     case 1:
       printf("Redirecting from %s\n", input_file);
    }
    output = redirect_output(arglist, &output_file);
    switch(output){
     case -1:
       printf("Error in output \n");
       continue;
       break;
     case 0:
       break;
     case 1:
       printf("Redirecting to %s\n", output_file);
    }
    execute(arglist, block, input, input_file, output, output_file);//  need to free arglist
         for(int j=0; j < MAXARGS+1; j++)
	         free(arglist[j]);
         free(arglist);
         free(cmdline);
      }
      getcwd(cwd, sizeof(cwd));
      strcpy(str,"cs321shell@");
      strcat(str, cwd);
      strcat(str, ":>");
   }
   printf("\n");
   return 0;
}
int history(char *hist[], int number){
  int i = number;
  printf("%s\n", hist[i]);
  return 0;
}
int internal_commands(char *arglist[]){
  int total_commands = 5;
  int i;
  int switch_args = 0;
  char* commands[total_commands];
  commands[0] = "cd";
  commands[1] = "exit";
  commands[2] = "jobs";
  commands[3] = "kill";
  commands[4] = "help";
  for(i=0; i<total_commands; i++){
    if(strcmp(arglist[0], commands[i]) == 0){
     switch_args = i+1;
     break;
    }
  }
  switch(switch_args){
  case 1:
    chdir(arglist[1]);
    return 1;
  case 2:
    exit(0);
  case 5:
    for(i=0; i<total_commands; i++){
      printf(commands[i]);
      printf("\n");
    }
    return 1;
  default:
    break;
  }
  return 0;
}
void execute_pipe(char *arglist[], char **piped_args){
  int fd[2];
  pid_t p1, p2;
  if(pipe(fd) < 0){
   printf("Pipes can't be initiated\n");
   return;
  }
  p1 = fork();
  if(p1 < 0){
   printf("Can't fork\n");
  }
  if(p1 == 0){
   close(fd[0]);
   dup2(fd[1], STDOUT_FILENO);
   close(fd[1]);
   if(execvp(arglist[0], arglist) < 0){
     printf("cant exec 1\n");
   }
  }
  else{
   p2 = fork();
   if(p2  <0){
     printf("Can't fork\n");
     exit(0);
   }
   if(p2 == 0){
    close(fd[1]);
    dup2(fd[0], STDIN_FILENO);
    close(fd[0]);
    if(execvp(piped_args[0], piped_args) < 0){
      printf("cant exec 2\n");
      exit(0);
    }
   }
   else{
    wait(NULL);
    wait(NULL);
   }
  }
}
int background_process(char *arglist[]){
   int i;
   for(i=0; arglist[i] != NULL; i++){
      if(arglist[i-1][0] == '&'){
        free(arglist[i-1]);
        arglist[i-1] = NULL;
        return 1;
      }
      else return 0;
   }
   return 0;
}
int check_pipe(char *arglist[], char** piped_args){
   int i;
   int j;
   for(i=0; arglist[i] != NULL; i++){
     if(arglist[i][0] == '|'){
      free(arglist[i]);
      if(arglist[i+1] != NULL){
  	*piped_args = arglist[i+1];
      }
      else return -1;
      for(j=i; arglist[j-1] != NULL; j++){
       	arglist[j] = arglist[j+2];
      }
      return 1;
   }
  }
  return 0;
}
int redirect_input(char* arglist[], char** input_file ){
   int i;
   int j;
   for(i=0; arglist[i]!=NULL; i++){
     if(arglist[i][0] == '<'){
      free(arglist[i]);
      if(arglist[i+1] != NULL){
  	*input_file = arglist[i+1];
      }
      else return -1;
      for(j=i; arglist[j-1] != NULL; j++){
       	arglist[j] = arglist[j+2];
      }
      return 1;
   }
  }
  return 0;
}
int redirect_output(char* arglist[], char** output_file ){
   int i;
   int j;
   for(i=0; arglist[i]!=NULL; i++){
     if(arglist[i][0] == '>'){
      free(arglist[i]);
      if(arglist[i+1] != NULL){
  	*output_file = arglist[i+1];
      }
      else return -1;
      for(j=i; arglist[j-1] != NULL; j++){
       	arglist[j] = arglist[j+2];
      }
      return 1;
   }
  }
  return 0;
}
int execute(char* arglist[], int block, int input, char* input_file,int output,  char* output_file){
   int status;
   int result;
   int cpid = fork();
      switch(cpid){
      case -1:
         perror("fork failed");
	 exit(1);
      case 0:
	      if(input){
		freopen(input_file, "r", stdin);
	      }
	      if(output){
                freopen(output_file, "w+", stdout);
              }
              execvp(arglist[0], arglist);
 	      perror("Command not found...");
	      exit(1);
      default:
	 waitpid(cpid, &status, 0);
         return 0;
       }
}
char** tokenize(char* cmdline){ //allocate memory
   char** arglist = (char**)malloc(sizeof(char*)* (MAXARGS+1));
   for(int j=0; j < MAXARGS+1; j++){
	   arglist[j] = (char*)malloc(sizeof(char)* ARGLEN);
      bzero(arglist[j],ARGLEN);
    }
   if(cmdline[0] == '\0') //if user has entered nothing and pressed enter key
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
char* read_cmd(char* str, FILE* fp){
  printf("%s", str);
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
