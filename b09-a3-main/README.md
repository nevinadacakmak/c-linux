# Assignment 3: System Monitoring Tool -- Concurrency & Signals

## Metadata

Author: Nevin Ada Cakmak
Date: 03/31/2025

## Introduction

For this assignment I built a tool to show the tables used by the OS to keep track of open files, pids, FDs and more. And, added handlers for ctrl-c and ctrl-z, and implementation of pipes, forks, and more.

## Description

I knew this was a major assingment, so I started by reading the assignment handout, piazza forum to read instructor posts about the assingments, and watch the demo video to come up with steps for my implementation. Inital to-do list was:
1- Modularize and fix my A1 code
2- Implement pipes for each cpu, memory, and cores calculations
3- Implement functions to handle SIGINT and SIGTSTP functions

Therefore, I solved these problems step by step (explained in ### Implementation)

## Implementation

Main logic:
Main.c has a big for loop (according to sample size). For every iteration, calculatecpu(), calculatememory(), calculatenumofcores() are called (from children), results are gotten via pipes, stored in arrays for cpu utilization, memory utilization, and passed to a graph to graph the values real-time.

Argument Parsing: The program checks the command-line arguments and assigns values accordingly. It also handles positional arguments like samples and tdelay (if present).

Memory Utilization: It uses the /proc/meminfo file to calculate the memory usage and display it in gigabytes, alongside a bar graph.

CPU Utilization: CPU usage is calculated based on the values in /proc/stat. The program compares the previous and current CPU states to determine the usage percentage and displays this value along with a graphical plot. If the value is 0, nothing is indicated on the 0 bar. In the situation of an empty line (ie no ':'s) it should be assumed 0.

Core Information and Max Frequency: The program retrieves core count and maximum CPU frequency from /proc/cpuinfo and /sys/devices/system/cpu/cpu0/cpufreq/cpuinfo, respectively. It then displays a representation of the cores.

Graphs work by printing line by line. If the percentage of the value of memo_util_arr or cpu_value_arr matches, # or : is printed, and otherwise " ".

This program uses usleep() do add the delays (with the flag tdelay and the respective variable delay). Users should be careful about which type of machines and versions of C accept usleep().

1-
Divided the entire thing into 4 modules:
main : this is the main file, it has the CLI argument parsing, and all function calls, as well as forks and pipes
calculations: this has the functions to calculate memory utilization, max cpu frequency, cpu utilization, and number of cores, so all necessary information
printing: this has the function to print the wanted graphs.
signals: this has the handler functions to handle SIGINT and SIGTSTP signals with respective signal_handler functions

Also, fixed my cpu computation logic (which was marked as INCORRECT by the TA)

2-
Initially, each of cpu, memory, and core calculation had their own and arrays to store data. Each had one function call for every sample. I used fork() and pipe() to implement those function calls and used reading-writing to pipes to pass values between function calls.

calculatecpu() returns an array of size 4. arr[0] is maxfrequency, arr[1] is previous idle value, arr[2] is previous sum value (both used for passing to cpu function and calculating utilization), and finally arr[3] is the utilization value (which is what we end up printing)

calculatemem() returns an array of size 2. arr[0] is the available memory (e.g. 32) and arr[1] is the utilization value (which is what we end up printing)

calculatenumofcores() just returns the number of cores.

calculatemaxfreq() just returns the max frequency.

Pipes were implemented for each function. Children writes to the pipe, parent reads it, then child gets killed (oh no!). Parent process does call wait() for children processes, and children processes exit() in the end of their tasks, so we do not end up in a pathological state.

3-
I created handler functions for SIGINT and SIGTSTP, sigint_handler() and sigtstp_handler(). SIGTSTP just calls SIGCONT, while SIGINT may call SIGCONT or SIGKILL to itself (which is the main process)

then used signal() in the main to complete my implementation. sigaction() can also be used. also, I preferred write() instead of printf() because printf is not a safe function for signals, according to our slides. i had a problem of both child processes and parent process calling signal() when ctrl-c or ctrl-z is pressed, so i used signal masking and SIG_IGN to prevent children processes from calling signal handler functions.

SIG_INT asks the user "do you want to stop the process? (y/n):". if pressed y, it stops. if pressed anyhting else, it continues.

SIG_TSTP just prints "ctrl-z detected, sig "... and continues afterwards. this text gets overwritten in the next iteration

## Pseudocode of the Code

MAIN:
      parse CLI arguments
      for each sample:

      fork and pipe for calcpu
      block signals
      call calccpu
      unblock signals
      store in cpu_util_arr

      fork and pipe for calcmemo
      block signals
      call calcmemory
      unblock signals
      store in memo_util_arr

      fork and pipe for calcnumofcores
      block signals
      call calcnumofcores
      unblock signals

      fork and pipe for calcmaxfreq
      block signals
      call calcmaxfreq
      unblock signals

      graph(cpu_util_arr, memo_util_arr, numofcores, index, CLI arguments)

      free allocated space
      return

BLOCKSIGNALS:

      block sigint
      block sigtstp

UNBLOCKSIGNALS:

      unblock sigint
      unblock sigtstp

GRAPH:
      if no arguments
      print everything

      if --cores
      print cores

      if --memory
      print memory graph with memo_util_arr

      if --cpu
      print cpu graph with cpu_util_arr

CALCULATIONS

      calccpu:
      go to file for cpu
      go to line and digit
      get the data with atoi(), strtok()
      return

      calcmemo:
      go to file for memory
      go to line and digit
      get the data with atoi(), strtok()
      return

      calcnumofcores:
      go to file for number of cores
      go to line and digit
      get the data with atoi()
      return

      calcmaxfreq:
      go to file for max frequency
      go to line and digit
      get the data with atoi()
      return

SIGNALS:

      sigint_handler:
      print: do you want to stop? (y/n)
      if yes, SIGKILL to ppid()
      if no, SIGCONT to ppid()

## Compilation Instructions

if not using a makefile, "gcc -Wall -Werror main.c printing.c calculations.c signals.c -lm" and then "./a.out [command line arguments]". if using the makefile, "make" or "make all" and ./myMonitoringTool [command line arguments]

Many libraries are included, mainly
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

and usual basic-level files #include <stdlib.h> #include <stdio.h> #include <string.h> #include <math.h>

My makefile has a few rules, mainly: all: creates the final executable myMonitoringTool by compiling everything

clean: cleans all .o files

help: echoes the basic to-do for the 2 rules above

Rest is, rules about each module's dependencies

Rules compile each by gcc -Werror -Wall [ all dependencies (.c)] (used variables)

Therefore, final executable myMonitoringTool is produced

Running the Program: Once compiled, the program can be executed with the following syntax:

./a.out [samples [tdelay]] [--memory] [--cpu] [--cores] [--samples=N] [--tdelay=T]

      samples: Number of times the statistics will be collected (default is 20).
      tdelay: Sampling frequency in microseconds (default is 500,000 microseconds or 0.5 seconds).
      --memory: Reports memory utilization.
      --cpu: Reports CPU utilization.
      --cores: Reports core information.
      --samples=N: Specifies the number of samples to collect (overrides the default).
      --tdelay=T: Specifies the delay between samples in microseconds (overrides the default).
      When a wrong input is given, the program just works with the default values.

## Expected Results

Memory Utilization: The program displays memory utilization in gigabytes and plots a bar graph.

CPU Utilization: The program reports CPU utilization as a percentage and displays a bar graph.

Core Information: The program displays the number of cores and their maximum frequency.

Based on given arguments, this program graphs the cpu and memory utilization, as well as prints the number of cores, all real-time. According to passed sample size and delay time. potential CLI's are:

      --memory
      --cpu
      --cores

      number for sample size
      number for delay

Examples:

To display memory, CPU, and core utilization every second with 10 samples: ./myMonitoringTool 10 1000000 --memory 

Nbr of samples: 10 --- every 1000000 microSecs (1.000 secs)

v Memory 8.91 GB
16 GB |         
      |         
      |         
      |         
      |         
      |#########
      |         
      |         
      |         
      |         
      |         
      |         
0 GB  ___________

To display only CPU utilization with 30 samples and a delay of 0.3 seconds: ./myMonitoringTool 30 300000 --cpu

Nbr of samples: 30 --- every 300000 microSecs (0.300 secs)

v CPU 10.16 %
100 % |                              
      |                              
      |                              
      |                              
      |                              
      |                              
      |                              
      |                              
      |   : :::::: : : ::: :: :: ::::
      | :: :      : : :   :  :  :    
0 %   _______________________________

To display default values: ./myMonitoringTool

Nbr of samples: 20 --- every 500000 microSecs (0.500 secs)

v Memory 8.93 GB
16 GB |                   
      |                   
      |                   
      |                   
      |                   
      |###################
      |                   
      |                   
      |                   
      |                   
      |                   
      |                   
0 GB  _____________________

v CPU 10.24 %
100 % |                    
      |                    
      |                    
      |                    
      |                    
      |                    
      |                    
      |                    
      |  ::::::::::::::: ::
      | :               :  
0 %   _____________________

Number of Cores : 20 @ 4.80 GHz

+--+ +--+ +--+ +--+
|  | |  | |  | |  |
+--+ +--+ +--+ +--+

+--+ +--+ +--+ +--+
|  | |  | |  | |  |
+--+ +--+ +--+ +--+

+--+ +--+ +--+ +--+
|  | |  | |  | |  |
+--+ +--+ +--+ +--+

+--+ +--+ +--+ +--+
|  | |  | |  | |  |
+--+ +--+ +--+ +--+

+--+ +--+ +--+ +--+
|  | |  | |  | |  |
+--+ +--+ +--+ +--+

## Test Cases

I tested this with some of the key possible arguments, including interrupting with ctrl-z and ctrl-c for each possible option. 3 examples are:

Nbr of samples: 30 --- every 1000000 microSecs (1.000 secs)

v Memory 8.34 GB
16 GB |            
      |            
      |            
      |            
      |            
      |############
      |            
      |            
      |            
      |            
      |            
      |            
0 GB  _______________________________

^C
This is ctrl c, Caught signal 2
Do you want to stop the process? (y/n): y
Stopping process...
Killed

Nbr of samples: 10 --- every 500000 microSecs (0.500 secs)

v CPU 10.33 %
100 % |          
      |          
      |          
      |          
      |          
      |          
      |          
      |          
      |  ::::: ::
      | :     :  
0 %   ___________

Nbr of samples: 10 --- every 500000 microSecs (0.500 secs)

Number of Cores : 20 @ 4.80 GHz

+--+ +--+ +--+ +--+
|  | |  | |  | |  |
+--+ +--+ +--+ +--+

+--+ +--+ +--+ +--+
|  | |  | |  | |  |
+--+ +--+ +--+ +--+

+--+ +--+ +--+ +--+
|  | |  | |  | |  |
+--+ +--+ +--+ +--+

+--+ +--+ +--+ +--+
|  | |  | |  | |  |
+--+ +--+ +--+ +--+

+--+ +--+ +--+ +--+
|  | |  | |  | |  |
+--+ +--+ +--+ +--+


## Disclaimers

The program assumes the following default values:

The program retrieves system statistics by reading the contents of files under /proc/ (like /proc/meminfo, /proc/stat, /proc/cpuinfo), and the program assumes these files are present and contain valid information.
The number of cores is determined by reading specific values from /proc/cpuinfo, and the maximum CPU frequency is read from /sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq.
The program assumes that the content of the files do not change (except the values it reads). In other words, lines, characters besides the read values, are assumed constant and used in calculations to reach to the wanted values. Also the wordings of the CLA's are also assumed to be not changing.
Just a side note that this program calculates everything and then prints necessary information, so printing just cores takes some time.

Also, pipes can be implemented on more areas, even when calling calculatememo() or calculatecpu() functions, or to graph(), but they are just implemented to get the return values from those functions.

## References

- CSCB09 course slides
- https://man7.org/linux/man-pages/man2/pipe.2.html
- https://man7.org/linux/man-pages/man7/signal.7.html
- https://man7.org/linux/man-pages/man2/signal.2.html
- https://www.youtube.com/watch?v=83M5-NPDeWs&ab_channel=JacobSorber
- https://stackoverflow.com/questions/76383580/send-data-to-another-program-in-c-with-pipes
