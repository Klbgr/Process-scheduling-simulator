#include <stdio.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "./inc/ipcTools.h"

void *shmalloc(key_t key, int size)
{
    void *res;                        // result
    int owner = 0;                    // owner of Shared Memory
    int shmid = shmget(key, 1, 0600); // get existing Shared Memory
    if (shmid == -1)                  // failed
    {
        owner = 1;                                              // i am owner
        shmid = shmget(key, size, IPC_CREAT | IPC_EXCL | 0600); // create Shared Memory
    }
    if (shmid == -1) // failed
        return 0;
    res = shmat(shmid, 0, 0); // get adress of Shared Memory
    if (res == (void *)-1)    // failed
    {
        if (owner)                      // i am owner
            shmctl(shmid, IPC_RMID, 0); // delete Shared Memory
        return 0;
    }
    return res;
}

int shmfree(key_t key)
{
    int shmid = shmget(key, 0, 0644);  // get id of Shared Memory
    return shmctl(shmid, IPC_RMID, 0); // delete Shared Memory
}

int semalloc(key_t key, int valInit)
{
    int semid = semget(key, 1, 0); // get existing Semaphore
    if (semid == -1)               // failed
    {
        semid = semget(key, 1, IPC_CREAT | IPC_EXCL | 0600); // create Semaphore
        if (semid == -1)                                     // failed
            return -1;
        if (semctl(semid, 0, SETVAL, valInit) == -1) // failed
        {
            semctl(semid, 0, IPC_RMID, 0); // delete Semaphore
            return -1;
        }
    }
    return semid;
}

int semfree(int semid)
{
    return semctl(semid, 0, IPC_RMID, 0); // delete Semaphore
}

static struct sembuf sP = {0, -1, 0}; // remove 1
static struct sembuf sV = {0, 1, 0};  // add 1

void P(int semid)
{
    semop(semid, &sP, 1); // remove 1
}

void V(int semid)
{
    semop(semid, &sV, 1); // add 1
}