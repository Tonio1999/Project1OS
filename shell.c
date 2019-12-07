#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COMMAND_LINE_LEN 1024
#define MAX_COMMAND_LINE_ARGS 128

char prompt[] = "shell > ";
char delimiters[] = " \t\r\n";
char **environ;

int main() 
{
    char command_line[MAX_COMMAND_LINE_LEN];
    char *arguments[MAX_COMMAND_LINE_ARGS];
    pid_t pid;
    
    while (true) 
    {
        printf("%s", prompt);
        fflush(stdout);
        if ((fgets(command_line, MAX_COMMAND_LINE_LEN, stdin) == NULL) && ferror(stdin)) 
        {
            fprintf(stderr, "fgets error");
            exit(0);
        }
        if (feof(stdin)) 
        {
            printf("\n");
            fflush(stdout);
            fflush(stderr);
            return 0;
        }
        char *command;
        char *arg;
        int i = 0;  
        command = strtok(command_line,delimiters);
        arg = strtok(NULL,delimiters);
        while (arg != NULL)
        {
          arguments[i] = arg;
          i+=1;
          arg = strtok(NULL,delimiters);
        }
        pid = fork();
        
        if (pid < 0)
        {
            perror("Fork error!\n"); 
          exit(1);
        }
      
        else if (pid == 0)
        {
          printf("This is the Child process!\n");
          if (execve(command, arguments, environ) < 0 )
          {
            perror("Execution error!\n");   
            exit(1); 
          }
          exit(0);
        }
        else 
        {
          printf("This is the Parent process!\n");
          wait(NULL);
          printf("Child has terminated.\n");
        }
    }
    return -1;
}
