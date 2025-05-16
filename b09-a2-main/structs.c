#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "parsing_proc.h"
#include "structs.h"

void add_file_info(int pid, int fd, int inode, char* filename, FileInfo** head, FileInfo** tail) {
    ///_|> descry: adds all the given info to the linked list as a node
    ///_|> pid: this is an integer, for the pid of a file
    ///_|> fd: this is an integer, for the FD of a pid of a file
    ///_|> returning: this function does not return anything

    FileInfo* new_node = (FileInfo*)malloc(sizeof(FileInfo));
    if (!new_node) {perror("Malloc failed");}

    new_node->pid = pid;
    new_node->fd = fd;
    new_node->inode = inode;

    strncpy(new_node->filename, filename, sizeof(new_node->filename) - 1);
    new_node->filename[sizeof(new_node->filename) - 1] = '\0';//adding \0 to indicate the end of the string

    new_node->next = NULL;

    if (*head == NULL) {
        *head = *tail = new_node;
    } 
    else {
        // Otherwise, add the new node to the end of the list and update tail
        (*tail)->next = new_node;
        *tail = new_node;
    }
}

void free_linked_list(FileInfo** head, FileInfo** tail) {
    ///_|> descry: frees all the space reserved for the linked list
    ///_|> head: this is the head of the linked list which stores file information
    ///_|> tail: this is the tail of the linked list which stores file information
    ///_|> returning: this function does not return anything
    FileInfo* current = *head;
    while (current) {
        FileInfo* temp = current;
        current = current->next;
        free(temp);
    }
    *head = *tail = NULL;
}