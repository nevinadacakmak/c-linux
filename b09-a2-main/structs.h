#ifndef STRUCTS_H
#define STRUCTS_H

//CDF to store info about files
typedef struct FileInfo {
    int pid ;
    int fd ;
    int inode;
    char filename[256];
    struct FileInfo* next;
} FileInfo;

void add_file_info(int pid, int fd, int inode, char* filename, FileInfo** head, FileInfo** tail);
void free_linked_list(FileInfo** head, FileInfo** tail);


#endif 