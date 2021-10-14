int fork_rc = fork();
if(fork_rc < 0){
    //fork failed -> exit
    exit(0);
}else if(fork_rc == 0){
    //do some stuff
}else{
    //parent
    int wc = wait(NULL);
    assert(wc >= 0);
    run_success = 0;
    break; //break out of current loop (might not need)
} 