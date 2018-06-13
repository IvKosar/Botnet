#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8080

int main(){
  int sock_fd, new_socket, valread;
  struct sockaddr_in address;
  int addrlen = sizeof(address);
  char buffer[2048] = {0};
  char* message[1024];
  int mes_len = 1024;

  if((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
    printf("%s\n", "Unable to open connection");
    return -1;
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  if(bind(sock_fd, (struct sockaddr*)&address, sizeof(address))< 0){

    printf("%s\n", "Unable to bind");
    return -2;

  }

  if(listen(sock_fd, 4) < 0){
    printf("%s\n", "Error while listening");
    return -3;
  }

  if((new_socket = accept(sock_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0){
    printf("%s\n", "Error while accept connection");
  }
  /*valread = read(new_socket, buffer, 2048);
  printf("%s\n","Message received");
  printf("%s%s\n", "Message: ", buffer);*/
  FILE* fp;
  FILE* fl;
  char buffr[255];

  fl = fopen("ind.txt", "r");
  while(fl == NULL){
    fl = fopen("ind.txt", "r");
  }

  fclose(fl);
  fp = fopen("conf.txt", "r");
  if(fp == NULL){
    printf("%s\n", "Unable open the configuration file");
    return 1;
  }
  while(fgets(buffr, 255, (FILE*) fp)) {
      printf("%s\n", buffr);
      send(new_socket, buffr, strlen(buffr), 0);
  }

  fclose(fp);

  printf("%s\n","Message send");
  return 0;

}
