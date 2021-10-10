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
        if(strcmp(argv[0],"./wish")== 0 && strcmp(argv[1],"batch.txt")== 0){
            printf("Two arguments: batch mode\n");
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
            // if(getc(stdin) == EOF){
            //     exit(0);
            // }
            printf("Retrieved line of length %zu:\n", nread);
            //fwrite(line, nread, 1, stdout);
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