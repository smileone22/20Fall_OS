
CLASS 0916, 0921 


fork: Creating new processes 
- int fork( void ) // creates a new process(child process) that is identical to the calling the processes(parent process), fork is called once but returns twice!


pid_t in header file
pid_t pid= fork() //pointing to process n, child process m 
if (pid==0){ //fork will return m to process n, return 0 to the child process
	printf("hello from child\n")
} else{ //return child's pid to the parent
	printf("hello from parent: child pid is %d\n",pid);
}


Fork example #1
- parent and child both run same code
		- distinguish parent from child by return value from fork
- start with same state, but each has private copy of memory
		- including shared output file descriptor
		- relative ordering of their parent statements undefined
		
void fork1(){
	
	int x =1 //local variable
	pid_t pid =fork() //another process when fork ,return number to pid.(int, process id type)
	//once fork is called then we have another process with its own adress space. each one has own x.
	if (pid==0){ //child
		printf("Child has x= %d\n", ++x) // do operation but after incrementation ); x equals two  
	} else { //parent 
		printf("Bye from process %d with x=%d\n",getpid(),x); //at the same time, the parent will be decrement x and it will become zero

	}

}

// What I will have on the screen is x equals 2 , parent =0 , we can't not know which will be printed first
// getting own id at the else 

Both parent and child are accessing the screen b/c they want to print something one the screen working at the same time(multicore) , 
why don't we see scrambled characters ? 

Buffer flushing? both processes will have buffer. 
Thye are working concurrently. Why doesn't os print something scrambled? 
Os intends a queue for wahtever i/o operation it is doing . So if printing to screen, it will create a queue so that 
one process finiish, it will exit the queue. If another process coming, it will create a queue. 

There is a buffer (each in process, so there is a system that tells os to tell buffer do blah blah blah) In real machines,
there is absolutely nothing that is called "at the same time" in hardware ( one or two nano seconds)
- when there is a systemcall, os calls atomically - queue inside os, while reading your buffer, in order for antoher buffer come in 
it needs to wait a bit. 
And then, os will take them one by one. Multicores, multiprocesses may work concurrently. 

-Os will let the parent process know that the child has been finiished or killed 


Fork example #2
- Both parent and child can continue forking
- How many times L0 printed on the screen? once! bc we have just the parent process
- How many times L1 printed on the screen? twice bc both processes executed 
- Another fork meaning both parent and child will!  How many times L2 printed on the screen? 4 times

- there is no if else.

void fork2(){
	printf("L0\n"); // one process 
	fork() // fork will create a process. So two processes now. 
	//They will continue exectuion from that same point. Both child and parent printing L0 
	printf("L1\n"); // now printed by two processes togetehr, two l1
	fork() // both parnet and child calling fork. Four processes existing on the system 
	printf("Bye\n") // four processese printing Bye
}

- The reason for unique ids for all processes is that you can use these unique ids to do a part in 
big probelm.


void fork3(){
	printf("L0\n"); // one process 
	fork() // fork will create a process. So two processes now. 
	//They will continue exectuion from that same point. Both child and parent printing L0 
	printf("L1\n"); // now printed by two processes togetehr, two l1
	fork() // both parnet and child calling fork. Four processes existing on the system 
	printf("L2\n"); //
	fork() // 8 processes 
	printf("Bye\n") // 8 processese printing Bye
}

void fork4(){
	printf("L0\n"); //L0 printed by the initial process 
	if (fork()!=0){ // fork called. 2 processes evaluating condition. Talking about the parent. O
		//only parent doing something in the braces
		//parent- child1 
		printf("L1\n"); // 
		if (fork()!=0){ // parent forking again. Three processes. parent child1, child2 
			printf("L2\n"); //initial parent printing l2 
			fork(); // fork another thing. parent child 1, 2 ,3 (4 proceeses)
		}
	}
	printf("Bye\n") //parent child 1, 2 ,3 (4 proceeses) printing bye 
	//l1,l0,l2 all printed by the parent 
}




void fork5(){
	//both parent and child can continue forking 
	printf("L0\n"); //L0 printed by the initial process 
	if (fork()==0){ // fork called. Then fork condition is equal to zero. Whatever done in braces
		//done by children. Now it is the child printing l1
		printf("L1\n"); // 
		if (fork()==0){ // child fork again.  
			printf("L2\n"); //child printing l2 
			fork(); // fork another thing. only one process will call this fork.
			//fork is not created from all processes 
		}
	}
	printf("Bye\n")  
	//l1,l2 all printed by the child
	//child will never print l0, 
}





























