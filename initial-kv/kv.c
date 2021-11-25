#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "kv.h"

// // referred to https://stackoverflow.com/questions/23279119/creating-and-understanding-linked-lists-of-structs-in-c/23280743
// typedef struct kv {
//     int key;
//     char value[100];
//     struct kv *next;
//     struct kv *prev;
// }Kv;

extern int errno;//for error numbers

// typedef struct kv_linked_list
// {
// /* Next-node pointer */
// Kv *next;     /* pointer to the next node in the list. */
// } Kv_linked_list;

// - *put*: The format is `p,key,value`, where `key` is an integer, and
// `value` an arbitrary string (without commas in it).
// neeeded this to help:https://stackoverflow.com/questions/49999948/how-to-use-malloc-outside-main
int put(int key, char value[], Kv **head){
    // printf("key: %i \n",key);
    // printf("value: %s \n",value);
    if(*head == NULL){
        //allocate mem for linked list
        *head = (Kv*)malloc(sizeof(Kv));
        //allocate space for node
        (*head)->key = key;
        strcpy((*head)->value,value); 
        // //print for now
        // printf("key: %i \n",(*head)->key);
        // printf("value: %s \n",(*head)->value);
    }else{
        //Kv* prevNode = NULL;
        Kv *currNode = *head;
        while(currNode->next != NULL){
            currNode = currNode->next;
        }
        //No kv node at next
        currNode->next = malloc(sizeof(Kv));
        currNode->next->key = key;
        currNode->next->prev = currNode;
        strcpy(currNode->next->value,value);

        // printf("currNode key: %i\n",currNode->key);
        // printf("currNode next: %i\n",currNode->next->key);
        // printf("nextNode previous: %i\n",currNode->next->prev->key);
    }

    return 0;
}

// - *get*: The format is `g,key`, where `key` is an integer. If the key
// is present, the system should print out the key, followed by a comma,
// followed by the value, followed by a newline (`\n`). If not present,
// print an error message on a line by itself, of the form `K not found`
// where `K` is the actual value of the key, i.e., some integer. 
int get(int key, Kv **head){
    if(*head == NULL){
        return 0;
    }
    //search for key
    Kv *currNode = *head;
    //search for key
    while(currNode->key != key){
        if(currNode->next == NULL){
            printf("%i not found\n",key);
            return 0;
        }   
        currNode = currNode->next;
    }
    //found kv pair with key
    printf("%i,%s\n",currNode->key,currNode->value);
    return 0;
}

// - *delete*: The format is `d,key`, which either deletes the relevant
// key-value pair (and prints nothing), or fails to do so (and prints
// `K not found` where `K` is the actual value of the key, i.e., some
// integer).
int delete(int key, Kv **head){
    if(*head == NULL){
        printf("%i not found\n",key);
        return 0;
    }   

    Kv *currNode = *head;
    //search for key
    while(currNode->key != key){
        if(currNode->next == NULL){
            printf("%i not found\n",key);
            return 0;
        }   
        currNode = currNode->next;
    }
    //now do delete operation
    currNode->prev->next = currNode->next;
    currNode->next->prev = currNode->prev;
    free(currNode);

    // Kv *checkNode = *head;
    // while(checkNode->next != NULL){
    //     checkNode = checkNode->next;
    // }
    // while(checkNode != NULL){
    //     printf("checknodes key: %i \n",checkNode->key);
    //     checkNode = checkNode->prev;

    // }
    
    return 0;
}
// - *clear*: The format is `c`. This command simply removes all
// key-value pairs from the database.
int clear(Kv **head){
    if(*head == NULL){
        return 0;
    }
    free(*head);
    return 0;
}
// - *all*: The format is `a`. This command prints out all key-value
// pairs in the database, in any order, with one key-value pair per line,
// each key and value separated by a comma.
int all(Kv **head){
    if(*head == NULL){
    }
    Kv *currNode = *head;
    while(currNode != NULL){
    printf("%i,%s\n",currNode->key,currNode->value);
    currNode = currNode->next;
    }
    return 0;
}

//read in database.txt data file at startup
int read_db(Kv **head){
    FILE *fp = fopen("database.txt","r+");
    if (fp == NULL){
        //cannot read in database
        return -1;
    }
    //read in line, add to data struct
    //char* line;
    //while(char)


    fclose(fp);

    //getline()
    //fclose()
    return 0;
}

//write out kv pairs to database.txt file right before exit
int write_db(Kv** head){
   FILE *fp = fopen("database.txt", "w+");
    if (fp == NULL) {
    fprintf(stderr, "Value of errno: %d\n", errno);
    fprintf(stderr, "Error opening the file: %s\n", strerror( errno ));
    perror("Error printed by perror");
	} else {
    Kv *currNode = *head;
        while(currNode!= NULL){
            fprintf(fp, "%d,%s\n",currNode->key, currNode->value);
            currNode = currNode->next;
        }
		fclose (fp);
	}

    return 0;
}

int main(int argc, char *argv[]){
    //Kv_linked_list *head = NULL;
    Kv *head = NULL;
    Kv **hd_ptr = &head;
    //load the current database into the data struct
    //read_db(hd_ptr);


    //check if no arguments
    if(argc < 2){
        return 0;
    }
    // check if there is a bad command,
    // if there is, read next char for correct input
    // otherwise store command and continue

    
    // CHECK: any bad commands found in CL input
    int val_cmd;
//    int ind = 1; 
    char* cmdtable[5] = {"p", "g", "a", "c","d"};
    // while(ind < argc){
    //     char *cmd_input = argv[ind];
    //     char cmd = cmd_input[0];

    //     for(int j = 0; j < 5; ++j){
    //         if(cmd == *cmdtable[j]){
    //              val_cmd = 1;
    //             break;
    //         }
    //     }
    //     if(val_cmd){
    //         break;
    //     }else{
    //         printf("%s \n", "bad command");
    //         ind++;//get next input
    //     }

    // }
    // if(!val_cmd){
    //     // TODO remove if needed at end
    //     printf("No valids commands found\n");
    //     return 0;
    // }
    // commands valid

    //Go over every argument
    for(int j = 1; j < argc; ++j){
        val_cmd = 0;
        //check if the current arg is valid
        char *cmd_input = argv[j];
        char cmd = cmd_input[0];
        for(int k = 0; k < 5; ++k){
            if(cmd == *cmdtable[k]){
                val_cmd = 1;
                break;
            }
        }
        if(!val_cmd){
            printf("%s\n", "bad command");
            continue;//keep processing rest of command line
        }

        //Split string on "," as delimiter
        char *dest[1000];
        // referred to: https://stackoverflow.com/questions/50915364/how-strsep-works-in-c
        for (int i=0;(dest[i]=strsep(&argv[j],","))!=NULL;i++){
            continue;
        }
        // for (int c=0;c<i;c++) {
        //     printf(" arg %d : [%s] \n",c,dest[c]);
        //     }
        printf("dest: %s \n", dest[0]);
        char in_cmd = *dest[0]; 
        printf("in_cmd input: %c \n", in_cmd);


        
        //checking for command type
        if(in_cmd == 'p')
        {
            put(atoi(dest[1]),dest[2],hd_ptr);
        }else if (in_cmd == 'g')
        {
            get(atoi(dest[1]),hd_ptr);
        }else if (in_cmd == 'd')
        {
            delete(atoi(dest[1]),hd_ptr);
        }else if (in_cmd == 'c')
        {
            clear(hd_ptr);
        }else if (in_cmd == 'a')
        {
            all(hd_ptr);
        }
        else
        {
            // printf("bad command\n");
        }

        }//end of arg checking
        write_db(hd_ptr);

        return 0;
}