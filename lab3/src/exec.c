#include <stdio.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <getopt.h>

int main(void)
{
        pid_t child = fork();
        if(child == 0){
            execl("sequential", "sequential", "360", "10", NULL );
        }
        else
            wait(NULL);

	       printf("Exec finished\n");
}