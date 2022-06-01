/**
 * @file sjf.h
 * @author Antoine QIU (qiuantoine@gmail.com)
 * @brief Algorithme d'ordonnancement Shortest Job First
 * @date 2022-04-04
 * @copyright Copyright (c) 2022
 */

#ifndef __sjf_h
#define __sjf_h
#include "process.h"
#include "stat.h"

/**
 * @brief Algorithme d'ordonnancement Shortest Job First
 * 
 * @param process Liste chaînée de processus
 * @param stats Tableau de statistiques
 */
void sjf(Process process, Stat *stats);

#endif