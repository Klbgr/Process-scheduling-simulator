#include <stdio.h>
#include <stdlib.h>
#include "./inc/readConfig.h"

Process readConfig(Process process)
{
    FILE *file = fopen("config.txt", "r");
    if (file == NULL)
    {
        perror("File \"config.txt\" not found");
        exit(0);
    }

    int processNumber;
    if (fscanf(file, "NUMBER OF PROCESS %d\n", &processNumber) != 1)
    {
        fclose(file);
        perror("Parsing error");
        exit(0);
    }

    char processName[BUFFER_SIZE][3];
    int arrival, duration[3], priority[3], count = 0, running = 1;
    while (running)
    {
        if (fscanf(file, "\nPROCESS %s %d\n", (char *)processName + count * BUFFER_SIZE, &arrival) == 2)
        {
            process = addProcess(process, (char *)processName + count * BUFFER_SIZE, arrival);
            count++;
        }
        else if (fscanf(file, "ES %d %d\n", &duration[count - 1], &priority[count - 1]) == 2)
        {
            process = addCycle(process, count - 1, ES, duration[count - 1], priority[count - 1]);
        }
        else if (fscanf(file, "CPU %d %d\n", &duration[count - 1], &priority[count - 1]) == 2)
        {
            process = addCycle(process, count - 1, CPU, duration[count - 1], priority[count - 1]);
        }
        else
        {
            fclose(file);
            running = 0;
        }
    }
    if (count != processNumber)
    {
        perror("Parsing error");
        exit(0);
    }
    return process;
}