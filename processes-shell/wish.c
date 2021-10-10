#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]){

    /// GLOBAL VARS
    // TODO REIMPLEMENT WITH AN ARRAY OF PATHS
    char *path = "/bin";

    static char* built_in_cmds[3] = {"cd","exit","path"};
    int num_bic = 3;
    
    //check cl args
    if(argc > 2){
        printf("error: too many arguments provided\n");
        return 0;//error, more than a single argument
    }
    if(argc == 2){
        //batch mode, second argument must be a text file to read input from
        if(strcmp(argv[0],"./wish")== 0){
            //printf("Two arguments: Batch Mode with argument: %s \n",argv[1]);
        }
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

            //parsed args in dest

            // BUILT-IN-COMMANDS CHECK
            for(int i = 0; i < num_bic; i++){
                if(strcmp(dest[0], built_in_cmds[i]) == 0){
                    //process command
                    char *proc = built_in_cmds[i];
                    //continue since we know command is built in
                    // (don't need to check for other commands)
                    if(strcmp(proc, "exit")){
                        //call exit
                    }
                    if(strcmp(proc, "cd")){
                        //call cd
                    }
                    if(strcmp(proc, "path")){
                        //call path
                    }
                    continue;
                }
            }
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