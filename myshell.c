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
int execute(char* arglist[], int, char*, int, char*);
char** tokenize(char* cmdline);
char* read_cmd(char*, FILE*);
int redirect_input(char* arglist[], char** input_file);
int redirect_output(char* arglist[], char** output_file);
int main(){
   int input;
   int output;
   char *output_file;
   char *input_file;
   char *cmdline;
   char** arglist;
   char cwd[1024];
   char str[80];
   signal(SIGINT, SIG_DFL);
   getcwd(cwd, sizeof(cwd));
   strcpy(str,"myshell@");
   strcat(str, cwd);
   strcat(str, ":>>>");
   while(1){
    printf(str);
    arglist = getline();
    input = redirect_input(arglist, &input_file);
    switch(input){
     case -1:
       printf("error");
       continue;
       break;
     case 0:
       break;
     case 1:
       printf("Redirecting from %s", input_file);
    }
    output = redirect_output(arglist, &output_file);
    switch(output){
     case -1:
       printf("error");
       continue;
       break;
     case 0:
       break;
     case 1:
       printf("Redirecting to %s", output_file);
    }
    execute(arglist, input, input_file_name, output, output_file);
   }
   printf("\n");
   return 0;
}
int redirect_input(char* arglist[], char** input_file ){
   int i;
   int j;
   for(i=0; arglist[i]!=NULL; i++){
     if(arglist[i][0] == '<'){
      free(arglist[i];
      if(arglist[i+1] != NULL){
  	*input_file = arglist[i+1};
      }
      else return -1;
      for(j=i; args[j-1] != NULL; j++){
       	args[j] = args[j+2];
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
      free(arglist[i];
      if(arglist[i+1] != NULL){
  	*output_file = arglist[i+1};
      }
      else return -1;
      for(j=i; args[j-1] != NULL; j++){
       	args[j] = args[j+2];
      }
      return 1;
   }
  }
  return 0;
}
int execute(char* arglist[], int input, char* input_file,int output,  char* output_file){
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
/*char** tokenize(char* cmdline){
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
}*/

