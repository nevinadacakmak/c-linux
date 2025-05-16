#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "printing.h"
#include "calculations.h"
#include "signals.h"

#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void block_signals() {
    ///_|> descry: function to block signals
    ///_|> returning: does not return anything, just adds SIGINT and SIGTSTP to SIG_BLOCK list
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGTSTP);
    sigprocmask(SIG_BLOCK, &mask, NULL);
}

//function to unblock signals
void unblock_signals() {
    ///_|> descry: function to block signals
    ///_|> returning: does not return anything, just adds SIGINT and SIGTSTP to SIG_UNBLOCK list
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGTSTP);
    sigprocmask(SIG_UNBLOCK, &mask, NULL);
}

int main(int argc, char* argv[]){
    ///_|> descry: main function call
    ///_|> argc: number of command line arguments
    ///_|> char* argv[]: a string, containing command line arguments.
    ///_|> returning: returns 1
    int samp = 20;
    int delay = 500000; 
    int mem = 0;
    int cp = 0;
    int core = 0;

    int numofcores = 0;

    double maxfreq = 0;

    double prev_idle = 0;
    double prev_sum = 0;

    pid_t main_pid = getpid(); // Store the main process PID

    // Command line argument parsing
    for (int i = 1; i < argc; i++)
    {
        if ((i==1) && (isdigit(*argv[i])))
        {samp = atoi(argv[i]);}

        else if ((i==2) && (isdigit(*argv[i])))
        {delay = atoi(argv[i]);}

        else if (strcmp(argv[i], "--memory") == 0)
        {mem = 1;}

        else if (strcmp(argv[i],"--cpu") == 0)
        {cp = 1;}

        else if (strcmp(argv[i],"--cores") == 0)
        {core = 1;}

        else if (strncmp(argv[i], "--samples=", 10) == 0)
        {samp = atoi(argv[i]+10);}

        else if (strncmp(argv[i], "--tdelay=", 9) == 0) 
        {delay = atoi(argv[i]+9);}

        else
        {
            printf("Problem! Skipped all cases! You have wrong input!\n"); 
            exit(1);
        }
    }

    long int* memo_util_arr = malloc((samp+2) * sizeof(long int));
    double* cpu_util_arr = malloc((samp+2) * sizeof(double));

    double w_short_cpu[4] = {0, 0, 0, 0};
    double r_short_cpu[4] = {0, 0, 0, 0};

    long int w_short_mem[2] = {0, 0};
    long int r_short_mem[2] = {0, 0};

    //set up signal handlers but only in the main process
    if (getpid() == main_pid) {
        signal(SIGINT, sigint_handler); 
        signal(SIGTSTP, sigtstp_handler);
    }

    for(int i = 1; i < samp+1; i++)
    {
        FILE* cpu_info = fopen("/proc/cpuinfo","r");
        if (cpu_info == NULL) {
            printf("/proc/cpuinfo is empty or couldn't be opened.\n");
        }

        //block signals
        block_signals();

        int p[2];
        if (pipe(p) == -1) {
            perror("Pipe failed");
            exit(1);
        }

        pid_t pid = fork();
        if (pid == -1) {
            perror("Fork failed");
            exit(1);
        }

        if (pid == 0)
        { 
            //child process - ignore signals
            signal(SIGINT, SIG_IGN);
            signal(SIGTSTP, SIG_IGN);
            
            close(p[0]);

            double* temp = calculatecpu(samp, delay, prev_sum, prev_idle);
            memcpy(w_short_cpu, temp, sizeof(w_short_cpu));

            write(p[1], w_short_cpu, sizeof(w_short_cpu));

            close(p[1]);
            exit(0);
        }
        else 
        { 
            //parent process
            close(p[1]);
            read(p[0], r_short_cpu, sizeof(r_short_cpu));
            close(p[0]);

            wait(NULL);
        }

        //unblock signals
        unblock_signals();

        cpu_util_arr[i] = r_short_cpu[3];
        cpu_util_arr[0] = r_short_cpu[0]; 
        prev_sum = r_short_cpu[1];
        prev_idle = r_short_cpu[2];

        //////////////////////////////////////////////////////////

        // Block signals again
        block_signals();

        int p2[2];
        if (pipe(p2) == -1) {
            perror("Pipe failed");
            exit(1);
        }

        pid_t pid2 = fork();
        if (pid2 == -1) {
            perror("Fork failed");
            exit(1);
        }

        if (pid2 == 0)
        { 
            // Child process
            signal(SIGINT, SIG_IGN);
            signal(SIGTSTP, SIG_IGN);
            
            close(p2[0]);

            long int* temp2 = calculatememory(samp, delay);
            memcpy(w_short_mem, temp2, sizeof(w_short_mem));

            write(p2[1], w_short_mem, sizeof(w_short_mem));

            close(p2[1]);
            exit(0);
        }
        else 
        { 
            // Parent process
            close(p2[1]);
            read(p2[0], r_short_mem, sizeof(r_short_mem));
            close(p2[0]);

            wait(NULL);
        }

        // Unblock signals
        unblock_signals();

        memo_util_arr[0] = r_short_mem[0];
        memo_util_arr[i] = r_short_mem[1];

        //////////////////////////////////////////////////////////

        // Block signals again
        block_signals();

        int p3[2];
        if (pipe(p3) == -1) {
            perror("Pipe failed");
            exit(1);
        }

        pid_t pid3 = fork();
        if (pid3 == -1) {
            perror("Fork failed");
            exit(1);
        }

        if (pid3 == 0)
        { 
            // Child process
            signal(SIGINT, SIG_IGN);
            signal(SIGTSTP, SIG_IGN);
            
            close(p3[0]);

            int temp3 = calculatenumofcores(cpu_info);
            write(p3[1], &temp3, sizeof(int));

            close(p3[1]);
            exit(0);
        }
        else 
        { 
            // Parent process
            close(p3[1]);
            read(p3[0], &numofcores, sizeof(int));
            close(p3[0]);

            wait(NULL);
        }

        // Unblock signals
        unblock_signals();

        ///////////////////////////////////////////////////////////////

        // Block signals again
        block_signals();

        int p4[2];
        if (pipe(p4) == -1) {
            perror("Pipe failed");
            exit(1);
        }

        pid_t pid4 = fork();
        if (pid4 == -1) {
            perror("Fork failed");
            exit(1);
        }

        if (pid4 == 0)
        { 
            // Child process
            signal(SIGINT, SIG_IGN);
            signal(SIGTSTP, SIG_IGN);
            
            close(p4[0]);

            double temp4 = calculatemaxfreq();
            write(p4[1], &temp4, sizeof(double));

            close(p4[1]);
            exit(0);
        }
        else 
        { 
            // Parent process
            close(p4[1]);
            read(p4[0], &maxfreq, sizeof(double));
            close(p4[0]);

            wait(NULL);
        }

        // Unblock signals
        unblock_signals();

        graph(samp, delay, mem, cp, core, numofcores, memo_util_arr, cpu_util_arr, i, maxfreq);

        fclose(cpu_info);
    }

    free(memo_util_arr);
    free(cpu_util_arr);

    return 0;
}