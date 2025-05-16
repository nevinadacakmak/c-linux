#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "outputs.h"
#include "structs.h"

void composite(int pidcli, FileInfo** head){
    ///_|> descry: prints necessary info for the --composite flag
    ///_|> pidcli: this is the given pdi as a positional argument, -1 if not given
    ///_|> head: this is the head of the linked list which stores file information
    ///_|> returning: this function does not return anything
    printf("PID\tFD\tFilename\tInode\n");
    printf("========================================\n");
    FileInfo* current = *head;
    while (current) {
        if (pidcli == -1)
        {
            printf("%d\t%d\t%s\t%d\n", current->pid, current->fd, current->filename,current->inode);
        }
        else //pid is given
        {
            if(current->pid == pidcli )
            {
                printf("%d\t%d\t%s\t%d\n", current->pid, current->fd, current->filename,current->inode);
            }
            if (current->pid > pidcli){
                break; //already over, no need to check others
            }
        }
        current = current->next;
    }
    printf("========================================\n\n");
}

void summary(int pidcli, FileInfo** head){
    ///_|> descry: prints necessary info for the --summary flag
    ///_|> pidcli: this is the given pdi as a positional argument, -1 if not given
    ///_|> head: this is the head of the linked list which stores file information
    ///_|> returning: this function does not return anything
    FileInfo* current = *head;

            int counter = 0;

            printf("\tSummary Table\n");
            printf("\t=============\n");

            while (current) {
                if (pidcli == -1)
                {
                    int curpid = current->pid; //current pid

                    //print pid and num of FD s
                    while (current && current->pid == curpid){
                        counter++;
                        current = current->next;
                    }
                    printf("%d (%d),",curpid,counter+1);
                    counter = 0;
                }
                else //pid is given
                {
                    if(current->pid == pidcli)
                    {
                        int curpid = current->pid; //current pid
    
                        //print pid and num of FD s
                        while (current && current->pid == curpid){
                            counter++;
                            current = current->next;
                        }
                        printf("%d (%d),",curpid,counter);
                        counter = 0;
                    }
                    if (current->pid > pidcli){
                        break; //already over, no need to check others
                    }
                }
                if(current) {current = current->next;} //check if curr is nil from the while loops above
            }
            printf("\n");
}

void perprocess(int pidcli, FileInfo** head){
    ///_|> descry: prints necessary info for the --per-process flag
    ///_|> pidcli: this is the given pdi as a positional argument, -1 if not given
    ///_|> head: this is the head of the linked list which stores file information
    ///_|> returning: this function does not return anything
    printf("PID\tFD\n");
    printf("============\n");
    FileInfo* current = *head;
    while (current) {
        if (pidcli == -1)
        {
            printf("%d\t%d\n", current->pid, current->fd);
        }
        else //pid is given
        {
            if(current->pid == pidcli )
            {
                printf("%d\t%d\n", current->pid, current->fd);
            }
            if (current->pid > pidcli){
                break; //already over, no need to check others
            }
        }
        current = current->next;
    }
    printf("============\n\n");
}

void systemWide(int pidcli, FileInfo** head){
    ///_|> descry: prints necessary info for the --systemWide flag
    ///_|> pidcli: this is the given pdi as a positional argument, -1 if not given
    ///_|> head: this is the head of the linked list which stores file information
    ///_|> returning: this function does not return anything
    printf("PID\tFD\tFilename\n");
    printf("========================================\n");
    FileInfo* current = *head;
    while (current) {
        if (pidcli == -1)
        {
            printf("%d\t%d\t%s\n", current->pid, current->fd, current->filename);
        }
        else //pid is given
        {
            if(current->pid == pidcli )
            {
                printf("%d\t%d\t%s\n", current->pid, current->fd, current->filename);
            }
            if (current->pid > pidcli){
                break; //already over, no need to check others
            }
        }
        current = current->next;
    }
    printf("========================================\n\n");
}

void vnodes(int pidcli, FileInfo** head){
    ///_|> descry: prints necessary info for the --Vnodes flag
    ///_|> pidcli: this is the given pdi as a positional argument, -1 if not given
    ///_|> head: this is the head of the linked list which stores file information
    ///_|> returning: this function does not return anything
    printf("FD\tInode\n");
    printf("========================================\n");
    FileInfo* current = *head;

    while (current) {
        if (pidcli == -1)
        {
            printf("%d\t%d\n", current->fd, current->inode);
        }
        else //pid is given
        {
            if(current->pid == pidcli)
            {
                printf("%d\t%d\n", current->fd, current->inode);
            }
            if (current->pid > pidcli){
                break; //already over, no need to check others
            }
        }
        current = current->next;
    }
    printf("========================================\n\n");
}

void threshold(int pidcli, FileInfo** head, int threshld){
    ///_|> descry: prints necessary info for the --threshold=X flag
    ///_|> pidcli: this is the given pdi as a positional argument, -1 if not given  
    ///_|> head: this is the head of the linked list which stores file information
    ///_|> threshld: this is the given threshold from the argument
    ///_|> returning: this function does not return anything

    FileInfo* current = *head;
    printf("threshold: %d\n",threshld);
    printf("## Offending processes:\n");
    int counter = 0;

    while (current) {
        if (pidcli == -1)
        {
            counter = 0;
            int curpid = current->pid; //current pid

            while (current && current->pid == curpid){
                counter++;
                current = current->next;
            }

            if (counter >= threshld){
                printf("%d (%d),",curpid,counter);
            }
        }
        else //pid is given
        {
            if(current->pid == pidcli)
            {
                counter = 0;
                int curpid = current->pid; //current pid

                while (current && current->pid == curpid){
                    counter++;
                    current = current->next;
                }

                if (counter >= threshld){
                    printf("%d (%d),",curpid,counter);
                }
            }
            else {
                current = current->next;
            }
            if (current->pid > pidcli){
                break; //already over, no need to check others
            }
        }
    }

    printf("\n");
}

void outputtxt(int pidcli, FileInfo** head){
    ///_|> descry: prints necessary info to the compositeTable.txt file for the --output_TXT flag
    ///_|> pidcli: this is the given pdi as a positional argument, -1 if not given  
    ///_|> head: this is the head of the linked list which stores file information
    FILE* ftxtptr;

    ftxtptr = fopen("compositeTable.txt","w");
    if (!ftxtptr) {perror("Error opening file");return;}

    fprintf(ftxtptr,"PID\tFD\tFilename\tInode\n");
    fprintf(ftxtptr,"========================================\n");
    FileInfo* current = *head;
    while (current) {
        if (pidcli == -1)
        {
            fprintf(ftxtptr,"%d\t%d\t%s\t%d\n", current->pid, current->fd, current->filename,current->inode);
        }
        else //pid is given
        {
            if(current->pid == pidcli)
            {
                fprintf(ftxtptr,"%d\t%d\t%s\t%d\n", current->pid, current->fd, current->filename,current->inode);
            }
            if (current->pid > pidcli)
            {
                break; //already over, no need to check others
            }
        }
        current = current->next;
    }
    fprintf(ftxtptr,"========================================\n");

    fclose(ftxtptr);
}

void outputbin(int pidcli, FileInfo** head){
    ///_|> descry: prints necessary info to the compositeTable.bin file for the --output_TXT flag
    ///_|> pidcli: this is the given pdi as a positional argument, -1 if not given  
    ///_|> head: this is the head of the linked list which stores file information
    FILE* fbinptr;

    fbinptr = fopen("compositeTable.bin","wb"); //writes binary
    if (!fbinptr) {perror("Error opening file");return;}

    //char buffer[512];

    FileInfo* current = *head;
    while (current) {
        if (pidcli == -1)
        {
            fwrite(&current->pid, sizeof(int), 1, fbinptr);
            fwrite(&current->fd, sizeof(int), 1, fbinptr);
            fwrite(&current->inode, sizeof(int), 1, fbinptr);

            char filename[256]; //storing filename
            strncpy(filename, current->filename, sizeof(filename) - 1);
            fwrite(filename, sizeof(char), 256, fbinptr);
        }
        else //pid is given
        {
            if(current->pid == pidcli )
            {
                fwrite(&current->pid, sizeof(int), 1, fbinptr);
                fwrite(&current->fd, sizeof(int), 1, fbinptr);
                fwrite(&current->inode, sizeof(int), 1, fbinptr);

                char filename[256]; //storing filename
                strncpy(filename, current->filename, sizeof(filename) - 1);
                fwrite(filename, sizeof(char), 256, fbinptr);
            }
            if (current->pid > pidcli){
                break; //already over, no need to check others
            }
        }
        current = current->next;
    }

    fclose(fbinptr);
}