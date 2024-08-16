// main.c
#include "shell.h"

int main(int argc, char* argv[]) {
    char line[MAX_INPUT_SIZE];            
    char **tokens;              

    signal(SIGINT, signal_handler);

    while(1) {           
        bzero(line, sizeof(line));
        printf("$ ");
        scanf("%[^\n]", line);
        getchar();

        line[strlen(line)] = '\n';

        tokens = split_input(line);

        if (tokens[0] && !strcmp(tokens[0], "exit") && !tokens[1]) {
            free_tokens(tokens);
            if (fore_pgid != -1) {
                kill(-fore_pgid, SIGINT);
            }
            if (back_pgid != -1) {
                kill(-back_pgid, SIGINT);
            }
            kill(-getpid(), SIGTERM);
            exit(0);
        }

        int check;
        while ((check = waitpid(-1, NULL, WNOHANG)) > 0) {
            printf("Shell : Background Process Finished\n");
            back_num--;
        }

        if (back_num == 0) {
            back_pgid = -1;
        }
        
        if (!tokens[0]) {
            continue;
        }

        int background = -1;
        for (int i = 0; i < MAX_NUM_TOKENS; i++) {
            if (tokens[i] == NULL) {
                break;
            } else if (!strcmp(tokens[i], "&")) {
                if (tokens[i+1] == NULL) {
                    background = i;
                } else {
                    background = -2;
                }
                break;
            }
        }

        if (background > 0) {
            for (int i = 0; i < MAX_NUM_TOKENS; i++) {
                if (tokens[i] == NULL) {
                    break;
                }
                if (!strcmp(tokens[i], "&&") || !strcmp(tokens[i], "&&&")) {
                    background = -2;
                    break;
                }
            }
        }

        if (background == -2) {
            printf("Shell : Incorrect Usage of &\n");
            continue;
        }
   
        if (strcmp(tokens[0], "cd")) {
            if (background > 0) {
                tokens[background] = NULL;
                back_num++;
            }

            int pid = fork();

            if (pid == 0) {
                if (background > 0) {
                    if (back_num == 1) {
                        setpgid(0, 0);
                    } else {
                        setpgid(0, back_pgid);
                    }
                } else {
                    setpgid(0, 0);
                }

                int co = 1;
                int prev = 0;

                for (int i = 0; i < MAX_NUM_TOKENS; i++) {
                    if (tokens[i] == NULL) {
                        break;
                    }

                    if (!strcmp(tokens[i], "&&")) {
                        tokens[i] = NULL;
                        if (fork() == 0) {
                            execvp(*(tokens+prev), tokens+prev);
                            printf("Shell: Incorrect Command : %s\n", *(tokens+prev));
                            exit(0);
                        } else {
                            wait(NULL);
                            prev = i+1;
                        }
                        continue;
                    }

                    if (!strcmp(tokens[i], "&&&")) {
                        tokens[i] = NULL;
                        co++;
                        if (fork() == 0) {
                            execvp(*(tokens+prev), tokens+prev);
                            printf("Shell: Incorrect Command : %s\n", *(tokens+prev));
                            exit(0);
                        } else {
                            prev = i+1;
                        }
                        continue;
                    }
                }
                if (fork() == 0) {
                    execvp(*(tokens+prev), tokens+prev);
                    printf("Shell: Incorrect Command : %s\n", tokens[0]);
                    exit(0);
                } else {
                    for (int i = 0; i < co; i++) {
                        wait(NULL);
                    }
                }
                exit(0);
            } else {
                if (background == -1) {
                    fore_pgid = pid;
                    waitpid(pid, NULL, 0);
                } else if (back_num == 1) {
                    back_pgid = pid;
                }
                fore_pgid = -1;
            }
        } else {
            if (background > 0) {
                printf("Shell: cd cannot be executed in background :\n");
                free_tokens(tokens);
                continue;
            }

            if (!tokens[1] || tokens[2]) {
                printf("Shell: Incorrect Command : %s\n", tokens[0]);
                free_tokens(tokens);
                continue;
            } else {
                int ret = chdir(tokens[1]);
                if (ret) {
                    printf("Shell: Incorrect Command : %s\n", tokens[0]);
                    free_tokens(tokens);
                    continue;
                }
            }
        }
       
        free_tokens(tokens);
    }
    return 0;
}