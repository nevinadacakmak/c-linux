# TITLE - Assignment 2: Recreating the System-Wide FD Tables

## Metadata
Author: Nevin Ada Cakmak
Date: 03/10/2025

## Introduction
For this assignment I built a tool to show the tables used by the OS to keep track of open files, pids, FDs and more.

## Description
/proc includes very useful information, including pids, FDs, file names, inodes, and more. we need to keep track of all the information, and print it accurately as needed, according to flags (positional flags, and others). In linux, everything is a file. Thus, all of them have some information stored in them, in /proc.

I knew this was a major assingment, so I started by reading the assignment handout, piazza forum to read instructor posts about the assingments, and watch the demo video to come up with steps for my implementation. Inital to-do list was:
    0- Come up with a way to store all the information in an easily accessible way
    1- Parse the given CLI arguments
    2- Parse /proc to get the necessary information, and store it
    3- Print the necessary information where needed, for each flag

Therefore, I solved these problems step by step (explained in ### implementation)

## Implementation
### i - main implementation

0- I defined my CDT to include PID, FD, Vnode, Filename, and *next for keeping track of the next node. I needed a "pay as you go" system, so I used a linked list to keep track of every node, which essentially has a node for every open process (explained more in limitations), implemented functions for adding a node, and freeing the linked list

1- Parsed the CLI arguments the same as A1, except this is not that complex. First, I traverse all to get the PID (if it is given, -1 otherwise) then I traversed all again because all implementations for flags differ, based on the PID (if given or not) and each arguments calls their own function (seperated for modularity, initial full code was 450 lines long)

2- Parsed /proc first to access each pid, then tried to access /proc/pid/fd for each pid, to get the information and store it. I think this was the hardest part, and I had to use some functions from outside libraries to get all the needed information

3- This part was just accessing the information, and printing it, seperated to a different .c file and functions for better modularity.

### ii &  iii - modules and functions 
structs.h & structs.c
this module includes all the functions about the linked list, and includes the structs for my linked list, along with the functions:

void add_file_info(int pid, int fd, int inode, char* filename, FileInfo** head, FileInfo** tail): this function gets the information pid, fd, filename, linked list head, linked list tail and stores all in the linked list as a new node. This doesn't use any outside libraries than default ones (stdio, stdlib, etc)

void free_linked_list(FileInfo** head, FileInfo** tail): this function gets the linked list head and tail, and frees the memory used for linked list at the end of the program. This doesn't use any outside libraries than default ones (stdio, stdlib, etc)

parsing_proc.c & parsing_proc.h
this module includes all the necessary code for parsing /proc and obtaining necessary information (filename, inode, pid, fd)
it just has the function: 

void parsing_proc(FileInfo** head, FileInfo** tail): this function parses the /proc and /proc/pid/fd to get the needed information. this is the one that needs most extra functions and libraries. Uses #include <dirent.h> to work on different directories, save them, and open them. uses snprintf() to write strings into char arrays, and adds \0 to terminate them. And uses #include <sys/stat.h> to define a filebuffer to use the sn_ino feature to get the inodes, and uses ssize_t to get the size to terminate the string, and uses readlink (included by #include <unistd.h>) to add information to the linked list as a node (by add_file_info function)

outputs.c & outputs.h
this is the module for the functions to print, according to each flag (composite, summary, vnodes....) along with the bonus parts

void composite(int pidcli, FileInfo** head);
void summary(int pidcli, FileInfo** head);
void perprocess(int pidcli, FileInfo** head);
void systemWide(int pidcli, FileInfo** head);
void vnodes(int pidcli, FileInfo** head);
void void threshold(int pidcli, FileInfo** head, int threshold);
void outputtxt(int pidcli,FileInfo** head);
void outputbin(int pidcli,FileInfo** head);

all of them get the given pidcli (positional argument for pid) and the head of the link list, pidcli is -1 if no pid is given  by the user. User is also warned with "You have the wrong input" if the input is not equal to any of the flags, or pid. Nothing other than default basic functions are used, along with standard libraries, and each function have 2 flows, based on if pid is given or not.

All of the functions above use the linked list, and nodes to print the needed information (ie vnodes, filename, pid...) since they are stored once, i am following file i/o best practices by reading it only once.

composite() is the default behaviour if no arguments is passed into it.

main.c
this is my main file/module, and it includes the step of printing the information i need. i implemented different solutions for each flag (ie printing different information) and all of the solutions for flag boils down to 2 steps, whether a pid is provided or not. 

int main(int argc, char* argv[])

This is the file where everything comes together, and finishes by deallocating space used by linked list.

## Pseudocode of the Code
main
    declare head, tail of linked list
    parse_proc()
    get pid from arguments
    parse CLI arguments
        if summary
            run summary()
        if composite
            run composite()
        if vnode
            run vnode()
        if perprocess
            run perprocess()
        if systemwide
            run systemwide()
        if threshold
            run threshold()
        if txtbinary
            run txtbinary()
        if txtfile
            run txtfile()
        else, warn the user about wrong input
    free_linked_list()

parse_proc()
    get /proc directory
    for each pid, enter /proc/pid/fd
        add_file_info(pid, fd, filename, inode)
    close directories

free_linked_list()
    free each node
    free the linked list

add_file_info()
    create a new node
    declare next
    change tail node
    enter pid, fd, filename, inode info

summary()
    if pid is given
        go to the first node with given pid
        for each process for pid
            print necessary info
    else
        for each node
            print necessary info
composite()
    if pid is given
        go to the first node with given pid
        for each process for pid
            print necessary info
    else
        for each node
            print necessary info
vnode()
    if pid is given
        go to the first node with given pid
        for each process for pid
            print necessary info
    else
        for each node
            print necessary info
perprocess()
    if pid is given
        go to the first node with given pid
        for each process for pid
            print necessary info
    else
        for each node
            print necessary info
systemwide()
threshold()
    if pid is given
        go to the first node with given pid and threshold
        for each process for pid
            print necessary info
    else
        for each node with num(process) > threshold
            print necessary info
txtfile()
    get all the info for composite()
    for each information
        print to compositeTXT
txtbinary()
    get all the info for composite()
    for each struct
        convert to binary
        store in compositeBIN

## Compilation Instructions
if not using a makefile, "gcc -Wall -Werror main.c outputs.c parsing_proc.c structs.c -lm" and then "./a.out [command line arguments]"
if using the makefile, "make" or "make all" and ./FD_table [command line arguments]

Many libraries are included, mainly 
#include <dirent.h> ->for direction
#include <sys/stat.h> ->for stat filebuffer, and accessing inode
#include <unistd.h> ->for snprintf and ssize_t
#include <ctype.h> ->isdigit to do sanity check on pid entries

and usual basic-level files
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

My makefile has a few rules, mainly:
all: creates the final executable FD_table by compiling everything
clean: cleans all .o files
help: echoes the basic to-do for the 2 rules above

Rest is, rules about each module
main is dependent on output, parsing_proc, structs .c
output is dependent on structs.c
parsing_proc is dependent on structs.c
structs is dependent on parsing_proc.c

Rules compile each by gcc -Werror -Wall [ all dependencies (.c)] (used variables)

Therefore, final executable FD_table is produced 

## Expected Results
If a command line argument is not valid, user is warned with "You entered an invalid input"
Otherwise, relevant info is printed for each argument, as expected.
If the user cannot access the /proc/pid/fd for a spesific pid, it skips it without outputs
Cases are: Summary, Composite, Per-Process, SystemWide, Vnode, Threshold, OutputTXT, and OutputBIN

Summary prints the number of processes for each individual pid
Composite prints pid, fd, inode, filename for each process
Per-process prints pid and each fd for a pid
Systemwide prints pid, each fd and filename
Vnode prints FD and inode information for every FD
Threshold prints summary, but the processes with FD numbers with threshold more than the given threshold > x
Outputtxt prints composite to the txt file in ascii
Outputbin pritns the struct linked list to the bin file, that is not human readable

Some examples, PIDs are given for simplicity

Case 1
cakmakne@it-ia3170-42:~/cscb09w25_space/b09-a2/b09-a2$ ./FD_table 130744
PID     FD      Filename        Inode
========================================
130744  0       /dev/null       5
130744  1       socket:[ 1196806 ]        1196806
130744  2       socket:[ 1196806 ]        1196806
130744  3       socket:[ 1189777 ]        1189777
130744  4       socket:[ 1189771 ]        1189771
130744  5       /usr/share/code/icudtl.dat      9054218
130744  6       /usr/share/code/v8_context_snapshot.bin 9075707
130744  7       /usr/share/code/chrome_100_percent.pak  9054213
130744  8       /usr/share/code/chrome_200_percent.pak  9054214
130744  9       /usr/share/code/locales/en-US.pak       9054237
130744  10      /usr/share/code/resources.pak   9075704
130744  11      /dev/urandom    10
========================================

Case 2
cakmakne@it-ia3170-42:~/cscb09w25_space/b09-a2/b09-a2$ ./FD_table 130744 --summary
        Summary Table
        =============
130744 (12),

Case 3
cakmakne@it-ia3170-42:~/cscb09w25_space/b09-a2/b09-a2$ ./FD_table 130744 --per-process
PID     FD
============
130744  0
130744  1
130744  2
130744  3
130744  4
130744  5
130744  6
130744  7
130744  8
130744  9
130744  10
130744  11
============


## Test Cases
I checked all potential flags with 2 cases: a pid is given, and pid is not given, also checked invalid inputs
Biggest problem was not being able to access /proc/pid/fd for spesific files, and open processes checking so i need to run all tests at once to get accureate results.
My program checks if pid and fd's are digits, and problems may occur if spesific pid or fd's are legit but not digits.

## Disclaimers
My implementation creates nodes for each process, so the length of the linked list is the number of open processes, which may be really big in some cases, and program amy run out of space or traversing the linked list for each flag might take some time, and be sub-optimal. A different and optimized approach would be using 2 or 3 linked lists to store files and processes, but my time permits that. 

## References
https://pubs.opengroup.org/onlinepubs/007904975/basedefs/sys/types.h.html
https://pubs.opengroup.org/onlinepubs/009696699/basedefs/sys/stat.h.html
https://pubs.opengroup.org/onlinepubs/7908799/xsh/dirent.h.html
https://pubs.opengroup.org/onlinepubs/7908799/xsh/unistd.h.html
https://pubs.opengroup.org/onlinepubs/009696699/functions/readlink.html
https://stackoverflow.com/questions/17598572/how-to-read-write-a-binary-file
https://www.geeksforgeeks.org/how-to-write-struct-to-a-binary-file-in-c/
https://piazza.com/class/m5j5smmmxyx2s0

## Bonus Part (Report)
Instructions: Run a comparison using these two flags, for a few cases, i.e. one PID and larger number (all PIDs for the user), time the run, you can use the time command from the shell and compare the file sizes.
For your comparison to be relevant should have some statistical support with a number of cases between 5 and 10, i.e. average the values in time and sizes over the number of times you run this and include average and standard deviations.
Include this information in your report and elaborate about the observations you notice and conclusions you can draw from them.

### Cases

Case 1: No PID given

cakmakne@it-ia3170-42:~/cscb09w25_space/b09-a2/b09-a2$ time ./FD_table --output_TXT

real	0m0.038s
user	0m0.000s
sys	0m0.014s

cakmakne@it-ia3170-42:~/cscb09w25_space/b09-a2/b09-a2$ time ./FD_table --output_binary
 
real	0m0.088s
user	0m0.004s
sys	0m0.008s

Case 2: PID#1 given
cakmakne@it-ia3170-42:~/cscb09w25_space/b09-a2/b09-a2$ time ./FD_table 130743 --output_TXT

real	0m0.020s
user	0m0.000s
sys	0m0.009s

cakmakne@it-ia3170-42:~/cscb09w25_space/b09-a2/b09-a2$ time ./FD_table 130743 --output_binary

real	0m0.024s
user	0m0.001s
sys	0m0.009s

Case 3: PID#2 given
cakmakne@it-ia3170-42:~/cscb09w25_space/b09-a2/b09-a2$ time ./FD_table 129869 --output_TXT

real	0m0.019s
user	0m0.001s
sys	0m0.009s

cakmakne@it-ia3170-42:~/cscb09w25_space/b09-a2/b09-a2$ time ./FD_table 129869 --output_binary

real	0m0.022s
user	0m0.000s
sys	0m0.010s

Case 4: PID#3 given
cakmakne@it-ia3170-42:~/cscb09w25_space/b09-a2/b09-a2$ time ./FD_table 129457 --output_TXT

real	0m0.022s
user	0m0.000s
sys	0m0.010s

cakmakne@it-ia3170-42:~/cscb09w25_space/b09-a2/b09-a2$ time ./FD_table 129457 --output_binary

real	0m0.024s
user	0m0.001s
sys	0m0.009s

Case 5: No PID given #2
cakmakne@it-ia3170-42:~/cscb09w25_space/b09-a2/b09-a2$ time ./FD_table --output_TXT

real	0m0.035s
user	0m0.003s
sys	0m0.008s

cakmakne@it-ia3170-42:~/cscb09w25_space/b09-a2/b09-a2$ time ./FD_table --output_binary
 
real	0m0.086s
user	0m0.002s
sys	0m0.008s

### Statistical Data
Average time for --output_TXT: 0.268
Average time for --output_binary: 0.488

Disclaimer: --output_binary only prints the structs, not the titles and "======="'s, but compared to a computer's machine power, that 3 converting and printing is not crucial for the time().

Average time for --output_TXT for when PID was given: 0.203
Average time for --output_TXT for when PID was not given: 0.365

Average time for --output_binary for when PID was given: 0.23
Average time for --output_binary for when PID was not given: 0.87

### Conclusion
In the cases where PID was not given, functions had to write hundreds, if not thousands of lines. Thus, it does not align with our notes from week 4. This implies a mistake in my implementation, or the way i am storing information in binary. On the other hand, i convert my struct to binary in 3 function calls. meanwhile TXT representation is just a function call. Therefore, we cannot compare a direct result with storing in binary or ascii from my implementation.

In the cases where PID was given, averages are more-or-less the same, and this agrees to our points from Week 4 where storing in binary and ASCII requires similar bytes, and thus, similar time

My overall complexity is not ideal since all processes are stored in one big linked list, and functions traverse that.
