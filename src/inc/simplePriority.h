/**
 * @file simplePriority.h
 * @author Antoine QIU (qiuantoine@gmail.com)
 * @brief Algorithme d'ordonnancement Simple Priority
 * @date 2022-04-04
 * @copyright Copyright (c) 2022
 */

#ifndef __simplePriority_h
#define __simplePriority_h
#include "process.h"
#include "stat.h"

/**
 * @brief Algorithme d'ordonnancement Simple Priority
 * 
 * @param process Liste chaînée de processus
 * @param stats Tableau de statistiques
 */
void simplePriority(Process process, Stat *stats);

#endif