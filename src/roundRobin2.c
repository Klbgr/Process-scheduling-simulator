#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "./inc/ipcTools.h"
#include "./inc/roundRobin2.h"

void roundRobin2(Process process, Stat *stats)
{

	int number = processNumber(process);

	key_t key = ftok("/etc/passwd", 0);
	int sem = semalloc(key + 1, 1);
	if (sem == -1)
	{
		perror("Semaphore failed");
		exit(0);
	}

	Cycle cycles;
	Process tab[number];
	char *name = malloc(sizeof(char) * BUFFER_SIZE);

	int duration, type, defcycle;
	int cpt = 0;
	int first = 1; // to calculate response time

	// init stat
	struct timeval creation; // creation
	struct timeval ready;	 // ready to start to process ES or CPU
	struct timeval start;	 // start to process ES or CPU
	float response;			 // response time
	float wait;				 // wait time

	tab[cpt] = process;
	name = tab[cpt]->name;
	printf("Start of %s\n", name);
	cycles = tab[cpt]->cycles;

	int flag_tab = 0;

	int all_arrived = 1;

	if (all_arrived)
	{
		for (int i = 1; i < number; i++)
		{
			cpt++;
			tab[i] = tab[i - 1]->next;
			name = tab[i]->name;
			usleep(process->arrival * 1000); // simulate late arrival
			gettimeofday(&creation, 0);		 // get time for creation
			stats[i] = initStat(name);
			printf("Start of %s\n", name);
		}
	}
	name = tab[cpt]->name;
	int result = 1;
	int cpt_type = 0;
	int find;

	while (result == 1)
	{

		if (cpt == 0)
		{										   // if there is only one process
			duration = tab[cpt]->cycles->duration; // get duration of cycle
			type = tab[cpt]->cycles->type;		   // get type of cycle
			defcycle = QUANTUM - duration;		   // know if we are in the quantum
			name = tab[cpt]->name;				   // get name of process
			if (defcycle < 0)					   // if quantum is less than duration of cycle
			{
				duration = duration - QUANTUM; // define duration in quantum
				tab[cpt]->cycles->duration = duration;
				gettimeofday(&ready, 0); // get time for start
				deftype(type, QUANTUM, name, sem, start);
				// stats
				wait = calculateTime(ready, start);						// calculate wait time in ms
				stats[cpt] = addWait(stats[cpt], wait);					// adding wait time to Stat
				stats[cpt] = addTurnaround(stats[cpt], wait + QUANTUM); // adding turnaround time to Stat
				if (first == 1)											// only if first cycle
				{
					response = calculateTime(creation, start);		// calculate response time
					stats[cpt] = addResponse(stats[cpt], response); // adding reponse time to Stat
					first = 0;
				}
				if (tab[cpt]->next != NULL && cpt < number && flag_tab != 1)
				{
					cpt++;
					tab[cpt] = tab[cpt - 1]->next;
					usleep(process->arrival * 1000); // simulate late arrival
					gettimeofday(&creation, 0);		 // get time for creation
					stats[cpt] = initStat(name);
					printf("Start of %s\n", tab[cpt]->name);
				}
			}
			else if (defcycle == 0)
			{
				gettimeofday(&ready, 0); // get time for start
				deftype(type, duration, name, sem, start);
				// stats
				wait = calculateTime(ready, start);						 // calculate wait time in ms
				stats[cpt] = addWait(stats[cpt], wait);					 // adding wait time to Stat
				stats[cpt] = addTurnaround(stats[cpt], wait + duration); // adding turnaround time to Stat
				if (first == 1)											 // only if first cycle
				{
					response = calculateTime(creation, start);		// calculate response time
					stats[cpt] = addResponse(stats[cpt], response); // adding reponse time to Stat
					first = 0;
				}
				if (type == CPU || type == ES)
				{
					tab[cpt]->cycles = tab[cpt]->cycles->next;
				}
				if (tab[cpt]->next != NULL && cpt < number && flag_tab != 1)
				{
					cpt++;
					tab[cpt] = tab[cpt - 1]->next;
					usleep(process->arrival * 1000); // simulate late arrival
					gettimeofday(&creation, 0);		 // get time for creation
					stats[cpt] = initStat(name);
					printf("Start of %s\n", tab[cpt]->name);
				}
			}
			else
			{
				gettimeofday(&ready, 0); // get time for start
				deftype(type, duration, name, sem, start);
				// stats
				wait = calculateTime(ready, start);						 // calculate wait time in ms
				stats[cpt] = addWait(stats[cpt], wait);					 // adding wait time to Stat
				stats[cpt] = addTurnaround(stats[cpt], wait + duration); // adding turnaround time to Stat
				if (first == 1)											 // only if first cycle
				{
					response = calculateTime(creation, start);		// calculate response time
					stats[cpt] = addResponse(stats[cpt], response); // adding reponse time to Stat
					first = 0;
				}
				tab[cpt]->cycles = tab[cpt]->cycles->next;

				if (tab[cpt]->next != NULL && cpt < number && flag_tab != 1)
				{
					cpt++;
					tab[cpt] = tab[cpt - 1]->next;
					usleep(process->arrival * 1000); // simulate late arrival
					gettimeofday(&creation, 0);		 // get time for creation
					stats[cpt] = initStat(name);
					printf("Start of %s\n", tab[cpt]->name);
				}
			}
			if ((flag_tab == 1 || tab[cpt]->next == NULL) && tab[cpt]->cycles == NULL)
			{
				printf("End of %s\n", name);
				result = 0;
			}
		}
		else
		{
			for (int i = 0; i <= cpt; i++)
			{
				type = tab[i]->cycles->type;
				if (type == CPU)
				{
					cpt_type++;
				}
			}
			if (cpt_type == cpt + 1)
			{
				cpt_type = 0;

				// AJOUTER TOUS EN WAIT SAUF PREMIER ICI
				for (int i = 1; i < cpt; i++)
				{
					// stats
					wait = calculateTime(ready, start);					// calculate wait time in ms
					stats[i] = addWait(stats[i], wait);					// adding wait time to Stat
					stats[i] = addTurnaround(stats[i], wait + QUANTUM); // adding turnaround time to Stat
				}

				duration = tab[0]->cycles->duration; // get duration of cycle
				type = tab[0]->cycles->type;		 // get type of cycle
				defcycle = QUANTUM - duration;		 // know if we are in the quantum
				name = tab[0]->name;				 // get name of process
				if (defcycle < 0)
				{
					duration = duration - QUANTUM; // define duration in quantum
					tab[0]->cycles->duration = duration;
					gettimeofday(&ready, 0); // get time for start
					deftype(type, QUANTUM, name, sem, start);
					// stats
					wait = calculateTime(ready, start);					// calculate wait time in ms
					stats[0] = addWait(stats[0], wait);					// adding wait time to Stat
					stats[0] = addTurnaround(stats[0], wait + QUANTUM); // adding turnaround time to Stat
					if (first == 1)										// only if first cycle
					{
						response = calculateTime(creation, start);	// calculate response time
						stats[0] = addResponse(stats[0], response); // adding reponse time to Stat
						first = 0;
					}
					if (tab[cpt]->next != NULL && cpt < number && flag_tab != 1)
					{
						cpt++;
						tab[cpt] = tab[cpt - 1]->next;
						usleep(process->arrival * 1000); // simulate late arrival
						gettimeofday(&creation, 0);		 // get time for creation
						stats[cpt] = initStat(name);
						printf("Start of %s\n", tab[cpt]->name);
					}
				}
				else if (defcycle == 0)
				{
					gettimeofday(&ready, 0);
					deftype(type, duration, name, sem, start);
					// stats
					wait = calculateTime(ready, start);					 // calculate wait time in ms
					stats[0] = addWait(stats[0], wait);					 // adding wait time to Stat
					stats[0] = addTurnaround(stats[0], wait + duration); // adding turnaround time to Stat
					if (first == 1)										 // only if first cycle
					{
						response = calculateTime(creation, start);	// calculate response time
						stats[0] = addResponse(stats[0], response); // adding reponse time to Stat
						first = 0;
					}
					if (type == CPU || type == ES)
					{
						tab[0]->cycles = tab[0]->cycles->next;
					}
					if (tab[cpt]->next != NULL && cpt < number && flag_tab != 1)
					{
						cpt++;
						tab[cpt] = tab[cpt - 1]->next;
						usleep(process->arrival * 1000); // simulate late arrival
						gettimeofday(&creation, 0);		 // get time for creation
						stats[cpt] = initStat(name);
						printf("Start of %s\n", tab[cpt]->name);
					}
				}
				else
				{
					gettimeofday(&ready, 0);
					deftype(type, duration, name, sem, start);
					// stats
					wait = calculateTime(ready, start);					 // calculate wait time in ms
					stats[0] = addWait(stats[0], wait);					 // adding wait time to Stat
					stats[0] = addTurnaround(stats[0], wait + duration); // adding turnaround time to Stat
					if (first == 1)										 // only if first cycle
					{
						response = calculateTime(creation, start);	// calculate response time
						stats[0] = addResponse(stats[0], response); // adding reponse time to Stat
						first = 0;
					}
					tab[0]->cycles = tab[0]->cycles->next;

					if (tab[cpt]->next != NULL && cpt < number && flag_tab != 1)
					{
						cpt++;
						tab[cpt] = tab[cpt - 1]->next;
						usleep(process->arrival * 1000); // simulate late arrival
						gettimeofday(&creation, 0);		 // get time for creation
						stats[cpt] = initStat(name);
						printf("Start of %s\n", tab[cpt]->name);
					}
				}
				if ((flag_tab == 1 || tab[cpt]->next == NULL) && tab[cpt]->cycles == NULL)
				{
					printf("End of %s\n", name);
					result = 0;
				}
			}
			else if (cpt_type > 1)
			{
				cpt_type = 0;
				find = 0;

				for (int i = 0; i <= cpt; i++)
				{
					type = tab[i]->cycles->type;

					if (type == CPU)
					{
						find++;
					}

					if ((type == ES) || (type == CPU && find == 1))
					{
						duration = tab[i]->cycles->duration; // get duration of cycle
						type = tab[i]->cycles->type;		 // get type of cycle
						defcycle = QUANTUM - duration;		 // know if we are in the quantum
						name = tab[i]->name;				 // get name of process
						if (defcycle < 0)					 // if quantum is less than duration of cycle
						{
							duration = duration - QUANTUM;		 // get duration of cycle
							tab[i]->cycles->duration = duration; // get type of cycle
							gettimeofday(&ready, 0);
							deftype(type, QUANTUM, name, sem, start);
							// stats
							wait = calculateTime(ready, start);					// calculate wait time in ms
							stats[i] = addWait(stats[i], wait);					// adding wait time to Stat
							stats[i] = addTurnaround(stats[i], wait + QUANTUM); // adding turnaround time to Stat
							if (first == 1)										// only if first cycle
							{
								response = calculateTime(creation, start);	// calculate response time
								stats[i] = addResponse(stats[i], response); // adding reponse time to Stat
								first = 0;
							}
							if (tab[cpt]->next != NULL && cpt < number && flag_tab != 1)
							{
								cpt++;
								tab[cpt] = tab[cpt - 1]->next;
								usleep(process->arrival * 1000); // simulate late arrival
								gettimeofday(&creation, 0);		 // get time for creation
								stats[cpt] = initStat(name);
								printf("Start of %s\n", tab[cpt]->name);
							}
						}
						else if (defcycle == 0)
						{
							gettimeofday(&ready, 0);
							deftype(type, duration, name, sem, start);
							// stats
							wait = calculateTime(ready, start);					 // calculate wait time in ms
							stats[i] = addWait(stats[i], wait);					 // adding wait time to Stat
							stats[i] = addTurnaround(stats[i], wait + duration); // adding turnaround time to Stat
							if (first == 1)										 // only if first cycle
							{
								response = calculateTime(creation, start);	// calculate response time
								stats[i] = addResponse(stats[i], response); // adding reponse time to Stat
								first = 0;
							}
							if (type == CPU || type == ES)
							{
								tab[i]->cycles = tab[i]->cycles->next;
							}
							if (tab[cpt]->next != NULL && cpt < number && flag_tab != 1)
							{
								cpt++;
								tab[cpt] = tab[cpt - 1]->next;
								usleep(process->arrival * 1000); // simulate late arrival
								gettimeofday(&creation, 0);		 // get time for creation
								stats[cpt] = initStat(name);
								printf("Start of %s\n", tab[cpt]->name);
							}
						}
						else
						{
							gettimeofday(&ready, 0);
							deftype(type, duration, name, sem, start);
							// stats
							wait = calculateTime(ready, start);					 // calculate wait time in ms
							stats[i] = addWait(stats[i], wait);					 // adding wait time to Stat
							stats[i] = addTurnaround(stats[i], wait + duration); // adding turnaround time to Stat
							if (first == 1)										 // only if first cycle
							{
								response = calculateTime(creation, start);	// calculate response time
								stats[i] = addResponse(stats[i], response); // adding reponse time to Stat
								first = 0;
							}
							tab[i]->cycles = tab[i]->cycles->next;

							if (tab[cpt]->next != NULL && cpt < number && flag_tab != 1)
							{
								cpt++;
								tab[cpt] = tab[cpt - 1]->next;
								usleep(process->arrival * 1000); // simulate late arrival
								gettimeofday(&creation, 0);		 // get time for creation
								stats[cpt] = initStat(name);
								printf("Start of %s\n", tab[cpt]->name);
							}
						}
					}
					else
					{
						// printf("Wait time here\n");
						// CONDITION WAIT ICI A METTRE POUR LES AUTRES
						// stats
						wait = calculateTime(ready, start);					// calculate wait time in ms
						stats[i] = addWait(stats[i], wait);					// adding wait time to Stat
						stats[i] = addTurnaround(stats[i], wait + QUANTUM); // adding turnaround time to Stat
					}
					if (tab[i]->cycles == NULL)
					{
						printf("End of %s\n", name);
						for (int j = 0; j < cpt; j++)
						{
							if (j == i || j >= i)
							{
								tab[j] = tab[j + 1];
							}
						}
						tab[cpt] = NULL;
						cpt--;
						number--;

						if (tab[cpt]->next != NULL)
						{
							flag_tab = 1;
						}
					}

					if ((flag_tab == 1 || tab[cpt]->next == NULL) && tab[cpt]->cycles == NULL)
					{
						printf("End of %s\n", name);
						result = 0;
					}
				}
			}

			else
			{
				cpt_type = 0;
				for (int i = 0; i <= cpt; i++)
				{
					duration = tab[i]->cycles->duration; // get duration of cycle
					type = tab[i]->cycles->type;		 // get type of cycle
					name = tab[i]->name;				 // get name of process
					defcycle = QUANTUM - duration;
					if (defcycle < 0)
					{
						duration = duration - QUANTUM;		 // get duration of cycle
						tab[i]->cycles->duration = duration; // get type of cycle
						gettimeofday(&ready, 0);
						deftype(type, QUANTUM, name, sem, start);
						// stats
						wait = calculateTime(ready, start);					// calculate wait time in ms
						stats[i] = addWait(stats[i], wait);					// adding wait time to Stat
						stats[i] = addTurnaround(stats[i], wait + QUANTUM); // adding turnaround time to Stat
						if (first == 1)										// only if first cycle
						{
							response = calculateTime(creation, start);	// calculate response time
							stats[i] = addResponse(stats[i], response); // adding reponse time to Stat
							first = 0;
						}
						if (tab[cpt]->next != NULL && cpt < number)
						{
							cpt++;
							tab[cpt] = tab[cpt - 1]->next;
							usleep(process->arrival * 1000); // simulate late arrival
							gettimeofday(&creation, 0);		 // get time for creation
							stats[cpt] = initStat(name);
							printf("Start of %s\n", tab[cpt]->name);
						}
					}
					else if (defcycle == 0)
					{
						gettimeofday(&ready, 0);
						deftype(type, duration, name, sem, start);
						// stats
						wait = calculateTime(ready, start);					 // calculate wait time in ms
						stats[i] = addWait(stats[i], wait);					 // adding wait time to Stat
						stats[i] = addTurnaround(stats[i], wait + duration); // adding turnaround time to Stat
						if (first == 1)										 // only if first cycle
						{
							response = calculateTime(creation, start);	// calculate response time
							stats[i] = addResponse(stats[i], response); // adding reponse time to Stat
							first = 0;
						}
						if (type == CPU || type == ES)
						{
							tab[i]->cycles = tab[i]->cycles->next;
						}
						if (tab[cpt]->next != NULL && cpt < number)
						{
							cpt++;
							tab[cpt] = tab[cpt - 1]->next;
							usleep(process->arrival * 1000); // simulate late arrival
							gettimeofday(&creation, 0);		 // get time for creation
							stats[cpt] = initStat(name);
							printf("Start of %s\n", tab[cpt]->name);
						}
					}
					else
					{
						gettimeofday(&ready, 0);
						deftype(type, duration, name, sem, start);
						// stats
						wait = calculateTime(ready, start);					 // calculate wait time in ms
						stats[i] = addWait(stats[i], wait);					 // adding wait time to Stat
						stats[i] = addTurnaround(stats[i], wait + duration); // adding turnaround time to Stat
						if (first == 1)										 // only if first cycle
						{
							response = calculateTime(creation, start);	// calculate response time
							stats[i] = addResponse(stats[i], response); // adding reponse time to Stat
							first = 0;
						}
						tab[i]->cycles = tab[i]->cycles->next;
						if (tab[cpt]->next != NULL && cpt < number)
						{
							cpt++;
							tab[cpt] = tab[cpt - 1]->next;
							usleep(process->arrival * 1000); // simulate late arrival
							gettimeofday(&creation, 0);		 // get time for creation
							stats[cpt] = initStat(name);
							printf("Start of %s\n", tab[cpt]->name);
						}
					}
					if (tab[i]->cycles == NULL)
					{
						printf("End of %s\n", name);
						for (int j = 0; j < cpt; j++)
						{
							if (j == i || j >= i)
							{
								tab[j] = tab[j + 1];
							}
						}
						tab[cpt] = NULL;
						cpt--;
						number--;

						if (tab[cpt]->next != NULL)
						{
							flag_tab = 1;
						}
					}
					if (tab[cpt]->next == NULL && tab[i]->cycles == NULL)
					{
						printf("End of %s\n", name);
						result = 0;
					}
				}
			}
		}
	}
	semfree(sem);
}

void deftype(int type, int duration, char *name, int sem, struct timeval start)
{
	if (type == ES)
	{
		gettimeofday(&start, 0); // get time for start
		printf("\tStart of ES of %s for %dms\n", name, duration);
		usleep(duration * 1000); // simulate
		printf("\tDone ES of %s\n", name);
	}
	else if (type == CPU)
	{
		P(sem);					 // wait for sem to start
		gettimeofday(&start, 0); // get time for start
		printf("\tStart of CPU of %s for %dms\n", name, duration);
		usleep(duration * 1000); // simulate
		printf("\tDone CPU of %s\n", name);
		V(sem); // grant another CPU to start
	}
}
