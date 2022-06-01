all : main doxygen
./out/readConfig.o : ./src/readConfig.c ./src/inc/readConfig.h
	mkdir -p ./out
	gcc -c ./src/readConfig.c -o ./out/readConfig.o
./out/exportation.o : ./src/exportation.c ./src/inc/exportation.h
	mkdir -p ./out
	gcc -c ./src/exportation.c -o ./out/exportation.o
./out/process.o : ./src/process.c ./src/inc/process.h
	mkdir -p ./out
	gcc -c ./src/process.c -o ./out/process.o
./out/ipcTools.o : ./src/ipcTools.c ./src/inc/ipcTools.h
	mkdir -p ./out
	gcc -c ./src/ipcTools.c -o ./out/ipcTools.o
./out/fifo.o : ./src/fifo.c ./src/inc/fifo.h
	mkdir -p ./out
	gcc -c ./src/fifo.c -o ./out/fifo.o
./out/sjf.o : ./src/sjf.c ./src/inc/sjf.h
	mkdir -p ./out
	gcc -c ./src/sjf.c -o ./out/sjf.o
./out/roundRobin.o : ./src/roundRobin.c ./src/inc/roundRobin.h
	mkdir -p ./out
	gcc -c ./src/roundRobin.c -o ./out/roundRobin.o
./out/roundRobin2.o : ./src/roundRobin2.c ./src/inc/roundRobin2.h
	mkdir -p ./out
	gcc -c ./src/roundRobin2.c -o ./out/roundRobin2.o
./out/simplePriority.o : ./src/simplePriority.c ./src/inc/simplePriority.h
	mkdir -p ./out
	gcc -c ./src/simplePriority.c -o ./out/simplePriority.o
./out/stat.o : ./src/stat.c ./src/inc/stat.h
	mkdir -p ./out
	gcc -c ./src/stat.c -o ./out/stat.o
./out/main.o : ./src/main.c
	mkdir -p ./out
	gcc -c ./src/main.c -o ./out/main.o
main : ./out/readConfig.o ./out/exportation.o ./out/process.o ./out/ipcTools.o ./out/fifo.o ./out/sjf.o ./out/roundRobin.o ./out/roundRobin2.o ./out/simplePriority.o ./out/stat.o ./out/main.o
	gcc ./out/*.o -o main 
doxygen :
	doxygen ./generate-doxygen
clean : 
	rm -rf ./out
	rm -rf ./doxygen