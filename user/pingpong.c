#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


char * buffer[100]; 


int main(int argc, char *argv[]){
    int* pipeid =(int*) malloc(sizeof(int) * 2);
    pipe(pipeid);
    int id = fork(); 

    if (id == 0){ //Child 
        read(pipeid[0], buffer, 1); 
        printf("%d: received ping\n", getpid());
        write(pipeid[1], buffer, 1);

    } else { //Parent
        write(pipeid[1], buffer, 1);
        read(pipeid[0], buffer, 1); 
        printf("%d: received pong\n", getpid());

    } 

    exit(0);
}
