// Create a zombie process that
// must be reparented at exit.
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"


const int MAX_READ = 1000; 
char buf[1000]; 


void run_command(int argc, char ** argv){
    int c = fork(); 
    if (c != 0){
        wait(&c); 
        return; 
    }

    for (int i = 1; i < argc; ++i)
        argv[i - 1] = argv[i]; 
    argv[argc - 1] = buf; 

    exec(argv[0], argv); 
}

int main(int argc, char ** argv){
    int count = 0; 
    char inp = 0; 

    while (read(0, &inp, 1)){
        if (inp == '\n'){
            run_command(argc, argv); 

            for (int i = 0; i < count; ++i)
                buf[i] = 0; 
            count = 0; 
        }

        else buf[count++] = inp; 
    }
    
    return 0; 
}
