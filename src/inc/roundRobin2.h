/**
 * @file roundRobin2.h
 * @author Christian BERANGER (beranger.chritou@orange.fr)
 * @brief Algorithme d'ordonnancement Round Robin
 * @date 2022-04-04
 * @copyright Copyright (c) 2022
 */

#ifndef __roundRobin2_h
#define __roundRobin2_h
#include "process.h"
#include "stat.h"
#define QUANTUM 4000

/**
 * @brief Algorithme d'ordonnancement Round Robin
 *
 * @param process Liste chaînée de processus
 * @param stats Tableau de statistiques
 */
void roundRobin2(Process process, Stat *stats);

/**
 * @brief Récupération du type du cycle du processus courant et simulation d'exécution
 *
 * @param type Type du cycle du processus
 * @param duration Durée du cycle du processus
 * @param name Nom du cycle du processus
 * @param sem Sémaphore
 * @param start Valeur du temps d'exécution de départ
 */
void deftype(int type, int duration, char *name, int sem, struct timeval start);

#endif
