/**
 * @file roundRobin.h
 * @author Antoine QIU (qiuantoine@gmail.com)
 * @brief Algorithme d'ordonnancement Round Robin
 * @date 2022-04-04
 * @copyright Copyright (c) 2022
 */

#ifndef __roundRobin_h
#define __roundRobin_h
#include "process.h"
#include "stat.h"

/**
 * @brief Algorithme d'ordonnancement Round Robin
 * 
 * @param process Liste chaînée de processus
 * @param stats Tableau de statistiques
 * @param quantum Quantum de temps pour chaque processus
 */
void roundRobin(Process process, Stat *stats, int quantum);

#endif