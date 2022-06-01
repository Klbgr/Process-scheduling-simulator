# Process Scheduling Simulator
[English version](https://github.com/Klbgr/Process-scheduling-simulator/README.md)

Ce projet a été réalisé en Licence 3 Informatique, dans le cadre de l'unité d'enseignement de Systèmes d'Exploitation à CY Cergy Paris Université.

Ce guide est destiné aux développeurs, afin de leur faciliter la compréhension et la modification de ce programme.

## Compilation
La compilation de ce programme nécessite gcc, makefile. Pour la documentation, doxygen et graphviz sont requis.

Si l'un des exécutables est manquant, il faut l'installer avec la commande
```
sudo apt install nom_de_l_executable_a_installer
```
### Programme
Pour (re)compiler le programme, il faut utiliser la commande
```
make main
```
Pour lancer le programme
```
./main
```
### Documentation
Une documentation Doxygen est disponible. Une fois générée, celle-ci se trouvera dans le dossier `doxygen`. Pour la générer, il suffit d'entre la commande
```
make doxygen
```
Pour re-générer la documentation, il faut utiliser les commandes
```
make clean
make doxygen
```
### Remarque
Pour compiler le programme et la documentation en même temps, la commande suivante peut être utilisée.
```
make all
```
Pour nettoyer le répertoire, il y a la commande
```
make clean
```
Le programme à besoin d'un fichier "config.txt" valide, contenant les informations concernant les différents processus à simuler. Pour en savoir plus, veuillez vous référer au [guide utilisateur](https://github.com/Klbgr/Process-scheduling-simulator/Guide_Utilisateur.pdf).

## Structures
Afin de faciliter la manipulation des différentes données, le programme utilise un certain nombre de structures différentes. 
### Process
```c
typedef struct process *Process;
struct process
{
    char name[BUFFER_SIZE];
    int arrival;
    Cycle cycles;
    struct process *next;
};
```
Cette structure est une liste chaînée, contenant des informations concernant des processus. En effet, chaque élément de la liste contient un nom, un temps d'arrivé, des cycles (ES ou CPU) et l'adresse du prochain Process. La déclaration de celle-ci et de ses primitives se trouvent dans le fichier `src/inc/process.h`. La définition de ses primitives se trouve dans le fichier `src/process.c`.
### Cycle
```c
typedef struct cycle *Cycle;
struct cycle
{
    enum type type;
    int duration;
    int priority;
    struct cycle *next;
};
```
Tout comme la structure Process, la structure Cycle est une liste chaînée. Celle-ci comporte des informations concernant des cycles. En effet, chaque élément de la liste contient un type (ES ou CPU), une durée, une priorité et l'adresse du prochain Cycle. Étant indispensable à la structure Process, la déclaration de celle-ci et de ses primitives se trouvent au même endroit que celle de la structure Process. De même, la définition de ses primitives se trouve au même endroit que celle de la structure Process.
### Stat
```c
typedef struct
{
    char processName[BUFFER_SIZE];
    float wait;
    float turnaround;
    float response;
} Stat;
```
La structure Stat permet de récupérer des statistiques concernant un Process. En effet, celle-ci contient le nom, le temps d'attente, le temps de restitution et le temps de réponse d'un processus. La déclaration de cette structure et de ses primitives se trouve dans le fichier `src/inc/stat.h`, tandis que la définition des ces primitives se trouve dans le fichier `src/stat.c`.

## Algorithmes d'ordonnancement
Un certain nombre d'algorithmes d'ordonnancement sont disponibles. L'unité de temps utilisé dans ce programme est la milliseconde.
### First In First Out
Déclaration : `src/inc/fifo.h`

Définition : `src/fifo.c`
### Shortest Job First
Déclaration : `src/inc/sjf.h`

Définition : `src/sjf.c`
### Round Robin
Déclaration : `src/inc/roundRobin.h`

Définition : `src/roundRobin.c`
### Round Robin 2 (expérimental)
Un second algorithme Round Robin, n'utilisant ni Fork, ni Thread est présent. Cette version fonctionne partiellement et permet d'illustrer l'importance de l'utilisation de Fork ou de Thread.

Déclaration : `src/inc/roundRobin2.h`

Définition : `src/roundRobin2.c`
### Simple Priority
Déclaration : `src/inc/simplePriority.h`

Définition : `src/simplePriority.c`

## Modifier le programme
Afin de faciliter la modification du programme, le code est commenté et documenté.
### Modifier un algorithme d'ordonnancement
Après la modification d'un fichier `.c` ou d'un fichier `.h`, il faut re-compiler le programme avec la commande
```
make main
```
### Ajouter un algorithme d'ordonnancement
Pour ajouter un algorithme d'ordonnancement, il faut :
- Ajouter sa déclaration dans dans le fichier `src/inc/nom_de_l_algo.h`
- Ajouter sa définition dans le fichier `src/mon_de_l_algo.c`
- Ajouter l'appel de cet algorithme dans `src/main.c`
- Ajouter au fichier `makefile` les lignes
```
./out/nom_de_l_algo.o : ./src/nom_de_l_algo.c ./src/inc/nom_de_l_algo.h
	mkdir -p ./out
	gcc -c ./src/nom_de_l_algo.c -o ./out/nom_de_l_algo.o
```
- Ajouter `./out/nom_de_l_algo.o`  à la cible "main" du fichier `makefile`. Par exemple
```
main : ./out/readConfig.o ./out/process.o ./out/ipcTools.o ./out/fifo.o ./out/sjf.o ./out/simplePriority.o ./out/stat.o ./out/main.o ./out/nom_de_l_algo.o
```
- Re-compiler avec
```
make main
```
### Modifier la lecture du fichier "config.txt"
Pour cela, il faut modifier la définition de la fonction "readConfig" qui se trouve dans le fichier `src/readConfig.c`, et si besoin modifier le fichier de déclaration `src/inc/readConfig.h`.

## Contact
Ce projet a été réalisé par :
- [@Klbgr](https://github.com/Klbgr), Antoine QIU (qiuantoine@gmail.com)
- [@ChrisB822](https://github.com/ChrisB822), Christian BERANGER (beranger.chritou@orange.fr)