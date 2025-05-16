/*

Assignment 2, CSCB09, Winter 2025

*/

#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "parsing_proc.h"
#include "structs.h"
#include "outputs.h"

int main(int argc, char* argv[]){
    FileInfo* head = NULL;
    FileInfo* tail = NULL;

    //parse /proc for all the necessary info
    parsing_proc(&head, &tail);

    int pidcli = -1;

    //get the pid first, if mentioned as a positional flag
    for(int i = 1; i < argc; i++){
        if (isdigit(argv[i][0])){
            pidcli = atoi(argv[i]);
        }
    }

    if (argc == 1 || (argc == 2 && pidcli != -1)){ //defining default behaviour
        composite(pidcli, &head);
    }

    //parsing CLI and printing necessary info
    for(int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i],"--per-process")==0)
        {
            perprocess(pidcli, &head);
        }
        else if (strcmp(argv[i],"--systemWide")==0)
        {
            systemWide(pidcli, &head);
        }
        else if (strcmp(argv[i],"--Vnodes")==0)
        {
            vnodes(pidcli, &head);
        }
        else if (strcmp(argv[i],"--summary")==0)
        {
            summary(pidcli, &head);
        }
        else if (strcmp(argv[i],"--output_TXT")==0)
        {
            outputtxt(pidcli, &head);
        }
        else if (strcmp(argv[i],"--output_binary")==0)
        {
            outputbin(pidcli, &head);
        }
        else if (strncmp(argv[i],"--threshold=", 12)==0)
        {
            int threshld = atoi(argv[i]+12);
            threshold(pidcli, &head, threshld);
        }
        else if (atoi(argv[i]) == pidcli){ //so that positional arg doesnt go to the composite case or "else"
            continue;
        }
        else if (strcmp(argv[i],"--composite")==0)
        {
            composite(pidcli, &head);
        }
        else{ //invalid input
            printf("You have entered an invalid input!\n");
        }
     }

    free_linked_list(&head, &tail);

    return 0;
}