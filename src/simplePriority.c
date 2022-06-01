#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "./inc/ipcTools.h"
#include "./inc/simplePriority.h"

void simplePriority(Process process, Stat *stats)
{
    int number = processNumber(process); // number of processes

    // init IPC
    key_t key = ftok("/etc/passwd", 0);
    int sem[number];                 // array of semaphores to control access to CPU of each processes
    for (int i = 0; i < number; i++) // populating array of semaphores
    {
        sem[i] = semalloc(key + i + 1, 0);
        if (sem[i] == -1) // check result
        {
            perror("Semaphore failed");
            exit(0);
        }
    }
    int *priorities = (int *)shmalloc(key - 1, sizeof(int) * number); // array of priorities, to indicate duration of CPU cycles that are waiting to start. -2 means process finished, -1 means no CPU is waiting, > -1 means a CPU is waiting
    if (priorities == 0)                                              // check result
    {
        perror("Shared memory failed");
        exit(0);
    }
    for (int i = 0; i < number; i++) // populating array of priorities
    {
        priorities[i] = -1;
    }

    // init process and cycles
    Cycle cycles;                                    // cycles of process
    char *name = malloc(sizeof(char) * BUFFER_SIZE); // name of process
    int duration;                                    // duration of cycle
    int priority;                                    // priority of cycle
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
        switch (fork()) // creation of son
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
                priority = cycles->priority; // get priority of cycle
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
                    priorities[i] = priority; // indicate priority of cycle to father
                    P(sem[i]);                // wait for sem to start
                    gettimeofday(&start, 0);  // get time for start
                    printf("\tStart of CPU of process %s with priority of %d for %dms\n", name, priority, duration);
                    usleep(duration * 1000); // simulate
                    priorities[i] = -1;      // indicate to father cycle finished
                    printf("\tDone CPU of process %s\n", name);
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

            priorities[i] = -2; // indicate to father process ended
            printf("End of process %s\n", name);
            exit(0);
        }
        process = process->next;
    }

    // controlling CPUs
    int running = 1; // control running
    int min;         // value of minimum
    int minIndex;    // index of minimum
    while (running == 1)
    {
        usleep(100 * 1000);              // wait 100ms to let processes getting ready
        min = 1000000000;                // reset
        minIndex = -1;                   // reset
        for (int i = 0; i < number; i++) // for each duration
        {
            if (priorities[i] > -1 && priorities[i] <= min) // get min
            {
                min = priorities[i];
                minIndex = i;
            }
        }
        if (minIndex != -1) // if min is found
        {
            V(sem[minIndex]); // grant CPU with highest priority to start
            while (priorities[minIndex] > -1)
                ; // wait for granted CPU to finish
        }

        // check if all processes finished
        running = 0;
        for (int i = 0; i < number; i++)
        {
            if (priorities[i] != -2) // at least one process not finished
            {
                running = 1;
                break;
            }
        }
    }

    // cleaning IPC
    for (int i = 0; i < number; i++)
    {
        semfree(sem[i]);
    }
    shmfree(key - 1);
}