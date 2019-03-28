CC=gcc
CFLAGS=-I.

all: sequential parallelik exec

sequential : utils.o find_min_max.o utils.h find_min_max.h
	$(CC) -o sequential find_min_max.o utils.o sequential_min_max.c $(CFLAGS)

parallelik : utils.o find_min_max.o utils.h find_min_max.h
	$(CC) -o parallel utils.o find_min_max.o parallel_min_max.c $(CFLAGS)
	
parallelFILE : parallel
	$(shell ./parallel --seed 340 --array_size 10 --pnum 2 --by_files)
	
parallelPIPE : parallel
	$(shell ./parallel --seed 340 --array_size 10 --pnum 2 --timeout 10)
	
exec :
	$(CC) -o exec exec.c $(CFLAGS)
	
utils.o : utils.h
	$(CC) -o utils.o -c utils.c $(CFLAGS)

find_min_max.o : utils.h find_min_max.h
	$(CC) -o find_min_max.o -c find_min_max.c $(CFLAGS)
	
clean :
	rm utils.o find_min_max.o sequential parallel exec parallel_min_max.o sequential_min_max.o Out.txt zombie
