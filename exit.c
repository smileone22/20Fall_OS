exit: Ending a process

- void exit (int status)
		- exits a process, normally return with status 0
		- But one small variation: atexit (function_name) make function_name execute upon exit 
				- before you exit, it will execute the function_name

- Two questions to think about 
1. exit will end the process, then what is the importance of having an argument?
	- nothing to do with that value 
2. exit is void. Why is that?
	- If it returns a number, what should we do with the number since it exited? 
	- indicate error ? to the parent process
	- 0 means terminated normally. 
	- The reason for antoher number is because the parent process needs to know whether
	  the child finished correctly or there is a problem. 
	- What if the process does not have a parent? that is never the case. At least the 
	  initial kernal will be the parent.  


void cleanup(void){
	printf("cleaning up\n"); // nothing done yet, before exit executed the function clean up willl be executed
}

void fork6(){
	atexit(cleanup);
	fork();
	exit();
}

When a process finishes, there is a command in linux called ps(process status)
It will tell you that processes are related to only you. After exitng process, you will
find that the ps will show you that process that exited. Why?

Zombies!
	- Idea: when process terminates, still consumes system resources
			(except!! an entry in process table)
	- Why? So that parents can learn of childrens exit status
	- Called a "Zombie"
	- There was a state called finished. Exit will lead to finish status 
	- What will need to happen to be removed from process table?
		We need REAPING
		- performed by parent on terminated child (parent does not have access 
			to process table 
			but it can directly tell os that children are done)
		- Parent is given exit status information
		- Os discareds process 
		- What if the parent does not reap? 
			If parent has terminated, then child will be reaped by init process
			(the great-great-..-great grandparent of all user-level processes)


Zombie example 

void fork7(){
	if (fork()==0){
		/*CHILD*/
		printf("Terminating Child, PID=%d\n",getpid());
		exit(0) // ps - defunct - however we still have entry on process table 
		// killing the parent will do reaping , ps again , no forks 

	} else { //parent will be doing 
		printf("Running Parent, PID =%d\n",getpid()); //print own id
		while (1); /*parent will go INFINITE LOOP*/
	}
}


- ps shows child process as defunct
- killing parent allows child to be reaped by init 































