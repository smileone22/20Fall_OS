#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
	while (1){
		printf("lab1>");
		char command[100];
		fgets(command,100,stdin); 
		printf("Parent Process %d\n",getpid());
        command[strlen(command) - 1] = '\0';
	    
        if (strcmp(command,"printid")==0){
		    //print on the screen The ID of the current process is X where x is the process ID.
	        printf("The ID of the current process is %d\n", getpid());
        } else if (strcmp(command,"exit")==0){
		    //ends the program and exits
		    exit(0);
	    } else if (strcmp(command,"greet")==0) {
		    //prints “Hello\n” on the screen
		    printf("Hello\n");
	    }  else{
            int pid = fork();
            if (pid == 0){ // child p 
                printf("Child process %d will execute the command %s\n",getpid(),command);
                char path[100] = "/bin/";
                char *progname[2];
                progname[0]=command;
                progname[1]=NULL;
                execve(strcat(path,command),progname,NULL);
                printf("Command Not Found!\n");
                exit(0);
            }
        }
	wait(NULL);
    }

}

