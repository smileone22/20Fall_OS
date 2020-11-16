
wait: Synchronizing with Children

- init wait (int * child_status)
	- blocks until some child exits, 
	return value is the pid of the terminated child
	// You are a process, you call wait, you will be blocked until
	// all your children finishes or exits. 
	- int for knowing whatever child status are  
	- If multiple children completed, will take in arbitrary order
	(use waitpid to wait for a specific child)
	- goes only one generation only


void fork8(){
	if (fork()==0){ //child will print, two processes existing
		printf("HC:hello from child\n");
	}
	else { // parent printing 
		printf("HP:hello from parent\n");
		wait (NULL); // wait null - meaning waiting for child to finish
		// parent won't proceed until child finishes
		// null- not caring whatever child status are 

		printf("CT:child has terminated\n");
		//meaning child exited
		// we are sure that HC will be printed before CT 
		// child won't be finished until printing HC
		// CT will be always printed after HC
		// BUT order of HP ? out of order 

	}
	printf("Bye\n");
	exit(0); 
}
 // This is how child process is reaped by parent process 
- How many times Bye will be printed on the screen? 2 times
	b/c fork() - two processes, after child finishes, printf bye is not 
	in any condition. So bye printed by child and parent. 
	CT will be always printed after HC. 
	Child will not be finished before it prints HC and bye. 
	Then the parent can proceed print CT and bye at the end. 
> HP HC (out of order ) Bye CT Bye 
















- execve (OVERWRITING)
	int execve ( char * fname, char *argv[], char *envp[])
		- executes program named by fname
		- called once, never returns 
polymorphism is when you can act differently depending on arguments etc
fork creates a copy of process and do if else do differnt thing
But this is not really flexible 
You have to take care of exit status, status etc
What does execve do? It is called by a process and it does not return. 

It takes at least two args. 
1. String: path to an executable file 
once a process call execve, it will not create another process
but it will erase the calling process and morph it with the executable 
speicified first argument. 
- int is just in case with error:
	when you open a terminal, you type a command like ps. What will happen is that
	process itself created by os will fork itself. Second process will execve
	overwrite it by (information of p1)commmand you have wrote down. 

2. Argument of that executable. Array of pointers. 




if ((pid = fork())==0){
	//Child runs user job
	if (execve(argv[0],argv,environ) <0 ){
		printf("%s: Command not found.\n", argv[0]);
		exit(0);
	}
}

execve: Load a new program image (executable file on your disk)
- execve causes OS to overwrite code, data, and stack of process 
	- keeps pid and open files


- linking already before generating executable file 
- loading and linking will be done by execve
- static done before
















