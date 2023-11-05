#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


char TERMINATE = 100; 


void child_proc(int* pipeid){
    int* pipeidChild = malloc(sizeof(int) * 2);
    char* buffer = malloc(sizeof(char) * 100); 

    int count = 0; 
    while (1){
        read(pipeid[0], buffer + count, 1); 
        if (buffer[count] == TERMINATE) 
            break;
        count++; 
    }    

    if (count){
        pipe(pipeidChild); 
        printf("prime %d\n", buffer[0]); 
        for (int i = 1; i < count; ++i){
            if (buffer[i] % buffer[0])
                write(pipeidChild[1], buffer + i, 1); 
        }
        write(pipeidChild[1], buffer + count, 1); 
    }
    close(pipeid[0]); 
    close(pipeid[1]); 

    if (count && fork() == 0){
        child_proc(pipeidChild); 
        exit(0);
    }
}


int main(int argc, char *argv[]){
    int* pipeid =(int*) malloc(sizeof(int) * 2);
    char* buffer = malloc(sizeof(char)); 

    pipe(pipeid);
    int id = fork(); 

    if (id){
        for(int i = 2; i <= 35; ++i){
            buffer[0] = i; 
            write(pipeid[1], buffer, 1); 
        }
        buffer[0] = TERMINATE; 
        write(pipeid[1], buffer, 1); 
    }
    else child_proc(pipeid); 

    sleep(5); 
    exit(0);
}
