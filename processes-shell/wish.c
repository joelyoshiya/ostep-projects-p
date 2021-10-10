#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char *argv[]){
    
    //check cl args
    if(argc > 2){
        printf("error: too many arguments provided\n");
        return 0;//error, more than a single argument
    }
    if(argc == 2){
        //batch mode, second argument must be a text file to read input from
        if(strcmp(argv[0],"./wish")== 0){
            printf("Two arguments: Batch Mode with argument: %s \n",argv[1]);
        }
    }else{
        //interactive mode
        //while loop - condition: if the next input is not exit        FILE *stream;
        
        char *line = " ";
        size_t len = 0;
        ssize_t nread;

        //MAIN LOOP
        while (strcmp(line,"exit\n") != 0) {
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
            char *dest[10];
            char *copy = line;
            int i;
            for (i = 0; i < 10 && (dest[i] = strsep(&copy, " ")) != NULL; i++)
                ;   
            for (int c = 0; c < i; c++)
                printf(" arg %d : [%s]\n", c, dest[c]); 


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