// signal_handler.c
#include "shell.h"

int fore_pgid = -1;
int back_num = 0;
int back_pgid = -1;

void signal_handler(int sig) {
    if (fore_pgid == -1) {
        return;
    }
    kill(-fore_pgid, SIGTERM);
}