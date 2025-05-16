# b09-a1

## Assignment 1 for CSCB09 - Winter 2025

- an overview of the functions (including documentation)

This program monitors the system’s memory, CPU, and core utilization metrics. It collects and reports real-time system statistics with graphical output, continuously displaying the changing values over a defined number of periods.

It is targeted to run on a Linux-based system, and it outputs useful performance data such as memory usage, CPU utilization, and core information.

### Features

- Memory Utilization: Displays memory usage in gigabytes and shows a graphical plot of memory utilization.
- CPU Utilization: Reports CPU usage percentage and displays a graphical plot of CPU utilization.
- Core Information: Displays the number of CPU cores and their maximum frequency, along with a graphical diagram representing the cores.

### Functions

1. graph():

   - This function is responsible for generating and displaying graphical output.
   - It accepts parameters to decide which metrics to display (memory, CPU, or core) and the number of samples and their respective frequency.
   - It also creates graphical bars to represent memory and CPU utilization.

2. main():
   - The main driver of the program. It handles parsing the command-line arguments, managing system resources, and calling necessary functions.
   - It reads system files (such as `/proc/meminfo`, `/proc/stat`, `/proc/cpuinfo`) to retrieve information about system memory, CPU, and cores.
   - It continuously collects these metrics at specified intervals and calls the `graph()` function to display the results.

### How to Run the Program

1. Compilation: To compile the program, run the following command in the terminal:

   gcc assignment1.c -lm

   -lm should be indicated since the program includes the math.h library. used libraries are:
   <stdio.h>
   <ctype.h>
   <string.h>
   <stdlib.h>
   <unistd.h>
   <math.h>

2. Running the Program: Once compiled, the program can be executed with the following syntax:

   ./a.out [samples [tdelay]] [--memory] [--cpu] [--cores] [--samples=N] [--tdelay=T]

   - `samples`: Number of times the statistics will be collected (default is 20).
   - `tdelay`: Sampling frequency in microseconds (default is 500,000 microseconds or 0.5 seconds).
   - `--memory`: Reports memory utilization.
   - `--cpu`: Reports CPU utilization.
   - `--cores`: Reports core information.
   - `--samples=N`: Specifies the number of samples to collect (overrides the default).
   - `--tdelay=T`: Specifies the delay between samples in microseconds (overrides the default).

   When a wrong input is given, the program just works with the default values.

   Examples:

   - To display memory, CPU, and core utilization every second with 10 samples:
     ./a.out 10 1000000 --memory --cpu --cores
   - To display only CPU utilization with 30 samples and a delay of 0.3 seconds:
     ./a.out 30 300000 --cpu

### Assumptions

- The program assumes the following default values:
- The program retrieves system statistics by reading the contents of files under `/proc/` (like `/proc/meminfo`, `/proc/stat`, `/proc/cpuinfo`), and the program assumes these files are present and contain valid information.
- The number of cores is determined by reading specific values from `/proc/cpuinfo`, and the maximum CPU frequency is read from `/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq`.
- The program assumes that the content of the files do not change (except the values it reads). In other words, lines, characters besides the read values, are assumed constant and used in calculations to reach to the wanted values. Also the wordings of the CLA's are also assumed to be not changing.

### Structure

1. Argument Parsing:
   The program checks the command-line arguments and assigns values accordingly. It also handles positional arguments like `samples` and `tdelay` (if present).

2. Memory Utilization:
   It uses the `/proc/meminfo` file to calculate the memory usage and display it in gigabytes, alongside a bar graph.

3. CPU Utilization:
   CPU usage is calculated based on the values in `/proc/stat`. The program compares the previous and current CPU states to determine the usage percentage and displays this value along with a graphical plot. If the value is 0, nothing is indicated on the 0 bar. In the situation of an empty line (ie no ':'s) it should be assumed 0.

4. Core Information:
   The program retrieves core count and maximum CPU frequency from `/proc/cpuinfo` and `/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo`, respectively. It then displays a representation of the cores.

Graphs work by printing line by line. If the percentage of the value of memo_util_arr or cpu_value_arr matches, # or : is printed, and otherwise " ".

This program uses usleep() do add the delays (with the flag tdelay and the respective variable delay). Users should be careful about which type of machines and versions of C accept usleep().

All 3 graphs (Memory, CPU, Cores) are written at the same time, unlike the demo video.

This program includes many variables, including buffer[x], line[y], and dummy[z]. These variables only exist for the fgets() function and do not get used anywhere else. Other variables have more meaningful names.

### Expected Output

- Memory Utilization: The program displays memory utilization in gigabytes and plots a bar graph.

- CPU Utilization: The program reports CPU utilization as a percentage and displays a bar graph.

- Core Information: The program displays the number of cores and their maximum frequency.

  Example:

  ```
  Number of Cores: 4 @ 3.40 GHz
  +--+ +--+ +--+ +--+
  |  | |  | |  | |  |
  +--+ +--+ +--+ +--+
  ```

### Additional Notes

- Modular Code: The program is written in a modular style with functions having clearly specified goals and tasks. There are no global variables, and all values are passed through function arguments.
- Error Handling: Proper error handling is included to check for failed memory allocations and file operations.
