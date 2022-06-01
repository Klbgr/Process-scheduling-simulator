/**
 * @file main.c
 * @author Christian BERANGER (beranger.chritou@orange.fr)
 * @brief Programme main avec une Interface Homme-Machine en ligne de commande
 * @date 2022-04-04
 * @copyright Copyright (c) 2022
 */

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include "./inc/ipcTools.h"
#include "./inc/process.h"
#include "./inc/fifo.h"
#include "./inc/sjf.h"
#include "./inc/roundRobin.h"
#include "./inc/roundRobin2.h"
#include "./inc/simplePriority.h"
#include "./inc/readConfig.h"
#include "./inc/exportation.h"

/**
 * @brief Programme main avec une Interface Homme-Machine en ligne de commande
 * 
 * @return int Résultat
 */
int main()
{
	Process process = NULL;
	process = readConfig(process);
	key_t key = ftok("/etc/passwd", 0);
	int number = processNumber(process);
	char *name;
	Stat *stats = (Stat *)shmalloc(key, sizeof(Stat) * number); // array of Stat, one Stat for each process
	if (stats == 0)												// check result
	{
		perror("Shared memory failed");
		exit(0);
	}
	int choice;
	int quantum;
	printf(" ____________________________________________________________ \n");
	printf("| Bonjour, Veuillez sélectionner votre algorithme à tester ! |\n");
	printf("| Les options disponnibles sont les suivantes : 	     |\n");
	printf("|	1 : FIFO		|	2 : SJF		     |\n|	3 : Priorité Simple	|	4 : Round Robin	     |\n|	5 : Round Robin (Seconde version)		     |\n|	6 : Intégralité des algorithmes			     |\n");
	printf("|____________________________________________________________|\n");
	printf("| Pour quitter, vous pouvez taper : 0			     |\n");
	printf("|____________________________________________________________|\n");
	printf("Votre choix : ");
	scanf("%d", &choice);
	switch (choice)
	{
	case 1:
		printf(" ____________________________________________________________ \n");
		printf("| Vous avez lancé l'algorithme FIFO			     |\n");
		printf("|____________________________________________________________|\n");
		fifo(process, stats);
		name = "fifo.csv";
		break;
	case 2:
		printf(" ____________________________________________________________ \n");
		printf("| Vous avez lancé l'algorithme SJF			     |\n");
		printf("|____________________________________________________________|\n");
		sjf(process, stats);
		name = "sjf.csv";
		break;
	case 3:
		printf(" ____________________________________________________________ \n");
		printf("| Vous avez lancé l'algorithme PRIORITE SIMPLE		     |\n");
		printf("|____________________________________________________________|\n");
		simplePriority(process, stats);
		name = "simplePriority.csv";
		break;
	case 4:
		printf(" ____________________________________________________________ \n");
		printf("| Vous avez lancé l'algorithme ROUND ROBIN		     |\n");
		printf("| Choisissez votre QUANTUM				     |\n");
		printf("|____________________________________________________________|\n");
		printf("Quantum ? ");
		scanf("%d", &quantum);
		roundRobin(process, stats, quantum);
		name = "roundRobin.csv";
		break;
	case 5:
		printf(" ____________________________________________________________ \n");
		printf("| Vous avez lancé l'algorithme ROUND ROBIN 2		     |\n");
		printf("|____________________________________________________________|\n");
		roundRobin2(process, stats);
		name = "roundRobin2.csv";
		break;
	case 6:
		printf(" ____________________________________________________________ \n");
		printf("| Vous avez lancé l'intégralité des algorithmes		     |\n");
		printf("|____________________________________________________________|\n");
		name = "general.csv";
		existFile(name);
		printf(" ____________________________________________________________ \n");
		printf("| Algorithme FIFO					     |\n");
		printf("|____________________________________________________________|\n");
		fifo(process, stats);
		printf("\n");
		printStatList(stats, number);
		Stat averagesfifo = calculateAverages(stats, number);
		printStat(averagesfifo);
		exportCsvGeneral(name, number, stats, averagesfifo, "FIFO");
		printf(" ____________________________________________________________ \n");
		printf("| Algorithme SJF					     |\n");
		printf("|____________________________________________________________|\n");
		sjf(process, stats);
		printf("\n");
		printStatList(stats, number);
		Stat averagessjf = calculateAverages(stats, number);
		printStat(averagessjf);
		exportCsvGeneral(name, number, stats, averagessjf, "SJF");
		printf(" ____________________________________________________________ \n");
		printf("| Algorithme PRIORITE SIMPLE				     |\n");
		printf("|____________________________________________________________|\n");
		simplePriority(process, stats);
		printf("\n");
		printStatList(stats, number);
		Stat averagessp = calculateAverages(stats, number);
		printStat(averagessp);
		exportCsvGeneral(name, number, stats, averagessp, "PRIORITE SIMPLE");
		printf(" ____________________________________________________________ \n");
		printf("| Algorithme ROUND ROBIN				     |\n");
		printf("| Choisissez votre QUANTUM				     |\n");
		printf("|____________________________________________________________|\n");
		printf("Quantum ?");
		scanf("%d", &quantum);
		roundRobin(process, stats, quantum);
		printf("\n");
		printStatList(stats, number);
		Stat averagesrr = calculateAverages(stats, number);
		printStat(averagesrr);
		exportCsvGeneral(name, number, stats, averagesrr, "ROUND ROBIN");
		break;
	case 0:
		printf(" ____________________________________________________________ \n");
		printf("| Vous avez choisi de quitter :(			     |\n");
		printf("|			A Bientôt !!			     |\n");
		printf("|____________________________________________________________|\n");
		shmfree(key);
		return 0;
	default:
		break;
	}
	if (choice != 6)
	{
		printf("\n");
		printStatList(stats, number);
		Stat averages = calculateAverages(stats, number);
		printStat(averages);
		existFile(name);
		exportCsv(name, number, stats, averages);
	}
	printf(" ____________________________________________________________ \n");
	printf("| 	Vos résultats ont été exportés dans le fichier :     |\n");
	if (choice == 3)
	{
		printf("|			%s 		     |\n", name);
	}
	else
	{
		printf("|			%s 			     |\n", name);
	}
	printf("|			A Bientôt !!			     |\n");
	printf("|____________________________________________________________|\n");
	shmfree(key);
	return 0;
}
