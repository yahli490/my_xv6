#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"


void find(char * path, char * search){
  int fd;
  struct stat st;
  struct dirent de;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
    case T_DEVICE:

    case T_FILE:
      int n = 0; 
      for (int i = 0; i < strlen(path); ++i)
        if(path[i] == '/') n = i + 1; 

      if (strcmp(search, path + n) == 0){
        write(1, path, strlen(path));
        write(1, "\n", 1);
      }  
      break;

    case T_DIR:

      while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if(de.inum == 0)
          continue;
          
        if(strcmp(de.name, ".") == 0)
          continue; 

        if (strcmp(de.name, "..") == 0)
          continue;

        char * newpath = (char*) malloc(strlen(path) + 2 + strlen(de.name) + 1000); 
        memmove(newpath, path, strlen(path)); 
        newpath[strlen(path)] = '/'; 
        memmove(newpath + strlen(path) + 1, de.name, strlen(de.name)); 
        newpath[strlen(path) + 1 + strlen(de.name)] = 0;  

        find(newpath, search); 
        free(newpath); 
      }
      break;
    }

  close(fd);
}

int main(int argc, char *argv[]){
  if (argc != 3){
    printf("Wrong usage..."); 
    return -1; 
  }

  find(argv[1], argv[2]); 
  return 0; 
}
