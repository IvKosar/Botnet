#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define PORT 8080

int main(int argc, char* argv[]){

  int sock = 0, valread;
  struct sockaddr_in serv_addr;
  //int addrlen = sizeof(address);
  char buffer[2048] = {0};

  if((sock = socket(AF_INET, SOCK_STREAM, 0)) == 0){
    printf("%s\n", "Unable to open connection");
    return -1;
  }

  memset(&serv_addr, '0', sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0){
    printf("%s\n", "Invalid IP Address");
    return -2;
  }

    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){
      printf("%s\n", "Unable to connect");
      return -3;
    }
  FILE* fl;
  fl = fopen("confr.txt", "w");
  while(read(sock, buffer, 2048) > 0){
      //printf("%s", buffer);
      fprintf(fl, "%s", buffer);
  }
  fclose(fl);
  printf("%s\n", "Message received");
  //printf("%s%s\n", "Message: ", buffer);


  char* args[] = {"./bot", "confr.txt", NULL};
    pid_t pid = fork();
    printf("%d\n", pid);
    if(pid == -1){
        printf("%s\n", "Faild to fork");
        return 1;
    }else if (pid == 0){
      printf("%s\n", "Run bot");
      execv("bot", args);
    }else{
      int status;
      while (wait(&status) != pid){}
    }

  return 0;
}
