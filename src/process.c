#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./inc/process.h"

Process addProcess(Process process, char name[BUFFER_SIZE], int arrival)
{
    Process p = malloc(sizeof(struct process));
    strcpy(p->name, name);
    p->arrival = arrival;
    p->cycles = NULL;
    p->next = NULL;
    if (process == NULL)
    {
        return p;
    }
    else
    {
        Process tmp = process;
        while (tmp->next != NULL)
        {
            tmp = tmp->next;
        }
        tmp->next = p;
        return process;
    }
}

Process addCycle(Process process, int position, enum type type, int duration, int priority)
{
    Process pTmp = process;
    for (int i = 0; i < position; i++)
    {
        pTmp = pTmp->next;
    }
    Cycle c = malloc(sizeof(struct cycle));
    c->type = type;
    c->duration = duration;
    c->priority = priority;
    c->next = NULL;

    if (pTmp->cycles == NULL)
    {
        pTmp->cycles = c;
    }
    else
    {
        Cycle cTmp = pTmp->cycles;
        while (cTmp->next != NULL)
        {
            cTmp = cTmp->next;
        }
        cTmp->next = c;
    }
    return process;
}

void printProcess(Process process)
{
    Cycle cycles = process->cycles;
    char *type;
    printf("Process %s\n", process->name);
    while (cycles != NULL)
    {
        if (cycles->type == ES)
        {
            type = "ES";
        }
        else
        {
            type = "CPU";
        }
        printf("Type : %s\tDuration : %d\tPriority : %d\n", type, cycles->duration, cycles->priority);
        cycles = cycles->next;
    }
}

void printProcessList(Process process)
{
    while (process != NULL)
    {
        printProcess(process);
        process = process->next;
    }
}

int processNumber(Process process)
{
    int number = 0;
    while (process != NULL)
    {
        number++;
        process = process->next;
    }
    return number;
}