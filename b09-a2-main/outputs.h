#ifndef OUTPUTS_H
#define OUTPUTS_H

#include "structs.h"

void composite(int pidcli, FileInfo** head);
void summary(int pidcli, FileInfo** head);
void perprocess(int pidcli, FileInfo** head);
void systemWide(int pidcli, FileInfo** head);
void vnodes(int pidcli, FileInfo** head);
void threshold(int pidcli, FileInfo** head, int threshld);
void outputtxt(int pidcli,FileInfo** head);
void outputbin(int pidcli,FileInfo** head);

#endif