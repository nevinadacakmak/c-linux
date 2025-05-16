#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

//calculating memory, core, and cpu

int calculatenumofcores(FILE* cpu_info){
    ///_|> descry: function to calculate the number of cores
    ///_|> cpu_info: a file pointer to the cpu_info file
    ///_|> returning: returns the number of cores

    //siblings in the proc/cpuinfo
    // cpu_info is the file <- /proc/stat
    int numofcores = 0;
    char line[256];

    //calculates num of cores
    for(int i = 0; i < 11; i++)
        {
            fgets(line, 256, cpu_info);
        }
    numofcores = atoi(line+11);

    return numofcores;
}

double* calculatecpu(int samp, int delay, double prev_sum, double prev_idle){ 
    ///_|> descry: function to calculate the cpu utilization for every sample
    ///_|> samp: number of samples
    ///_|> delay: int for the delay time
    ///_|> prev_sum: double for the prev_sum to calculate cpu
    ///_|> prev_idle: double for the prev_idle to calculate cpu
    ///_|> returning: returns cpu utilization value

    //maxfreq is be the first index of the array
    //second and third are prev and current.
    //forth is the cpu value

    double* short_cpu_arr = malloc(4 * sizeof(double));
    if (short_cpu_arr == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }

    FILE* stat_info = fopen("/proc/stat","r");
    if (stat_info == NULL) {
        printf("/proc/stat is empty or couldn't be opened.\n");
    }
    
    char buffer2[256];
    
    //cpu util:(sum - idle / sum) x 100 is the formula.

    char* cpu_calc = fgets(buffer2, 256, stat_info);

    double sum = 0;
    double cpu_value = 0;
    double idle = 0;
    double v1,v2,v3,v4,v5,v6,v7,v8,v9,v10;
    char bleh[32];

    sscanf(cpu_calc, "%s %le %le %le %le %le %le %le %le %le %le", bleh, &v1, &v2, &v3, &v4, &v5, &v6, &v7, &v8, &v9, &v10); //calculation for the cpu
    sum = v1 + v2 + v3 + v4 + v5 + v6 + v7 + v8 + v9 + v10;
    idle = v4;

    double sum_diff = sum - prev_sum;
    double idle_diff = idle - prev_idle;

    //usleep(100000); //more sleep can added if wanted

    if (sum_diff != 0.0) 
    {
        cpu_value = ((sum_diff - idle_diff)/(double)sum_diff)*100.0; //divide by 0 error can happen
    }
    else
    {
        cpu_value = 0.0;
    }

    short_cpu_arr[3] = cpu_value;
    short_cpu_arr[1] = sum; //new prevsum
    short_cpu_arr[2] = idle; //new previdle
    short_cpu_arr[0] = 0;

    usleep(delay); //delay
    fclose(stat_info);

    return short_cpu_arr;
}

long int* calculatememory(int samp, int delay){
    ///_|> descry: function to calculate the memory utilization for every sample
    ///_|> samp: number of samples
    ///_|> delay: int for the delay time
    ///_|> returning: returns cpu utilization value

    long int* short_memo_arr = malloc(2 * sizeof(long int));
    if (short_memo_arr == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }

    FILE* memo_info = fopen("/proc/meminfo","r");
    if (memo_info == NULL) {
        printf("/proc/meminfo is empty or couldn't be opened.\n");
    }
    
    char buffer[32];
    char buffer4[32];
    char dummy[50];
    char buffer3[32];
    char buffer2[32];
    
    //find used memo (line3 - line1 - line4)
    //memory util: total - free / total x 100
    //free: total - used - buff/cache
    //used: total - free 

    //so utilization is used - free / total x 100
    char* total_memo = fgets(buffer,32,memo_info);
    fgets(buffer2, 32, memo_info);
    char* avail_value = fgets(buffer4, 32, memo_info);
    fgets(buffer3, 32, memo_info);
    

    long int avail_memo = 0;
    long int total_value = 0;

    sscanf(total_memo,"%s %ld %s",&dummy[0],&total_value,&dummy[1]);
    sscanf(avail_value,"%s %ld %s",&dummy[2],&avail_memo,&dummy[3]);

    long int memo_util = total_value - avail_memo; //used

    usleep(delay); //delay

    fclose(memo_info);

    short_memo_arr[0] = total_value;
    short_memo_arr[1] = memo_util;

    return short_memo_arr;
    
}

double calculatemaxfreq(){
    ///_|> descry: function to calculate the maximum frequency
    ///_|> returning: returns the maximum frequency

    double maxfreq = 0;
    char buffer5[32];

    FILE* maxfreqfile = fopen("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq","r");
    if (maxfreqfile == NULL) {
        printf("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq is empty or couldn't be opened.\n");
    }

    fgets(buffer5, 32, maxfreqfile);
    maxfreq = atoi(buffer5); //finds maximum frequency

    fclose(maxfreqfile);

    return maxfreq;
}
