#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// - *put*: The format is `p,key,value`, where `key` is an integer, and
// `value` an arbitrary string (without commas in it).
int put(int key, char value[], struct kv** kv_pairs){
    // printf("key: %i \n",key);
    // printf("value: %s \n",value);
    return 0;
}

// - *get*: The format is `g,key`, where `key` is an integer. If the key
// is present, the system should print out the key, followed by a comma,
// followed by the value, followed by a newline (`\n`). If not present,
// print an error message on a line by itself, of the form `K not found`
// where `K` is the actual value of the key, i.e., some integer. 
int get(int key){
    return 0;
}

// - *delete*: The format is `d,key`, which either deletes the relevant
// key-value pair (and prints nothing), or fails to do so (and prints
// `K not found` where `K` is the actual value of the key, i.e., some
// integer).
int delete(int key){
    return 0;
}
// - *clear*: The format is `c`. This command simply removes all
// key-value pairs from the database.
int clear(){
    return 0;
}
// - *all*: The format is `a`. This command prints out all key-value
// pairs in the database, in any order, with one key-value pair per line,
// each key and value separated by a comma.
int all(){
    return 0;
}

int main(int argc, char *argv[]){
    // printf("0th arg: %s\n",argv[0]);
    // printf("1st arg: %s\n",argv[1]);
    // printf("2nd arg: %s\n",argv[2]);

    //allowed no, one, or two CL Args, handle arguments based on command:

    //check if no arguments
    if(argc < 2){
        return 0;
    }
    // check if there is a bad command,
    // if there is, read next char for correct input
    // otherwise store command and continue

    
    // CHECK: any bad commands found in CL input
    int val_cmd = 0; 
    int ind = 1; 
    char cmd;
    while(ind < argc){
        cmd = *argv[ind];
        if((cmd != 'p')&(cmd != 'g')&(cmd != 'a')&(cmd != 'c')&(cmd != 'd')){
        printf("%s \n", "bad command");
        ind++;//get next input
        }else{
            val_cmd = 1;
            break;
        }
    }
    if(!val_cmd){
        // TODO remove if needed at end
        printf("No valids commands found\n");
        return 0;
    }

    int npairs = 100; //number of key value structs allocated within array
    struct kv{
        int key; 
        char *value;
        };
    struct kv *kv_pairs = malloc(npairs * sizeof(struct kv));   

    for(int j = 1; j < argc; j++){
         //Split string
        //char *command;
        char *dest[10];
        // printf("%s\n",command);
        int i;
        // referred to: https://stackoverflow.com/questions/50915364/how-strsep-works-in-c
        for (i=0;(dest[i]=strsep(&argv[j],","))!=NULL;i++){
            continue;
        }
        // for (int c=0;c<i;c++) {
        //     printf(" arg %d : [%s] \n",c,dest[c]);
        //     }

        char in_cmd = *dest[0];  
        
        //checking for command type
        if(in_cmd == 'p')
        {
            put(atoi(dest[1]), dest[2], kv_pairs);
        }else if (in_cmd == 'g')
        {
            get(atoi(dest[1]));
        }else if (in_cmd == 'd')
        {
            delete(atoi(dest[1]));
        }else if (in_cmd == 'c')
        {
            clear();
        }else if (in_cmd == 'a')
        {
            all();
        }
        else
        {
            // printf("bad command\n");
        }

        }

        return 0;
}