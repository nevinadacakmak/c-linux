#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>


void sigint_handler(int sig) {
    ///_|> descry: function to handle SIGINT (Ctrl+C) signal
    ///_|> sig: signal number
    ///_|> returning: returns nothing
    char msg[] = "\nThis is ctrl c, Caught signal 2\n";
    write(STDOUT_FILENO, msg, sizeof(msg) - 1); // write() is async-signal-safe
    
    char prompt[] = "do you want to stop the process? (y/n): ";
    write(STDOUT_FILENO, prompt, sizeof(prompt) - 1);

    char response;
    ssize_t n = read(STDIN_FILENO, &response, 1); //read a single char

    if (n > 0 && (response == 'y')) 
    {
        char quit_msg[] = "stopping process...\n";
        write(STDOUT_FILENO, quit_msg, sizeof(quit_msg) - 1);
        kill(getpid(), SIGKILL); //kill itself instead of parent
    } 
    else {
        char cont_msg[] = "continuing...\n";
        write(STDOUT_FILENO, cont_msg, sizeof(cont_msg) - 1);
        kill(getpid(), SIGCONT); //continue itself
    }

}

void sigtstp_handler(int sig) {
    ///_|> descry: function to handle SIGTSTP (Ctrl+Z) signal
    ///_|> sig: signal number
    ///_|> returning: returns nothing
    char msg[] = "This is ctrl z, Caught signal 20\n";
    write(STDOUT_FILENO, msg, sizeof(msg) - 1);
}