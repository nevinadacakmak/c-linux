#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "structs.h"
#include "parsing_proc.h"

void parsing_proc(FileInfo** head, FileInfo** tail) {
    ///_|> descry: parses /proc and gets needed information (pid, FD, inode, filename)
    ///_|> head: this is the head of the linked list which stores file information
    ///_|> tail: this is the tail of the linked list which stores file information
    ///_|> returning: this function does not return anything
    char buffer[64];
    char filename[256];

    int pid;
    int fd;

    DIR* proc_dir = opendir("/proc");
    if (!proc_dir) {perror("Cannot open /proc"); return;}

    struct dirent* proc_entry;
    while ((proc_entry = readdir(proc_dir)) != NULL) { //traversing proc part
        //gets a pid
        if (isdigit(proc_entry->d_name[0])){
            pid = atoi(proc_entry->d_name);
        }

        //look for the fd, vnode, filename of the file with pid
        snprintf(buffer, sizeof(buffer), "/proc/%d/fd",pid); //prints to buffer
        DIR* pid_dir = opendir(buffer);
        if (!pid_dir) {continue;} //checks for permission problems

        struct dirent* pid_entry;
        while ((pid_entry = readdir(pid_dir)) != NULL) { //traversing fd part
            if (isdigit(pid_entry->d_name[0]))
            {
                fd = atoi(pid_entry->d_name);
                snprintf(buffer, sizeof(buffer), "/proc/%d/fd/%d", pid, fd); //prints to buffer again
            }

            struct stat filebuffer;
            if (stat(buffer, &filebuffer) == 0) 
            { //checking that filebuffer is valid
                ssize_t len = readlink(buffer, filename, sizeof(filename) - 1);
                if (len != -1) 
                {
                    filename[len] = '\0';
                    add_file_info(pid, fd, (int)filebuffer.st_ino, filename, head, tail); //adding info to the linked list
                }
            }
        }
        closedir(pid_dir);
    }
    closedir(proc_dir);
}


