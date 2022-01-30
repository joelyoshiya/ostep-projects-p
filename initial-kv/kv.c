#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "kv.h"

extern int errno;//for error numbers

// - *put*: The format is `p,key,value`, where `key` is an integer, and
// `value` an arbitrary string (without commas in it).
// needed this to help:https://stackoverflow.com/questions/49999948/how-to-use-malloc-outside-main
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
// TODO: #2 seg fault when this op performed, deleting seg faults when key is found
int delete(int key, Kv **head){
    //BASE CASE 0 
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
    //at start of the linked list
    if(currNode->prev == NULL){
        //only one node
        if(currNode->next == NULL){
            free(currNode);
        }else{
            //deleting the first node off the LL
            *head = currNode->next;
            free(currNode);
        }
    }
    //not at start of linked list
    else{
        // at then end of the linked list
        if(currNode->next == NULL){
            currNode->prev->next = NULL;
            free(currNode);
        }
        // somewhere in the middle (have to reconnect both parts)
        else{
            currNode->prev->next = currNode->next;
            currNode->next->prev = currNode->prev;
            free(currNode);            
        }
    }
    return 0;
}
// - *clear*: The format is `c`. This command simply removes all
// key-value pairs from the database.
int clear(Kv **head){
    if(*head == NULL){
        return 0;
    }
    Kv *currNode = *head;
    Kv *next;
    do{
        printf("%i to be deleted\n", currNode->key);
        next = currNode->next;
        free(currNode);
        currNode = next;
    }while(currNode->next != NULL);
    //at end of the linked list
    free(currNode);
    free(head);
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

// TODO #1
//read in database.txt data file at startup
int read_db(Kv **head){
    FILE *fp = fopen("database.txt","r+");
    if (fp == NULL){
        //cannot read in database
        return -1;
    }

    // https://c-for-dummies.com/blog/?p=1112
    char *buffer;
    size_t bufsize = 32;
    char *kv[2];

    buffer = (char *)malloc(bufsize * sizeof(char));
    if( buffer == NULL)
    {
        perror("Unable to allocate buffer");
        exit(1);
    }



    while(getline(&buffer,&bufsize,fp) != -1){
        buffer[strcspn(buffer, "\n")] = '\0';  /* Zap trailing newline */
        char *st = buffer;
        for (int i = 0; i < 2 && (kv[i] = strsep(&st, ",")) != NULL; i++)
        ;
        //printf("Input: %s,%s\n",kv[0],kv[1]);
        put(atoi(kv[0]), kv[1], head);
    }

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
    int args = 0;
    Kv *head = NULL;
    Kv **hd_ptr = &head;
    //load the current database into the data struct
    read_db(hd_ptr);

    //check if no arguments
    if(argc < 2){
        return 0;
    }
    
    // CHECK: any bad commands found in CL input
    int good_cmd;
    char* cmdtable[5] = {"p", "g", "a", "c","d"};

    //Go over every argument
    for(int j = 1; j < argc; ++j)
    {  
        good_cmd = 0;
        //check if the current arg is valid
        char *cmd_input = argv[j];
        char cmd = cmd_input[0];
        for(int k = 0; k < 5; ++k){
            if(cmd == *cmdtable[k]){
                good_cmd = 1;
                break;
            }
        }
        if(!good_cmd){
            printf("%s\n", "bad command");
            continue;//keep processing rest of command line
        }

        //Split string on "," as delimiter
        char *dest[1000];
        // referred to: https://stackoverflow.com/questions/50915364/how-strsep-works-in-c
        for (int i=0;(dest[i]=strsep(&argv[j],","))!=NULL;i++){
            args += 1;
        }
        char in_cmd = *dest[0]; 
     
        //checking for command type
        if((in_cmd == 'p') && (args > 1))
        {
            put(atoi(dest[1]),dest[2],hd_ptr);

        }else if ((in_cmd == 'g') && (args > 0))
        {
            get(atoi(dest[1]),hd_ptr);
        }else if ((in_cmd == 'd') && (args > 0))
        {
            delete(atoi(dest[1]),hd_ptr);
        }else if (in_cmd == 'c')
        {
            clear(hd_ptr);
        }else if (in_cmd == 'a')
        {
            all(hd_ptr);
        }
    }//end of arg checking

    write_db(hd_ptr);

    return 0;
}