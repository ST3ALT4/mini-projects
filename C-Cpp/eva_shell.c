#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define EVA_RL_BUFSIZE 1024

#define EVA_TOK_BUFSIZE 64
#define EVA_TOK_DELIM " \t\r\n\a"

void eva_loop();

int eva_cd(char **args);
int eva_help(char **args);
int eva_exit(char **args);

char *builtin_str[]={
    "cd",
    "help",
    "exit"
};

int (*builtin_func[]) (char**) = {
    &eva_cd,
    &eva_help,
    &eva_exit
};

int main(int argc,char **argv){
    eva_loop();

    return EXIT_SUCCESS;
}

void eva_loop(){

    char *line;
    char **args;
    int status;

    do{
        printf("$");
        line = eva_read_line();
        args = eva_split_line(line);
        status = eva_execute();

        free(line);
        free(args);

    }while(status);

}

char* eva_read_line(){

    char *line = NULL;
    size_t buff_size = 0;

    if(getline(&line,&buff_size,stdin)==-1){
        if(feof(stdin)){
            exit(EXIT_SUCCESS);
        }else{
            perror("readline\n");
            exit(EXIT_FAILURE);
        }
    }

}

char** eva_split_line(char* line){
    
    int bufsize = EVA_TOK_BUFSIZE,position =0;
    char** tokens = malloc(bufsize*sizeof(char*));
    char *token;

    if(!tokens){
        fprinf(stderr,"allocation: token allocation error!\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line,EVA_TOK_DELIM);
    while(token != NULL){
        tokens[position] = token;
        position++;

        if(position>=bufsize){
            bufsize+=EVA_TOK_BUFSIZE;
            tokens = realloc(tokens,bufsize*sizeof(char*));

            if(!tokens){
                fprinf(stderr,"reallocation: failed to realloc token\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL,EVA_TOK_DELIM);
    }

    tokens[position] = NULL;
    return tokens;
}

int eva_launch(char** args){

    pid_t pid,wpid;
    int status;

    pid = fork();
    if(pid == 0){
        if(execvp(args[0],args) == -1){
            perror("eva");
        }
        exit(EXIT_FAILURE);
    }else if(pid<0){
        perror("eva");
    }else{
        do{
            wpid = waitpid(pid,&status,WUNTRACED);
        }while(!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

