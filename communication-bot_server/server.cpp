#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>


int main(int argc, char *argv[])
{
    char sendBuff[1025];
    FILE *F;
    while(fopen("conf.txt", "r") == NULL){}
    printf("%s\n", "Got file");
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr;


    time_t ticks;

  //------------------------- create network socket -----------------------------
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff));

  //----------------------------- Initialize server -----------------------------
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000);

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    listen(listenfd, 10);

    while(1)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);

        ticks = time(NULL);

    //---------------------------  file read and send -----------
    FILE * fp;
      char * line = NULL;
        size_t len = 0;
        ssize_t read;

        fp = fopen("conf.txt", "r");
        if (fp == NULL)
            exit(EXIT_FAILURE);

        while ((read = getline(&line, &len, fp)) != -1) {
      snprintf(sendBuff, sizeof(sendBuff), "%s\r\n", line);
            write(connfd, sendBuff, strlen(sendBuff));
        }

        fclose(fp);
        if (line)
            free(line);
        //======================== end of file read and send
        close(connfd);
        sleep(1);
     }
    return 0;
}
