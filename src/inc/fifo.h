/**
 * @file fifo.h
 * @author Antoine QIU (qiuantoine@gmail.com)
 * @brief Algorithme d'ordonnancement First In First Out
 * @date 2022-04-04
 * @copyright Copyright (c) 2022
 */

#ifndef __fifo_h
#define __fifo_h
#include "process.h"
#include "stat.h"

/**
 * @brief Algorithme d'ordonnancement First In First Out
 * 
 * @param process Liste chaînée de processus
 * @param stats Tableau de statistiques
 */
void fifo(Process process, Stat *stats);

#endif