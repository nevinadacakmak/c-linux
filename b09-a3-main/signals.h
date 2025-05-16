#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void sigint_handler(int sig);

void sigtstp_handler(int sig);