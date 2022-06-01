#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "./inc/ipcTools.h"
#include "./inc/roundRobin.h"

void roundRobin(Process process, Stat *stats, int quantum)
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
    int canStart = semalloc(key - 1, 0);                          // semaphore to indicate if a CPU is currently running
    int *status = (int *)shmalloc(key - 2, sizeof(int) * number); // array of status, to indicate if CPU cycles that are waiting to start. -1 means process finished, 0 means no CPU is waiting, 1 means a CPU is waiting
    if (status == 0)                                              // check result
    {
        perror("Shared memory failed");
        exit(0);
    }
    for (int i = 0; i < number; i++) // populating array of status
    {
        status[i] = 0;
    }

    // init process and cycles
    Cycle cycles;                                    // cycles of process
    char *name = malloc(sizeof(char) * BUFFER_SIZE); // name of process
    int duration;                                    // duration of cycle
    int type;                                        // type of cycle, ES or CPU
    int first = 1;                                   // to calculate response time
    int time;                                        // time of execution of CPU
    int toRun;                                       // duration of CPU to be run

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
                type = cycles->type;         // get type of cycle
                if (type == ES)              // ES
                {
                    gettimeofday(&ready, 0); // get time for ready
                    gettimeofday(&start, 0); // get time for start
                    printf("\tStart of ES of process %s for %dms\n", name, duration);
                    usleep(duration * 1000); // simulate
                    printf("\tDone ES of process %s\n", name);

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
                }
                else if (type == CPU) // CPU
                {
                    time = 0;      // reset
                    status[i] = 1; // indicate to father cycle is waiting
                    printf("\tStart of CPU of process %s for %dms\n", name, duration);
                    while (time < duration) // while CPU not finished
                    {
                        gettimeofday(&ready, 0);       // get time for ready
                        P(sem[i]);                     // wait for sem to start
                        gettimeofday(&start, 0);       // get time for start
                        if (time + quantum > duration) // quantum is too long
                        {
                            toRun = duration - time; // remaining time
                        }
                        else
                        {
                            toRun = quantum;
                        }
                        time += toRun;
                        printf("\tRunning %dms of CPU of process %s\n", toRun, name);
                        usleep(toRun * 1000); // simulate
                        V(canStart);          // grant another CPU to continue

                        // stats
                        wait = calculateTime(ready, start);               // calculate wait time in ms
                        stats[i] = addWait(stats[i], wait);               // adding wait time to Stat
                        stats[i] = addTurnaround(stats[i], wait + toRun); // adding turnaround time to Stat
                        if (first == 1)                                   // only if first cycle
                        {
                            response = calculateTime(creation, start);  // calculate response time
                            stats[i] = addResponse(stats[i], response); // adding reponse time to Stat
                            first = 0;
                        }
                    }
                    status[i] = 0; // indicate to father cycle finished
                    printf("\tDone CPU of process %s\n", name);
                }

                cycles = cycles->next; // next cycle
            }

            status[i] = -1; // indicate to father process ended
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
        for (int i = 0; i < number; i++)
        {
            if (status[i] == 1)
            {
                V(sem[i]);
                P(canStart);
            }
        }
        // check if all processes finished
        running = 0;
        for (int i = 0; i < number; i++)
        {
            if (status[i] != -1) // at least one process not finished
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
    semfree(canStart);
    shmfree(key - 2);
}