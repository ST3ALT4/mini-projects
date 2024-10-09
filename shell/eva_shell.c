#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define EVA_RL_BUFSIZE 1024
#define EVA_TOK_BUFSIZE 64
#define EVA_TOK_DELIM " \t\r\n\a"

void eva_loop();

int eva_cd(char **args);
int eva_help(char **args);
int eva_exit(char **args);

char *builtin_str[] = {
    "cd",
    "help",
    "exit"
};

int (*builtin_func[]) (char**) = {
    &eva_cd,
    &eva_help,
    &eva_exit
};

int eva_num_builtin() {
    return sizeof(builtin_str) / sizeof(char *);
}

int eva_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "eva: expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("eva");
        }
    }
    return 1;
}

int eva_help(char **args) {
    printf("Copied from Stephen Brennan's LSH\n");
    printf("Type program name and argument and hit enter\n");
    printf("The following are builtin:\n");
    for (int i = 0; i < eva_num_builtin(); i++) {
        printf("  %s\n", builtin_str[i]);
    }
    printf("Use the command for information on other programs.\n");
    return 1;
}

int eva_exit(char **args) {
    return 0;
}

char *eva_read_line() {
    char *line = NULL;
    size_t buff_size = 0;

    if (getline(&line, &buff_size, stdin) == -1) {
        if (feof(stdin)) {
            exit(EXIT_SUCCESS);
        } else {
            perror("readline");
            exit(EXIT_FAILURE);
        }
    }
    return line;
}

char **eva_split_line(char* line) {
    int bufsize = EVA_TOK_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    if (!tokens) {
        fprintf(stderr, "allocation: token allocation error!\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, EVA_TOK_DELIM);
    while (token != NULL) {
        tokens[position++] = token;

        if (position >= bufsize) {
            bufsize += EVA_TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                fprintf(stderr, "reallocation: failed to realloc token\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, EVA_TOK_DELIM);
    }

    tokens[position] = NULL;
    return tokens;
}

int eva_launch(char** args) {
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            perror("eva");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("eva");
    } else {
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}

int eva_execute(char **args) {
    if (args[0] == NULL) {
        return 1; // An empty command was entered
    }

    for (int i = 0; i < eva_num_builtin(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_func[i])(args);
        }
    }

    return eva_launch(args);
}

void eva_loop() {
    char *line;
    char **args;
    int status;
    char cwd[EVA_RL_BUFSIZE];

    do {
        printf("$ ");
        if(getcwd(cwd,sizeof(cwd))!=NULL){
            printf("%s> ",cwd);
        }else{
            fprintf(stderr,"EVA: current directory");
        }
        line = eva_read_line();
        args = eva_split_line(line);
        status = eva_execute(args);

        free(line);
        free(args);
    } while (status);
}

int main(int argc, char **argv) {
    eva_loop();
    return EXIT_SUCCESS;
}