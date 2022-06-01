/**
 * @file readConfig.h
 * @author Antoine QIU (qiuantoine@gmail.com)
 * @brief Lecture du fichier de configuration "config.txt"
 * @date 2022-04-04
 * @copyright Copyright (c) 2022
 */

#ifndef __readConfig_h
#define __readConfig_h
#include "process.h"

/**
 * @brief Lecture du fichier de configuration "config.txt"
 * 
 * @param Process Liste de processus à remplir
 * @return Process Liste de processus
 */
Process readConfig(Process Process);

#endif