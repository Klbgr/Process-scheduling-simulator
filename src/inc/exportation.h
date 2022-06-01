/**
 * @file exportation.h
 * @author Christian BERANGER (beranger.chritou@orange.fr)
 * @brief Algorithme d'exportation en fichier csv
 * @date 2022-04-04
 * @copyright Copyright (c) 2022
 */
#ifndef __exportation_h
#define __exportation_h
#include "stat.h"
#define MAX 100

/**
 * @brief Procédure permettant de concaténer les éléments
 *
 * @param dst Destination de la concaténation et premier élément de la concaténation
 * @param src Élément à concaténer
 * @param c
 * @param n	Nombre d'élément a prendre en compte dans la variable
 */
void *concatStrings(void *restrict dst, const void *restrict src, int c, size_t n);

/**
 * @brief Procédure permettant de vérifier l'existance d'un fichier
 *
 * @param name Nom du fichier
 */
void existFile(char *name);

/**
 * @brief Procédure permettant de concaténer les éléments
 *
 * @param name Nom du fichier csv a exporter
 * @param number Nombre de processus total effectués
 * @param stats Statistiques des processus effectués
 * @param averages Moyenne des résultats obtenu par rapport aux processus effectués
 */
void exportCsv(char *name, int number, Stat *stats, Stat averages);

/**
 * @brief Procédure permettant de concaténer les éléments
 *
 * @param name Nom du fichier csv a exporter
 * @param number Nombre de processus total effectués
 * @param stats Statistiques des processus effectués
 * @param averages Moyenne des résultats obtenu par rapport aux processus effectués
 * @param algoName Nom de l'algorithme en cours
 */
void exportCsvGeneral(char *name, int number, Stat *stats, Stat averages, char *algoName);

#endif
