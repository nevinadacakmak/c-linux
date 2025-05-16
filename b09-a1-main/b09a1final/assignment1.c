/* 
Write a C program that will report different metrics of the utilization of a given system as described below.
The program will present this information in real-time in graphical form by displaying at each instant of time how the quantities are changing.
*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

//function for graphing to the terminal.
void graph(int samp, int delay, int mem, int cp, int core, int numofcores, long int* memo_util_arr, long int overall_value, int maxfreq, double* cpu_value_arr, int j)
{
    printf("\033[2J\n");
    printf("\033[H\n");

    printf("Nbr of samples: %d --- every %d microSecs (%.3f secs)\n", samp, delay, (float)delay/1e6);

    printf("\n");

    if(!(mem || cp || core) || (mem))//all 0 or memory 1
    {
        //MEMORY
        printf("v Memory %.2f GB\n", memo_util_arr[j] / 1e6); 

        for(int n = 12; n > 0; n--)
        {
            if (n==12)
            {
                printf("%d GB |",(int)(overall_value/1e6)); 
            }
            else
            {
                printf("      |");
            }
            for(int i = 0; i < j; i++)
            {
                    if (ceil(memo_util_arr[i]*12.0/(int)(overall_value)) == n) //how many bars does it fill?
                    {
                        printf("#");
                    }
                    else
                    {
                        printf(" ");
                    }
            }
            printf("\n");
        }
        
        printf("0 GB  ");
        for(int o = 0; o < samp+1; o++) //+1 for the _ for the axis
        {
            printf("_");
        }
        printf("\n");
        printf("\n");
    }

    if(!(mem || cp || core) || (cp))
    {
        //CPU
        printf("v CPU %.2f %%\n",cpu_value_arr[j]);
        for(int n = 10; n > 0; n--)
        {
            if (n==10)
            {
                printf("100 %% |");
            }
            else {printf("      |");}
            for(int i = 0; i < j; i++)
            {
                    if (ceil(cpu_value_arr[i]*10/100) == n) //how many bars does it fill?
                    {
                        printf(":");
                    }
                    else
                    {
                        printf(" ");
                    }
            }
            printf("\n");
        }
        printf("0 %%   ");
        for(int o = 0; o < samp+1; o++)
        {
            printf("_");
        }
        printf("\n");
        printf("\n");
    }

    if(!(mem || cp || core) || (core))
    {

        //CORES
        printf("Number of Cores : %d @ %.2f GHz\n",numofcores,(double)(maxfreq/1e6));
        while(numofcores > 0)
        {
            if (numofcores%4==0)
            {
                printf("\n");
            }

            if (numofcores >= 4)
            {
                printf("+--+ +--+ +--+ +--+\n");
                printf("|  | |  | |  | |  |\n");
                printf("+--+ +--+ +--+ +--+\n");
                numofcores -= 4;
            }
            if (numofcores == 3)
            {
                printf("+--+ +--+ +--+\n");
                printf("|  | |  | |  |\n");
                printf("+--+ +--+ +--+\n");
                numofcores -= 3;
            }
            if (numofcores == 2)
            {
                printf("+--+ +--+\n");
                printf("|  | |  |\n");
                printf("+--+ +--+\n");
                numofcores -= 2;
            }
            if (numofcores == 1) //bad code, can be simplified
            {
                printf("+--+\n");
                printf("|  |\n");
                printf("+--+\n");
                numofcores -= 1;
            }
        }
        printf("\n");
    }
}   

//main function
int main(int argc, char* argv[])
{
    int samp = 20;
    int delay = 500000; 
    int mem = 0;
    int cp = 0;
    int core = 0;

    int numofcores = 0;
    int maxfreq = 0;

    long int prev_sum= 0, prev_idle = 0; //to calculate CPU by difference

    //parsing
    //mem cp and core can appear in any order, and they are boolean
    //samp and tdelay do come first if they are used. if only one number, it is samp

    for (int i = 1; i < argc; i++)
    {
        if ((i==1) && (isdigit(*argv[i]))) //parses the CLA arguments.
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

    long int* memo_util_arr = malloc((samp+1) * sizeof(long int)); //to record values by the sample size
    double* cpu_value_arr = malloc((samp+1) * sizeof(double)); 
    if (memo_util_arr == NULL || cpu_value_arr == NULL) {
        printf("Memory allocation failed");
        exit(1);
    }

    char buffer5[32];
    char line[256];

    FILE* cpu_info = fopen("/proc/cpuinfo","r");
    FILE* maxfreqfile = fopen("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq","r");

    if (cpu_info == NULL) { //file check
        printf("/proc/cpuinfo is empty or couldn't be opened.\n");
    }

    if (maxfreqfile == NULL) {
        printf("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq is empty or couldn't be opened.\n");
    }
    
    fgets(buffer5, 32, maxfreqfile);
    maxfreq = atoi(buffer5); //finds maximum frequency

    //calculates num of cores
    for(int i = 0; i < 11; i++)
        {
            fgets(line, 256, cpu_info);
        }
    numofcores = atoi(line+11);

    for(int j = 0; j < samp; j++)
    {
        FILE* memo_info = fopen("/proc/meminfo","r");
        FILE* stat_info = fopen("/proc/stat","r");

        if (memo_info == NULL) {
            printf("/proc/meminfo is empty or couldn't be opened.\n");
        }

        if (stat_info == NULL) {
            printf("/proc/stat is empty or couldn't be opened.\n");
        }
        
        char buffer[32];
        char buffer4[32];
        char dummy[50];
        char buffer2[256];
        
        //find used memo (line3 - line1)
        char* overall_memo = fgets(buffer,32,memo_info);

        fgets(dummy,32,memo_info);
        char* used_memo = fgets(buffer4, 32, memo_info);

        long int used_value = 0;
        long int overall_value = 0;

        sscanf(overall_memo,"%s %ld %s",&dummy[0],&overall_value,&dummy[1]);
        sscanf(used_memo,"%s %ld %s",&dummy[2],&used_value,&dummy[3]);

        long int memo_util = overall_value - used_value; //ceil or floor can both be used
        memo_util_arr[j] = memo_util;

        //"CPU"
        //cpu util:(sum - idle / sum) x 100 is the formula.

        char* cpu_calc = fgets(buffer2, 256, stat_info);

        long int sum = 0;
        double cpu_value = 0;
        long int idle = 0;
        long int v1,v2,v3,v4,v5,v6,v7,v8,v9,v10;
        char bleh[32];

        sscanf(cpu_calc, "%s %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld", bleh, &v1, &v2, &v3, &v4, &v5, &v6, &v7, &v8, &v9, &v10); //calculation for the cpu
        sum = v1 + v2 + v3 + v4 + v5 + v6 + v7 + v8 + v9 + v10;
        idle = v4 + v5;

        long int sum_diff = sum - prev_sum;
        long int idle_diff = idle - prev_idle;

        //usleep(100000); //more sleep can added if wanted

        if (sum_diff != 0.0) 
        {
            cpu_value = ((sum_diff - idle_diff)/(double)sum_diff)*100.0; //divide by 0 error can happen
        }
        else
        {
            cpu_value = 0.0;
        }

        cpu_value_arr[j] = cpu_value;

        prev_sum = sum;
        prev_idle = idle;

        //"Cores"
        //siblings in the proc/cpuinfo
        // cpu_info is the file <- /proc/stat

        graph(samp, delay, mem, cp, core, numofcores, memo_util_arr, overall_value, maxfreq, cpu_value_arr, j);

        usleep(delay); //delay

        fclose(memo_info);
        fclose(stat_info);

        if(mem==0 && cp == 0 && core == 1)
        {
            j = 39;
        }
    }
    
    fclose(cpu_info);
    fclose(maxfreqfile);

    free(memo_util_arr);
    free(cpu_value_arr);

    return 0;
}
