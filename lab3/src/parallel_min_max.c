#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <getopt.h>

#include "find_min_max.h"
#include "utils.h"

int main(int argc, char **argv) {
  int seed = -1;
  int array_size = -1;
  int pnum = -1;
  bool with_files = false;

  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"seed", required_argument, 0, 0},
                                      {"array_size", required_argument, 0, 0},
                                      {"pnum", required_argument, 0, 0},
                                      {"by_files", no_argument, 0, 'f'},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "f", options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            seed = atoi(optarg);
            if (seed <= 0) {
                printf("seed must be a positive number\n");
                return 1;
              }
            break;
          case 1:
            array_size = atoi(optarg);
             if (array_size <= 0) {
                printf("array_size must be a positive number\n");
                return 1;
              }
            break;
          case 2:
            pnum = atoi(optarg);
             if (pnum <= 0) {
                printf("pnum must be a positive number\n");
                return 1;
              }
            break;
          case 3:
            with_files = true;
            break;

          defalut:
            printf("Index %d is out of options\n", option_index);
        }
        break;
      case 'f':
        with_files = true;
        break;

      case '?':
        break;

      default:
        printf("getopt returned character code 0%o?\n", c);
    }
  }
  if (optind < argc) {
    printf("Has at least one no option argument\n");
    return 1;
  }

  if (seed == -1 || array_size == -1 || pnum == -1) {
    printf("Usage: %s --seed \"num\" --array_size \"num\" --pnum \"num\" \n",
           argv[0]);
    return 1;
  }

  int *array = malloc(sizeof(int) * array_size);
  GenerateArray(array, array_size, seed);
  //print array
  printf("\n\nARRAY:\n");
  for(int i = 0; i < array_size; i++)
    printf("%i ", array[i]);
  printf("\n\n");
  int active_child_processes = 0;

  struct timeval start_time;
  gettimeofday(&start_time, NULL);

  //pipe
  int pipefd[2];
  pid_t currentPID;
  pipe(pipefd);
  int array_piece = array_size / pnum > 0 ? array_size / pnum : 1;

  for (int i = 0; i < pnum; i++) {
    pid_t child_pid = fork();
    
    
    currentPID = child_pid;
    if (child_pid >= 0) {
      // successful fork
      active_child_processes += 1;
      if (child_pid == 0) {
        // child process
        struct MinMax min_max;
        
        int begin = i * array_piece < array_size ? i * array_piece : array_size;
        int end = (i + 1) * array_piece < array_size ? (i + 1) * array_piece : array_size;
        
        if (begin == array_size) {
            min_max = GetMinMax(array, 0, 1);
        } else {
            min_max = GetMinMax(array, begin, end);
        }// parallel somehow
        printf("\nPARENT PID: %d, THIS PID: %d, CHILD PID: %d # min: %i, max: %i\n---------------------------------\n", getppid(), getpid(), currentPID, min_max.min, min_max.max);
       
        if (with_files) {
          // use files here
          FILE* fp = fopen("Out.txt", "a");
          fwrite(&min_max, sizeof(struct MinMax), 1, fp);
          fclose(fp);
        } else {
          // use pipe here
          write(pipefd[1],&min_max,sizeof(struct MinMax));
        }
        return 0;
      }

    } else {
      printf("Fork failed!\n");
      return 1;
    }
  }

  while (active_child_processes > 0) {
    // your code here
    close(pipefd[1]);
    wait(NULL);
    
    active_child_processes -= 1;
  }

  struct MinMax min_max;
  min_max.min = INT_MAX;
  min_max.max = INT_MIN;

  printf("//FROM ");
  printf(with_files ? "FILE" : "PIPE");
  for (int i = 0; i < pnum; i++) {
    struct MinMax buff;
    
    if (with_files) {
      // read from files
        FILE* fp = fopen("Out.txt", "rb");
       // printf("\nBYTE FILE POS: %i, FILE POINTER: %p" COLOR _NC, i*sizeof(struct MinMax),*fp);
        fseek(fp, i*sizeof(struct MinMax), SEEK_SET);
        fread(&buff, sizeof(struct MinMax), 1, fp);
        printf("\nPNUM #%i : min: %i  max: %i",i+1,buff.min, buff.max);
        fclose(fp);
    } else {
      // read from pipes
      read(pipefd[0], &buff, sizeof(struct MinMax));
      printf("\nPNUM #%i :min:%i  max: %i",i+1,buff.min, buff.max);
    }

    if (buff.min < min_max.min) min_max.min = buff.min;
    if (buff.max > min_max.max) min_max.max = buff.max;
  }

  struct timeval finish_time;
  gettimeofday(&finish_time, NULL);

  double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
  elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

  free(array);

  printf("\n\n\nRESULT");
  printf("\nMin: %i\n", min_max.min);
  printf("Max: %i\n", min_max.max);
  printf("Elapsed time: %fms\n", elapsed_time);
  fflush(NULL);
  return 0;
}
