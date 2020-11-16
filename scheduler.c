#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0
#define MAX_process 1000
#define RUNNING 0
#define BLOCKED 1
#define READY 2 
#define TERMINATED 3
#define NEW 4

typedef struct structure_process_data {
    int state;
    int index, cputime, iotime;
    int remaining_cputime, remaining_iotime;
    int arrival_time, last_start_time;
    int finish_time, start_time;
    struct structure_process_data* prev, * next; //pointers for linked list 
}process_data;

void start_process_data(process_data* target) { 
    target->state = NEW;
    target->cputime = 0;
    target->iotime = 0;
    target->remaining_cputime = 0;
    target->remaining_iotime = 0;
    target->arrival_time = 0;
    target->last_start_time = -1;
    target->finish_time = -1;
    target->start_time = -1;
    target->prev = target->next = target;
} //process queue starts the chosen process as the head of the linked list 

//initializing variables 
process_data process[MAX_process];  
int process_cnt = 0; //process_counter 
process_data* process_queue = NULL; //pointer for process_queue
int ready_cnt = 0; //count for the ready process_dataes
int cpu_processing_time = 0; //current cpu time time
int term_count = 0; //counter for the term
int cur_time = 0; //current
int time_quantum = 2; //tq 

void read_input_file(char* file_name) {
    FILE* fp = fopen(file_name, "r"); 
    fscanf(fp, "%d", &process_cnt); //first line as the process_data count
    for (int i = 0; i < process_cnt; i++) {
        start_process_data(process + i);
        fscanf(fp, "%d %d %d %d", &process[i].index, &process[i].cputime, &process[i].iotime, &process[i].arrival_time);
        if (process[i].cputime % 2 != 0) {
            process[i].cputime++;
        } // since we use integers and no floating points, rounded to the follwing cycle 
        process[i].remaining_cputime = process[i].cputime;
        process[i].remaining_iotime = process[i].iotime;
    }
}

//add process, meaning add ready or running process_dataes to the queue using pointers
void add_process(process_data** que, process_data* process) {
    ready_cnt++;
    process->state = READY;
    if (*que == NULL) {
        *que = process;
        return;
    }
    else {
        process->next = *que;
        process->prev = (*que)->prev;
        (*que)->prev->next = process;
        (*que)->prev = process;
        return;
    }
}

process_data* delete_process(process_data* prev, process_data* target, process_data* next) {
    ready_cnt--;
    if (ready_cnt == 1) {
        process_queue->prev = process_queue->next = process_queue;
    }
    if (target == prev && target == next) {
        return NULL;
    }
    prev->next = next;
    next->prev = prev;
    return next;
}


/*
    SCHEUDLING ALGORITHMS
*/

process_data* fcfs() { 
    if (process_queue != NULL && process_queue->remaining_cputime == 0) {
        term_count++; 
        process_queue->state = TERMINATED;
        process_queue = delete_process(process_queue->prev, process_queue, process_queue->next);
    }
    return process_queue;
}

process_data* rr() {
    //checking the previous process 
    if (process_queue != NULL && process_queue->remaining_cputime == 0) {
        term_count++;
        process_queue->state = TERMINATED;
        process_queue = delete_process(process_queue->prev, process_queue, process_queue->next);
        if (process_queue != NULL) process_queue->last_start_time = cur_time;
        return process_queue;
    }

    // checking the usage of time_quantum 
    if (process_queue != NULL && cur_time - process_queue->last_start_time == time_quantum) {
        process_queue->last_start_time = -1;
        if (process_queue->remaining_iotime > 0 && process_queue->remaining_cputime <= process_queue->cputime / 2) {
            //if time quantime used all and it is time for i/o time 
            process_queue->state = BLOCKED; //block the process_data and allow the other process_data to use the cpu
            process_queue->arrival_time = process_queue->remaining_iotime + cur_time; 
            process_queue = delete_process(process_queue->prev, process_queue, process_queue->next);
            if (process_queue != NULL) 
                process_queue->last_start_time = cur_time;
            return process_queue;
        }
        // used up all the time quantum but not the time for i/o 
        process_queue->state = READY; 
        process_queue = process_queue->next;
        if (process_queue != NULL) process_queue->last_start_time = cur_time;
        return process_queue;
    }


    if (process_queue != NULL && process_queue->last_start_time == -1) process_queue->last_start_time = cur_time;
    return process_queue;
}

process_data* sjf() { 
    if (process_queue == NULL) return process_queue;
    if (process_queue->remaining_cputime == 0) {
        process_queue->state = TERMINATED; //terminate when the last scehduled process is finished 
        process_queue = delete_process(process_queue->prev, process_queue, process_queue->next);
        term_count++;
    }
    //JUST RANDOM BIG NUMBER
    int ref = 1000; // it is needed to find the least time remaining process but since for the initial comparision, it is just set as big random number. 
    process_data* ret = NULL;

    for (int i = process_cnt - 1; i >= 0; i--) { //when the remaining time is the same, the smaller index gets the priority 
    // so iterate in the opposite direction 
        if (process[i].state == READY || process[i].state == RUNNING) {
            if (process[i].remaining_cputime <= ref) ret = process + i;
            // remaining_cputime is used because for a process_data, cpu used in "cpu/2 > io > cpu/2"  way 
        }
    }
    if (process_queue != ret) {  //to give the other process the use of cpu, then it needs 
    // to check if the previous process is requested block, running 
        if (process_queue->remaining_iotime > 0 && process_queue->remaining_cputime <= process_queue->cputime / 2) {
            //when previous blocked 
            process_queue->arrival_time = process_queue->remaining_iotime + cur_time;
            process_queue->state = BLOCKED;
        }
        else process_queue->state = READY; //when previous process was in the state of running 
        process_queue = ret;
    }
    return process_queue;
}

process_data* (*SCHEDULING)();
int run_process(process_data* tprocess) {
    if (tprocess == NULL) return NULL;
    // using the first cpu_time/2
    if (tprocess->remaining_cputime > tprocess->cputime / 2) { 
        tprocess->state = RUNNING;
        tprocess->remaining_cputime--;

        cpu_processing_time++;
        return FALSE;
    }
    // after i/o time, using the last cpu time/2 
    else if (tprocess->remaining_iotime == 0 && tprocess->remaining_cputime <= tprocess->cputime / 2) { 
        //remaining i/o time meaning that the i/o process has been finished 
        //tprocess->remaining_cputime <= tprocess->cputime / 2 meaning last half cpu time left 
        // meaning that the process still needs cpu computation 
        tprocess->state = RUNNING;
        tprocess->remaining_cputime--;

        if (tprocess->remaining_cputime == 0) {
            tprocess->finish_time = cur_time;
        }

        cpu_processing_time++;
        return FALSE;
    }
    //i/o time 
    else if (tprocess->remaining_iotime > 0) {
        tprocess->state = BLOCKED;
        //setting the time for re-arrival 
        tprocess->arrival_time = cur_time + tprocess->iotime;

        process_queue = delete_process(tprocess->prev, tprocess, tprocess->next);
        return TRUE; // again asking for scheduling 
    }
    return FALSE;
}

void print_state(char* filename, int sched_t) {
    char fname[1024] = { 0 };
    int len = strlen(filename);
    for(int i=0;i<len;i++){
        if(filename[i] == '.'){
            filename[i] = '\0';
            break;
        }
    }
    sprintf(fname, "%s_%d.txt", filename, sched_t);
    FILE* fp = fopen(fname, "a+");
    fprintf(fp, "%d ", cur_time);
    for (int i = 0; i < process_cnt; i++) {
        if (process[i].state == TERMINATED || process[i].state == NEW) continue;
        fprintf(fp, "%d:", i);
        if (process[i].state == RUNNING){
            fprintf(fp, "running ");
        } else if (process[i].state == BLOCKED) {
            fprintf(fp, "blocked ");
        } else if (process[i].state == READY) {
            fprintf(fp, "ready   ");
        }
    }

    fprintf(fp, "\n");
    fclose(fp);
}

void start() {
    process_queue = NULL;
    ready_cnt = 0;
    cpu_processing_time = 0;
    term_count = 0;
    cur_time = 0;
}

int scheduling_i;
int main(int argc, char* argv[]) {
    if(argc == 3){scheduling_i = atoi(argv[2]);}
    start();
    read_input_file(argv[1]);
    switch (scheduling_i) {
        case 0:
            SCHEDULING = fcfs;
            break;
        case 1:
            SCHEDULING = rr;
            break;
        case 2:
            SCHEDULING = sjf;
            break;
    }

    process_data* cur_process;
    while (1) {
        for (int i = 0; i < process_cnt; i++) {
            if (process[i].state == BLOCKED) process[i].remaining_iotime--;
            if (process[i].arrival_time == cur_time) {
                if (process[i].start_time == -1) process[i].start_time = cur_time;
                add_process(&process_queue, process + i);
            }
        }
        //how the run process computes the process 
        RESCHEDULE: 
        cur_process = SCHEDULING(); //asks for scheduling 
        if (term_count == process_cnt) break; //done with the given processs ! 
        if (run_process(cur_process)) goto RESCHEDULE; //current process asks for i/o and the other process_data gets the cpu > interrupt  (swtich process! )
        print_state(argv[1], scheduling_i);
        cur_time++;
    }

    // for creating output file in formatted style 
    char argv_cpy[1024] = { 0 };
    char fname[1024] = { 0 };
    strcpy(argv_cpy,argv[1]);
    int len = strlen(argv_cpy);
    for(int i=0;i<len;i++) {
        if (argv_cpy[i] == '.') {
            argv_cpy[i] = '\0';
            break;
        }
    }

    sprintf(fname, "%s_%d.txt", argv_cpy, scheduling_i);
    FILE* fp = fopen(fname, "a+");
    fprintf(fp, "\n Finishing time: %d\n", cur_time - 1);
    fprintf(fp, "CPU utilization: %.2f\n", (double)cpu_processing_time / (cur_time));
    for (int i = 0; i < process_cnt; i++) {
        fprintf(fp, "Turnaround process_data%d: %d\n", i, process[i].finish_time - process[i].start_time + 1);
    }
    fprintf(fp, "\n");
    fclose(fp);
    return 0;
}
