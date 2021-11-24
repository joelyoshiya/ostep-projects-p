typedef struct kv {
    int key;
    char value[100];
    struct kv *next;
    struct kv *prev;
}Kv;