#include <stdio.h>
#include <string.h>
#include "./inc/stat.h"

Stat initStat(char *processName)
{
    Stat stat;
    strcpy(stat.processName, processName);
    stat.wait = 0;
    stat.turnaround = 0;
    stat.response = 0;
    return stat;
}

Stat addWait(Stat stat, float time)
{
    stat.wait += time;
    return stat;
}

Stat addTurnaround(Stat stat, float time)
{
    stat.turnaround += time;
    return stat;
}

Stat addResponse(Stat stat, float time)
{
    stat.response += time;
    return stat;
}

void printStat(Stat stat)
{
    printf("Stats for process %s :\n- Wait : %fms\n- Turnaround : %fms\n- Response : %fms\n", stat.processName, stat.wait, stat.turnaround, stat.response);
}

void printStatList(Stat *stats, int size)
{
    for (int i = 0; i < size; i++)
    {
        printStat(stats[i]);
        printf("\n");
    }
}

Stat calculateAverages(Stat *stats, int size)
{
    float totalWait = 0;
    float totalTurnaround = 0;
    float totalResponse = 0;
    for (int i = 0; i < size; i++)
    {
        totalWait += stats[i].wait;
        totalTurnaround += stats[i].turnaround;
        totalResponse += stats[i].response;
    }
    float averageWait = totalWait / size;
    float averageTurnaround = totalTurnaround / size;
    float averageResponse = totalResponse / size;
    Stat averages = initStat("Averages");
    averages = addWait(averages, averageWait);
    averages = addTurnaround(averages, averageTurnaround);
    averages = addResponse(averages, averageResponse);
    return averages;
}

float calculateTime(struct timeval start, struct timeval end)
{
    return (end.tv_usec - start.tv_usec + 1000000.0 * (end.tv_sec - start.tv_sec)) / 1000;
}