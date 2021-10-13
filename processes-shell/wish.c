#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <sys/wait.h>


//testing: ~cs537-1/tests/p2a/test-wish.sh -c

int main(int argc, char *argv[]){

    // GLOBAL VARS

    // RUN MODE: 0 -> Interactive, 1 -> Batch
    int rm = 0;
    //BATCH MODE VARS
    FILE *fp;
    // INTERACTIVE mode vars
    char *line = " ";
    size_t len = 0;
    ssize_t nread;
    // PATH GLOBAL VARS - See Piazza: @376
    int paths_num = 100;
    int paths_used = 2;
    char *paths[paths_num];
    paths[0] = "/bin";
    paths[1] = "/usr/bin"; //check if prog is here if needed
    //BUILT IN COMMANDS
    static char* built_in_cmds[3] = {"exit","cd","path"};
    int num_bic = 3;
    
    //check cl args
    if(argc > 2){
        char error_message[30] = "An error has occurred\n";
        write(STDERR_FILENO, error_message, strlen(error_message)); 
        exit(1);//error, more than a single argument
    }else if(argc == 2){
        rm = 1; // Run mode indicator: BATCH
        fp = fopen(argv[1],"r");
        if (fp == NULL){
            exit(1);// BAD FILE
        }
    }   

    //MAIN LOOP
    while (line != NULL) {
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
            //this is where you open up the file
            nread = getline(&line, &len, fp);
            if(nread == EOF){
                //try ctrl-d to test
                //printf("EOF returned to nread\n");
                exit(0);
            }else if(nread == -1){
                //ERROR READING LINE -> PRINT ERROR
                char error_message[30] = "An error has occurred\n";
                write(STDERR_FILENO, error_message, strlen(error_message)); 
            }
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
                    // Exit BIC -> Exit SHELL
                    // call exit
                    // free the allocated line and close standard input
                    free(line);
                    fclose(stdin);
                    exit(0);
                }
                if(i == 1){
                    // CD BIC
                    if((num_args == 0) | (num_args > 1)){
                        char error_message[30] = "An error has occurred\n";
                        write(STDERR_FILENO, error_message, strlen(error_message)); 
                    }else{
                        int fork_rc = fork();
                        if(fork_rc < 0){
                            //fork failed -> exit
                            exit(0);
                        }else if(fork_rc == 0){
                            //VALID # OF ARGUMENTS
                            char *dir_path = dest[1];
                            int chdir_rc = chdir(dir_path);
                            if(chdir_rc == -1){
                                char error_message[30] = "An error has occurred\n";
                                write(STDERR_FILENO, error_message, strlen(error_message)); 
                                exit(0);
                            }else{
                                //successful
                                exit(0);
                            }
                        }else{
                            int wc = wait(NULL);
                            assert(wc >= 0);
                            break;//don't need to check for other bult-ins
                        }
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
                    break; // don't check for other built-ins
                }
            }
        }
        // FOUND A BIC - continue to next cmd
        if(is_bic == 0){
            continue;//skips the execv proc control flow below
        }

        // NOT A BUILT IN COMMAND -> check for corresponding binary in path

        char *path_copy; // char pointer holding a copy of path
        int access_rc; //return code of ls access

        //check paths given for prog
        for(int m = 0; m < paths_used; m++){
            //form path
            path_copy = malloc(100);
            memcpy(path_copy, paths[m], strlen(paths[m]));
            strcat(path_copy,"/");
            strcat(path_copy, dest[0]);//concat target prog name

            //attempt to access with path
            // printf("path_copy: %s\n", path_copy);
            access_rc = access(path_copy, X_OK);

            if(access_rc == 0){
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
    //end of main
    return 0;
}