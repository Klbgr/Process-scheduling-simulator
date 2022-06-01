/**
 * @file process.h
 * @author Antoine QIU (qiuantoine@gmail.com)
 * @brief Déclaration des structures Process et Cycle, et de leurs méthodes
 * @date 2022-04-04
 * @copyright Copyright (c) 2022
 */

#ifndef __process_h
#define __process_h
#define BUFFER_SIZE 512

/**
 * @brief Type de cycle
 */
enum type
{
    CPU = 0, /**< Cycle CPU */
    ES = 1   /**< Cyle ES */
};

/**
 * @brief Liste chaînée de cycles
 */
struct cycle
{
    enum type type;     /**< Type de cycle */
    int duration;       /**< Durée du cycle */
    int priority;       /**< Priorité du cycle */
    struct cycle *next; /**< Prochain cycle de la liste */
};

/**
 * @brief Adresse d'un cycle
 * 
 */
typedef struct cycle *Cycle;

/**
 * @brief Liste chaînée de processus
 */
struct process
{
    char name[BUFFER_SIZE]; /**< Nom du processus */
    int arrival;            /**< Temps d'arrivée du processus */
    Cycle cycles;           /**< Liste chaînée de cycles */
    struct process *next;   /**< Prochain processus de la liste */
};

/**
 * @brief Adresse d'un process
 */
typedef struct process *Process;

/**
 * @brief Ajouter un processus à la liste de processus
 *
 * @param process Liste de processus
 * @param name Nom du processus
 * @param arrival Temps d'arrivée du processus
 * @return Process Liste de processus
 */
Process addProcess(Process process, char *name, int arrival);

/**
 * @brief Ajouter un cycle à la liste de cycles d'un processus
 *
 * @param process Liste de processus
 * @param position Position du processus dans la liste
 * @param type Type de cycle
 * @param duration Durée du cycle
 * @param priority Priorité du cycle
 * @return Process Liste de processus
 */
Process addCycle(Process process, int position, enum type type, int duration, int priority);

/**
 * @brief Afficher un processus
 *
 * @param process Processus
 */
void printProcess(Process process);

/**
 * @brief Afficher tous les processus de la liste
 *
 * @param Process Liste de processus
 */
void printProcessList(Process Process);

/**
 * @brief Compte le nombre de processus dans une liste
 *
 * @param process Liste de processus
 * @return int Nombre de processus
 */
int processNumber(Process process);

#endif