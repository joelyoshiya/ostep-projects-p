#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>


int main(int argc, char *argv[]){

    /// GLOBAL VARS
    // TODO REIMPLEMENT WITH AN ARRAY OF PATHS
    char *path = "/bin";

    static char* built_in_cmds[3] = {"exit","cd","path"};
    int num_bic = 3;
    
    //check cl args
    if(argc > 2){
        printf("error: too many arguments provided\n");
        exit(1);//error, more than a single argument
    }
    if(argc == 2){
        //batch mode, second argument must be a text file to read input from
        if(strcmp(argv[0],"./wish")== 0){
            //printf("Two arguments: Batch Mode with argument: %s \n",argv[1]);
        }
        // try to open file and store txt -> if fails, exit(1)
    }else{
        //interactive mode
        //while loop - condition: if the next input is not exit        FILE *stream;
        
        char *line = " ";
        size_t len = 0;
        ssize_t nread;

        //MAIN LOOP
        while (strcmp(line,"exit") != 0) {
            printf("wish> ");
            nread = getline(&line, &len, stdin);
            if(nread == EOF){
                //try ctrl-d to test
                printf("EOF returned to nread\n"); 
                exit(0);
            }
            //PROCESS USER INPUT ex: "ls -la /tmp"
            // printf("Retrieved line of length %zu:\n", nread);
            //fwrite(line, nread, 1, stdout);
            char *dest[100];
            char *copy = line;
            int i;

            //https://stackoverflow.com/questions/50915364/how-strsep-works-in-c
            copy[strcspn(copy, "\n")] = '\0';//removes trailing newline

            for (i = 0; i < 10 && (dest[i] = strsep(&copy, " ")) != NULL; i++)
                ;   
            // for (int c = 0; c < i; c++)
            //     printf(" arg %d : [%s]\n", c, dest[c]); 
                        int num_args = i;
            printf("Num args: %i\n", num_args);

            //parsed args in dest, cleaned so no newline

            // BUILT-IN-COMMANDS CHECK
            for(int i = 0; i < num_bic; i++){
                if(strcmp(dest[0], built_in_cmds[i]) == 0){
                    //process command
                    //continue since we know command is built in
                    // (don't need to check for other commands)
                    if(i == 0){
                        //call exit
                        exit(0);
                    }
                    if(i == 1){
                        /*cd: cd always take one argument (0 or >1 args should be 
                        signaled as an error). To change directories, 
                        use the chdir() system call with the argument 
                        supplied by the user; if chdir fails, 
                        that is also an error.*/
                        //call cd
                    }
                    if(i == 2){
                        /*path: The path command takes 0 or more arguments, 
                        with each argument separated by whitespace 
                        from the others. A typical usage would be like this: 
                        wish> path /bin /usr/bin, 
                        which would add /bin and /usr/bin to the search path of the shell. 
                        If the user sets path to be empty, 
                        then the shell should not be able to run any programs 
                        (except built-in commands). 
                        The path command always overwrites the old path
                         with the newly specified path.*/
                    }
                    break;
                }
            }

            // NOT A BUILT IN COMMAND -> check for corresponding binary in path

            // TODO REIMPLEMENT -> GENERALIZE AND SEARCH PATH FOR ARG
            // check if cmd: LS
            if(strcmp(dest[0],"ls")==0){
                //run ls proc
                char *path_copy = malloc(20);
                memcpy(path_copy, path, strlen(path));
                strcat(path_copy,"/ls");
                int ls_rc = access(path_copy, X_OK);

                //TODO REIMPLEMENT CHECKING THROUGH PATHS ARRAY
                if(ls_rc == -1){
                    free(path_copy);
                    path_copy = "usr/bin/ls";
                    ls_rc = access(path_copy, X_OK);
                }

                if(ls_rc == 0){
                    //ready to run this ls process
                    //int execv(const char *pathname, char *const argv[]);
                    int ls_execv_rc = execv(path_copy, dest);
                    if(ls_execv_rc == -1){
                        //error from execv - ls
                        char error_message[30] = "An error has occurred\n";
                        write(STDERR_FILENO, error_message, strlen(error_message)); 
                    }

                }else{
                    //error from access
                    char error_message[30] = "An error has occurred\n";
                    write(STDERR_FILENO, error_message, strlen(error_message)); 
                }
            }
            // ARG NOT FOUND IN PATH
            // -> RAISE ERROR?
            // char error_message[30] = "An error has occurred\n";
            // write(STDERR_FILENO, error_message, strlen(error_message));
            //break; //exits loop and exits program 
            //END OF WHILE
        }

        free(line);
        fclose(stdin);
        exit(0);
        // Interactive mode
        // getLine() & strsep()

    }
    //end of main
    return 0;
}