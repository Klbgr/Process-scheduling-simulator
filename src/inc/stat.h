/**
 * @file stat.h
 * @author Antoine QIU (qiuantoine@gmail.com)
 * @brief Déclaration de la structure Stat, et de ses méthodes
 * @date 2022-04-04
 * @copyright Copyright (c) 2022
 */

#ifndef __stat_h
#define __stat_h
#include <sys/time.h>
#define BUFFER_SIZE 512

/**
 * @brief Statistiques d'un processus
 */
typedef struct
{
    char processName[BUFFER_SIZE]; /**< Nom du processus */
    float wait; /**< Temps d'attente */
    float turnaround; /**< Temps de restitution */
    float response; /**< Temps de réponse */
} Stat;

/**
 * @brief Initialise une Stat
 * 
 * @param processName Nom du processus
 * @return Stat Statistiques
 */
Stat initStat(char *processName);

/**
 * @brief Ajout de temps d'attente
 * 
 * @param stat Statistiques à modifier
 * @param time Temps d'attente à ajouter
 * @return Stat Statistiques
 */
Stat addWait(Stat stat, float time);

/**
 * @brief Ajout de temps de restitution
 * 
 * @param stat Statistiques à modifier
 * @param time Temps de restitution à ajouter
 * @return Stat Statistiques
 */
Stat addTurnaround(Stat stat, float time);

/**
 * @brief Ajout de temps de réponse
 * 
 * @param stat Statistiques à modifier
 * @param time Temps de réponse à ajouter
 * @return Stat Statistiques
 */
Stat addResponse(Stat stat, float time);

/**
 * @brief Afficher les statistiques
 * 
 * @param stat Statistiques à afficher
 */
void printStat(Stat stat);

/**
 * @brief Afficher toutes les statistiques d'une liste
 * 
 * @param stats Tableau de statistiques
 * @param size Taille de la liste
 */
void printStatList(Stat *stats, int size);

/**
 * @brief Calculer les moyennes d'une liste de statistiques
 * 
 * @param stats Liste de statistiques
 * @param size Taille de la liste
 * @return Stat Moyennes
 */
Stat calculateAverages(Stat *stats, int size);

/**
 * @brief Calculer la durée entre deux temps
 * 
 * @param start Temps de début
 * @param end Temps de fin
 * @return float Durée
 */
float calculateTime(struct timeval start, struct timeval end);

#endif