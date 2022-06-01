/**
 * @file ipcTools.h
 * @author Antoine QIU (qiuantoine@gmail.com) & Christian BERANGER (beranger.chritou@orange.fr)
 * @brief Boîte à outils IPC
 * @date 2022-04-04
 * @copyright Copyright (c) 2022
 */

#ifndef __ipcTools_h 
#define __ipcTools_h 
#include <sys/types.h>
#include <sys/ipc.h>

/**
 * @brief Allocation de Shared Memory
 * 
 * @param key Clé unique
 * @param size Taille à allouer
 * @return void* Adresse de la Shared Memory
 */
void *shmalloc(key_t key, int size);

/**
 * @brief Suppression d'une Shared Memory
 * 
 * @param key Clé unique
 * @return int Résultat
 */
int shmfree(key_t key);

/**
 * @brief Allocation d'un Sémaphore
 * 
 * @param key Clé unique
 * @param valInit Valeur initiale
 * @return int Identifiant du Sémaphore
 */
int semalloc(key_t key, int valInit);

/**
 * @brief Suppression d'un Sémaphore
 * 
 * @param semid Identifiant du Sémaphore
 * @return int Résultat
 */
int semfree(int semid);

/**
 * @brief Décrémentation du Sémaphore
 * 
 * @param semid Identifiant du Sémaphore
 */
void P(int semid);

/**
 * @brief Incrémentation du Sémaphore
 * 
 * @param semid Identifiant du Sémaphore
 */
void V(int semid);

#endif