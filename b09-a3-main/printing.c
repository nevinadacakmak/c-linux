#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

//function for graphing to the terminal. 
void graph(int samp, int delay, int mem, int cp, int core, int numofcores, long int* memo_util_arr, double* cpu_value_arr, int j, double maxfreq) {
    ///_|> descry: function to graph the memory, cpu, and core utilization
    ///_|> samp: number of samples
    ///_|> delay: int for the delay time
    ///_|> mem: int for the memory flag
    ///_|> cp: int for the cpu flag
    ///_|> core: int for the core flag
    ///_|> numofcores: number of cores
    ///_|> memo_util_arr: array for the memory utilization
    ///_|> cpu_value_arr: array for the cpu utilization
    ///_|> j: int for the current sample
    ///_|> maxfreq: double for the maximum frequency
    ///_|> returning: returns nothing
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
                printf("%d GB |",(int)(memo_util_arr[0]/1e6)); //!
            }
            else
            {
                printf("      |");
            }
            for(int i = 1; i < j; i++)
            {
                    if (ceil(memo_util_arr[i]*12.0/(int)(memo_util_arr[0])) == n) //how many bars does it fill?
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

    if((!(mem || cp || core) || (core)) && (j == samp))
    {

        //CORES
        printf("Number of Cores : %d @ %.2f GHz\n",numofcores,(maxfreq/1e6));
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