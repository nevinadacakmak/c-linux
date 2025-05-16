#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

int calculatenumofcores(FILE* cpu_info);

long int* calculatememory(int samp, int delay);

double* calculatecpu(int samp, int delay, double prev_sum, double prev_idle);

double calculatemaxfreq();