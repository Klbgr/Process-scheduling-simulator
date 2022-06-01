# Process Scheduling Simulator
[French version](https://github.com/Klbgr/Process-scheduling-simulator/README_Fr.md)

This project was made during our third year of Licence Informatique, as part of the Systèmes d'Exploitation teaching unit at CY Cergy Paris Université.

This guide is intended for developers, to make it easier for them to understand and modify this program.

## Compilation
Compiling this program requires gcc, makefile. For documentation, doxygen and graphviz are required.

If one of the executables is missing, it must be installed with the command
```
sudo apt install name_of_executable_to_install
```
### Program
Pour (re)compiler le programme, il faut utiliser la commande
```
make main
```
Pour lancer le programme
```
./main
```
### Documentation
Doxygen documentation is available. Once generated, it will be in the `doxygen` folder. To generate it, just enter the command
```
make doxygen
```
To re-generate the documentation, use the commands
```
make clean
make doxygen
```
### Remark
To compile the program and the documentation at the same time, the following command can be used.
```
make all
```
To clean the directory, there is the command
```
make clean
```
The program needs a valid "config.txt" file, containing information about the different processes to simulate. For more information, please refer to the [user guide](https://github.com/Klbgr/Process-scheduling-simulator/User_Guide.pdf).

## Structures
In order to facilitate the manipulation of the different data, the program uses a number of different structures.
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
This structure is a linked list, containing information about processes. Indeed, each element of the list contains a name, an arrival time, cycles (ES or CPU) and the address of the next Process. The declaration of this one and its primitives are in the file `src/inc/process.h`. The definition of its primitives is in the file `src/process.c`.
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
Like the Process structure, the Cycle structure is a linked list. This contains information about cycles. Indeed, each element of the list contains a type (ES or CPU), a duration, a priority and the address of the next Cycle. Being essential to the Process structure, the declaration of this one and its primitives are in the same place as that of the Process structure. Similarly, the definition of its primitives is in the same place as that of the Process structure.
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
The Stat structure is used to retrieve statistics concerning a Process. Indeed, this one contains the name, the waiting time, the restitution time and the response time of a process. The declaration of this structure and its primitives is in the file `src/inc/stat.h`, while the definition of these primitives is in the file `src/stat.c`.

## Scheduling algorithms
A number of scheduling algorithms are available. The unit of time used in this program is the millisecond.
### First In First Out
Declaration : `src/inc/fifo.h`

Definition : `src/fifo.c`
### Shortest Job First
Declaration : `src/inc/sjf.h`

Definition : `src/sjf.c`
### Round Robin
Declaration : `src/inc/roundRobin.h`

Definition : `src/roundRobin.c`
### Round Robin 2 (experimental)
A second Round Robin algorithm, using neither Fork nor Thread is present. This version partially works and helps to illustrate the importance of using Fork or Thread.

Declaration : `src/inc/roundRobin2.h`

Definition : `src/roundRobin2.c`
### Simple Priority
Declaration : `src/inc/simplePriority.h`

Definition : `src/simplePriority.c`

## Modify the program
In order to facilitate the modification of the program, the code is commented and documented.
### Modify a scheduling algorithm
After modifying a `.c` file or a `.h` file, the program must be re-compiled with the command
```
make main
```
### Adding a scheduling algorithm
To add a scheduling algorithm, you must:
- Add its declaration in the file `src/inc/name_of_algorithm.h`
- Add its definition in the file `src/name_of_algorithm.c`
- Add the call of this algorithm in `src/main.c`
- Add to the `makefile` the lines
```
./out/name_of_algorithm.o : ./src/name_of_algorithm.c ./src/inc/name_of_algorithm.h
	mkdir -p ./out
	gcc -c ./src/name_of_algorithm.c -o ./out/name_of_algorithm.o
```
- Add `./out/name_of_algorithm.o` to the "main" target of the `makefile` file. For instance
```
main : ./out/readConfig.o ./out/process.o ./out/ipcTools.o ./out/fifo.o ./out/sjf.o ./out/simplePriority.o ./out/stat.o ./out/main.o ./out/name_of_algorithm.o
```
- Re-compile with
```
make main
```
### Modify the reading of the "config.txt" file
To do this, modify the definition of the "readConfig" function found in the `src/readConfig.c` file, and if necessary modify the `src/inc/readConfig.h` declaration file.

## Contact
This project was made by:
- [@Klbgr](https://github.com/Klbgr), Antoine QIU (qiuantoine@gmail.com)
- [@ChrisB822](https://github.com/ChrisB822), Christian BERANGER (beranger.chritou@orange.fr)