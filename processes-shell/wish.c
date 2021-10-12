#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>


int main(int argc, char *argv[]){

    // GLOBAL VARS
    // RUN MODE: 0 -> Interactive, 1 -> Batch
    int rm = 0;

    // See Piazza: @376
    int paths_num = 100;
    int paths_used = 2;
    char *paths[paths_num];
    paths[0] = "/bin";
    paths[1] = "usr/bin/ls"; //check if prog is here if needed

    //BUILT IN COMMANDS
    static char* built_in_cmds[3] = {"exit","cd","path"};
    int num_bic = 3;
    
    //check cl args
    if(argc > 2){
        printf("error: too many arguments provided\n");
        exit(1);//error, more than a single argument
    }else if(argc == 2){
        rm = 1; // Run mode indicator: BATCH
    }


    // BATCH mode, second argument must be a text file to read input from
    if(strcmp(argv[0],"./wish")== 0){
        //printf("Two arguments: Batch Mode with argument: %s \n",argv[1]);
        // TODO IMPLEMENT FOR A FILE INPUT
        // try to open file and store txt -> if fails, exit(1)
    }    
    // INTERACTIVE mode
    //while loop - condition: if the next input is not exit
    char *line = " ";
    size_t len = 0;
    ssize_t nread;

    //MAIN LOOP
    while (strcmp(line,"exit") != 0) {
        
        if(rm == 0){
            //INTERACTIVE MODE
            printf("wish> ");
            nread = getline(&line, &len, stdin);
            if(nread == EOF){
                //try ctrl-d to test
                printf("EOF returned to nread\n"); 
                exit(0);
            }
        }else if(rm == 1){
            //BATCH MODE
            FILE *fp = fopen(filep,"r");
            if (fp == NULL){
                exit(1);// BAD FILE
            }else{
                //this is where you open up the file
            }
            exit(0);//todo implement
        }

        //PROCESS USER INPUT ex: "ls -la /tmp"
        // printf("Retrieved line of length %zu:\n", nread);
        //fwrite(line, nread, 1, stdout);
        char *dest[100];
        char *copy = line;
        int i;

        //https://stackoverflow.com/questions/50915364/how-strsep-works-in-c
        copy[strcspn(copy, "\n")] = '\0';//removes trailing newline

        for (i = 0; i < 10 && (dest[i] = strsep(&copy, " ")) != NULL; i++);   
        // for (int c = 0; c < i; c++)
        //     printf(" arg %d : [%s]\n", c, dest[c]); 

        int num_args = i - 1; //prog name implicit
        // printf("Num args: %i\n", num_args);

        //parsed args in dest, cleaned so no newline
        int is_bic = -1;
        // BUILT-IN-COMMANDS CHECK
        for(int i = 0; i < num_bic; i++){
            if(strcmp(dest[0], built_in_cmds[i]) == 0){
                is_bic = 0;
                //process command
                //continue since we know command is built in
                // (don't need to check for other commands)
                if(i == 0){
                    // EXIT BIC
                    //call exit
                    exit(0);
                }
                if(i == 1){
                    // CD BIC
                    if((num_args == 0) | (num_args > 1)){
                        char error_message[30] = "An error has occurred\n";
                        write(STDERR_FILENO, error_message, strlen(error_message)); 
                    }else{
                        //VALID # OF ARGUMENTS
                    }
                    char *dir_path = dest[1];
                    int chdir_rc = chdir(dir_path);
                    if(chdir_rc == -1){
                        char error_message[30] = "An error has occurred\n";
                        write(STDERR_FILENO, error_message, strlen(error_message)); 
                    }
                }
                if(i == 2){
                    // PATH BIC
                    //FIRST CLEAR DATA STRUCT
                    for(int k = 0; (k < paths_num) && (paths[k] != NULL); k++){
                        paths[k] = NULL;//de-allocates each char pointer
                    }
                    // ASSIGN PATHS
                    for(int j = 1; (j < (num_args+1)) && (j < paths_num); j++){
                        paths[j -1] = dest[j];
                        
                        printf("paths[%i]: %s\n", j-1, paths[j-1]);
                    }
                    paths_used = num_args;//update num of paths
                }
                //COMMAND IDENTIFIED -> BREAK LOOP
                break;
            }
        }
        // FOUND A BIC
        // Continue to next cmd
        if(is_bic == 0){
            continue;
        }

        // NOT A BUILT IN COMMAND -> check for corresponding binary in path

        // TODO REIMPLEMENT -> GENERALIZE AND SEARCH PATH FOR ARG
        // check if user arg is a CMD
        char *path_copy; // char pointer holding a copy of path
        int rc; //return code of ls access

        //check paths given for prog
        for(int m = 0; m < paths_used; m++){
            //form path
            path_copy = malloc(100);
            memcpy(path_copy, paths[m], strlen(paths[m]));
            strcat(path_copy,"/");
            strcat(path_copy, dest[0]);//concat target prog name

            //attempt to access with path
            // printf("path_copy: %s\n", path_copy);
            rc = access(path_copy, X_OK);

            if(rc == 0){
                int execv_rc = execv(path_copy, dest);
                if (execv_rc == -1){
                    //error while performing execution
                    char error_message[30] = "An error has occurred\n";
                    write(STDERR_FILENO, error_message, strlen(error_message)); 
                }
            }else{
                free(path_copy);//failed access, try again
            }
        }
        //tried to access with all paths, failed
        char error_message[30] = "An error has occurred\n";
        write(STDERR_FILENO, error_message, strlen(error_message));
        //END OF WHILE
    }

    free(line);
    fclose(stdin);
    exit(0);

    
    //end of main
    return 0;
}