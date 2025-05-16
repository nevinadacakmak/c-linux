#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

void graph(int samp, int delay, int mem, int cp, int core, int numofcores, long int* memo_util_arr, double* cpu_value_arr, int j, double maxfreq);