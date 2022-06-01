#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./inc/exportation.h"

void *concatStrings(void *restrict dst, const void *restrict src, int c, size_t n)
{
	const char *s = src;
	for (char *ret = dst; n; ++ret, ++s, --n)
	{
		*ret = *s;
		if ((unsigned char)*ret == (unsigned char)c)
			return ret + 1;
	}
	return 0;
}

void existFile(char *name)
{

	const char *str1 = "rm -f ";
	const char *str2;
	char buffer[MAX];

	FILE *fichier = fopen(name, "r+");
	if (fichier != NULL)
	{
		/* Le fichier n'existe pas */
		fclose(fichier);
		str2 = name;
		concatStrings(concatStrings(buffer, str1, '\0', MAX) - 1, str2, '\0', MAX);
		system(buffer);
	}
}

void exportCsvGeneral(char *name, int number, Stat *stats, Stat averages, char *algoName)
{
	FILE *f;
	const char *str1 = algoName;
	const char *str2 = "\n";
	char buffer[MAX];
	f = fopen(name, "a");
	if (f != NULL)
	{
		concatStrings(concatStrings(buffer, str1, '\0', MAX) - 1, str2, '\0', MAX);
		fwrite(buffer, 1, strlen(buffer), f);
		fclose(f);
	}
	else
	{
		perror("erreur d'ouverture");
	}

	exportCsv(name, number, stats, averages);
}

void exportCsv(char *name, int number, Stat *stats, Stat averages)
{
	FILE *f;
	const char *str1 = "gedit ";
	const char *str2 = name;
	const char *str3 = " &";
	char buffer[MAX];

	f = fopen(name, "a");
	if (f != NULL)
	{
		char *title = "Stats for process ; Wait ; Turnaround ; Response \n";
		fwrite(title, 1, strlen(title), f);
		char *value = malloc(10 * sizeof(float) + 10);

		for (int i = 0; i < number; i++)
		{
			sprintf(value, "%s ; %f ; %f ; %f \n", stats[i].processName, stats[i].wait, stats[i].turnaround, stats[i].response);
			fwrite(value, 1, strlen(value), f);
		}

		sprintf(value, "%s ; %f ; %f ; %f \n", averages.processName, averages.wait, averages.turnaround, averages.response);
		fwrite(value, 1, strlen(value), f);

		fclose(f);

		concatStrings(concatStrings(buffer, str1, '\0', MAX) - 1, str2, '\0', MAX);
		concatStrings(concatStrings(buffer, buffer, '\0', MAX) - 1, str3, '\0', MAX);
		// system(buffer);
	}
	else
	{
		perror("Erreur d'ouverture");
	}
}
