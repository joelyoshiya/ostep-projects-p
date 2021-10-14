#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <sys/wait.h>
#include <fcntl.h>



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
    char *PATH[paths_num];
    PATH[0] = "/bin";
    PATH[1] = "/usr/bin"; //check if prog is here if needed
    //BUILT IN COMMANDS
    static char* built_in_cmds[3] = {"exit","cd","path"};
    int num_bic = 3;
    int is_bic = -1;
    // REDIRECTION
    int redirect_on = -1;
    char *output_filename;
    
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
        //reset loop conditionals
        is_bic = -1;
        redirect_on = -1;

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

        for (i = 0; i < 100 && (dest[i] = strsep(&copy, " ")) != NULL; i++);   
        
        // for (int c = 0; c < i; c++)
        //         printf(" dest[%d] : [%s]\n", c, dest[c]);   

        int num_args = i - 1; //prog name implicit (+1 for total # of args passed in by user)
        //printf("Num args: %i\n", num_args);
        // i = total amount of args

        int red_sym_location;
        int output_name_location;
        int parse_error = -1;
        //HANDLE REDIRECTION




        //OLD IMPLEMENTATION
        for(int n = 0; n < i; n++){
            if(strcmp(dest[n], ">") == 0){
                //Found a Redirection symbol
                //check if only one arg to right of it
                if(n != (i - 2)){
                    //too many output arguments
                        char error_message[30] = "An error has occurred\n";
                        write(STDERR_FILENO, error_message, strlen(error_message)); 
                        parse_error = 0;
                        //exit(1);
                        // TODO skip passed rest of command parsing?
                        break;
                }else{
                    redirect_on = 0; //redirect is allowed
                    num_args = n;
                    red_sym_location = n;
                    output_name_location = n + 1;
                    output_filename = dest[n+1];
                    break;
                }
            }
        }
        // update dest to only hold input args
        if(redirect_on == 0){
            //go into dest and set last two args ">" and "output_name" to null
            dest[red_sym_location] = NULL;
            dest[output_name_location] = NULL;
        }else if(parse_error == 0){
            continue;//exit while loop
        }

        // for (int t = 0; t < num_args; t++)
        // printf(" dest[%d] : [%s]\n", t, dest[t]);   

        // BUILT-IN-COMMANDS CHECK
        for(int i = 0; (i < num_bic) && (redirect_on != 0); i++){
            if(strcmp(dest[0], built_in_cmds[i]) == 0){
                is_bic = 0;
                //process command
                //continue since we know command is built in
                // (don't need to check for other commands)
                if(i == 0){
                    //only can accept the exit cmd and no add'l args
                    if(num_args == 0){
                        // Exit BIC -> Exit SHELL
                        // call exit
                        // free the allocated line and close standard input
                        free(line);
                        fclose(stdin);
                        exit(0);     
                    }else{
                        char error_message[30] = "An error has occurred\n";
                        write(STDERR_FILENO, error_message, strlen(error_message)); 
                    }
          
                }
                if(i == 1){
                    // CD BIC
                    if((num_args == 0) | (num_args > 1)){
                        char error_message[30] = "An error has occurred\n";
                        write(STDERR_FILENO, error_message, strlen(error_message)); 
                    }else{
                            //VALID # OF ARGUMENTS
                            char *dir_path = dest[1];
                            int chdir_rc = chdir(dir_path);
                            if(chdir_rc == -1){
                                char error_message[30] = "An error has occurred\n";
                                write(STDERR_FILENO, error_message, strlen(error_message)); 
                            }else{
                                //successful
                                //printf("successful dir_path: %s \n",dir_path);
                                break;//don't need to check for other-built ins
                            }
                    }
                }
                if(i == 2){
                    char *path = malloc(100);
                    // PATH BIC
                    //FIRST CLEAR DATA STRUCT
                    for(int k = 0; k < paths_num; k++){
                        PATH[k] = NULL;//de-allocates each char pointer
                    }
                    // ASSIGN PATHS
                    for(int j = 0; (j < num_args) && (j < paths_num); j++){
                        strcpy(path, dest[j+1]);
                        PATH[j] = path;
                    }
                    paths_used = num_args;//update num of paths
                    // for(int l = 0; l < paths_used; l++){
                    //     printf("PATH[%i]: [%s]\n", l, PATH[l]);
                    // }
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
        int run_success = -1;

        //check paths given for prog
        for(int m = 0; m < paths_used; m++){
            //form path
            //printf("current path tested: [%s] \n", PATH[m]);
            // printf("current prog arg tested: [%s] \n", dest[0]);

            path_copy = malloc(100);
            memcpy(path_copy, PATH[m], strlen(PATH[m]));
            strcat(path_copy,"/");
            strcat(path_copy, dest[0]);//concat target prog name

            //attempt to access with path
            // printf("path_copy: %s\n", path_copy);
            access_rc = access(path_copy, X_OK);

            if(access_rc == 0){
                int fork_rc = fork();
                if(fork_rc < 0){
                    //fork failed -> exit
                    exit(0);
                }else if(fork_rc == 0){
                    if(redirect_on == 0){
                        //REDIRECT
                        close(STDOUT_FILENO);
                        open(output_filename, O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
                        int execv_rc = execv(path_copy, dest);
                        if (execv_rc == -1){
                            //error while performing execution
                            char error_message[30] = "An error has occurred\n";
                            write(STDERR_FILENO, error_message, strlen(error_message));
                            exit(0);  
                        }else{
                            //successful
                            run_success = 0;
                            exit(0);
                        }
                    }else{
                        //NON REDIRECT
                        int execv_rc = execv(path_copy, dest);
                        if (execv_rc == -1){
                            //error while performing execution
                            char error_message[30] = "An error has occurred\n";
                            write(STDERR_FILENO, error_message, strlen(error_message));
                            exit(0); 
                        }else{
                            //successful
                            run_success = 0;
                            exit(0);
                        }

                    }
                }else{
                    //parent
                    int wc = wait(NULL);
                    assert(wc >= 0);
                    run_success = 0;
                    break;//don't need to check for other valid paths
                }                
            }else{
                //printf("Access not ok\n");
                free(path_copy);//failed access, try again
            }
        }
        //check if proc ran successfully, otherwise tried all paths and failed
        if(run_success != 0){
            char error_message[30] = "An error has occurred\n";
            write(STDERR_FILENO, error_message, strlen(error_message));
        }else{
            //ready for next cmd
            continue;
        }
        //END OF WHILE
    }
    //end of main
    return 0;
}