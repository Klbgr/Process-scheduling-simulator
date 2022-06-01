#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "./inc/ipcTools.h"
#include "./inc/fifo.h"

void fifo(Process process, Stat *stats)
{
    int number = processNumber(process); // number of processes

    // init IPC
    key_t key = ftok("/etc/passwd", 0);
    int sem = semalloc(key + 1, 1); // semaphore to control access to CPU
    if (sem == -1)                  // check result
    {
        perror("Semaphore failed");
        exit(0);
    }

    // init process and cycles
    int pids[number];                                // array of pid of each son
    Cycle cycles;                                    // cycles of process
    char *name = malloc(sizeof(char) * BUFFER_SIZE); // name of process
    int duration;                                    // duration of cycle
    int type;                                        // type of cycle, ES or CPU
    int first = 1;                                   // to calculate response time

    // init stat
    struct timeval creation;         // creation
    struct timeval ready;            // ready to start to process ES or CPU
    struct timeval start;            // start to process ES or CPU
    float response;                  // response time
    float wait;                      // wait time
    for (int i = 0; i < number; i++) // for each process
    {
        switch (pids[i] = fork()) // creation of son
        {
        case -1: // fork failed
            perror("Fork failed");
            exit(0);
        case 0:                              // successful fork
            usleep(process->arrival * 1000); // simulate late arrival
            gettimeofday(&creation, 0);      // get time for creation
            cycles = process->cycles;        // get cycles of process
            name = process->name;            // get name of process
            stats[i] = initStat(name);       // init Stat
            printf("Start of process %s\n", name);
            while (cycles != NULL) // for each cycle
            {
                duration = cycles->duration; // get duration of cycle
                type = cycles->type;         // get type of cycle
                gettimeofday(&ready, 0);     // get time for ready
                if (type == ES)              // ES
                {
                    gettimeofday(&start, 0); // get time for start
                    printf("\tStart of ES of process %s for %dms\n", name, duration);
                    usleep(duration * 1000); // simulate
                    printf("\tDone ES of process %s\n", name);
                }
                else if (type == CPU) // CPU
                {
                    P(sem);                  // wait for sem to start
                    gettimeofday(&start, 0); // get time for start
                    printf("\tStart of CPU of process %s for %dms\n", name, duration);
                    usleep(duration * 1000); // simulate
                    printf("\tDone CPU of process %s\n", name);
                    V(sem); // grant another CPU to start
                }

                // stats
                wait = calculateTime(ready, start);                  // calculate wait time in ms
                stats[i] = addWait(stats[i], wait);                  // adding wait time to Stat
                stats[i] = addTurnaround(stats[i], wait + duration); // adding turnaround time to Stat
                if (first == 1)                                      // only if first cycle
                {
                    response = calculateTime(creation, start);  // calculate response time
                    stats[i] = addResponse(stats[i], response); // adding reponse time to Stat
                    first = 0;
                }

                cycles = cycles->next; // next cycle
            }
            printf("End of process %s\n", name);
            exit(0);
        }
        process = process->next; // next process
    }

    // waiting for sons to finish
    for (int i = 0; i < number; i++)
    {
        waitpid(pids[i], 0, 0);
    }

    // cleaning IPC
    semfree(sem);
}