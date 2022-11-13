#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>
#define PORT 6001

int main()
{
    int sockfd, i, fdl, length;
    char buff[100];
    struct sockaddr_in sa, tal;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("socket not created");
        exit(1);
    }

    sa.sin_family = AF_INET;
    sa.sin_port = PORT;
    sa.sin_addr.s_addr = INADDR_ANY;

    i = bind(sockfd, (struct sockaddr *)&sa, sizeof(sa));
    printf("test %d %d\n", sockfd, i);

    listen(sockfd, 5);

    length = sizeof(sa);
    fdl = accept(sockfd, (struct sockaddr *)&tal, length);
    for (i = 0; i < 100; i++)
    {
        buff[i] = '\0';
        strcpy(buff, "Message from client");
        send(fdl, buff, 100, 0);
    }

    for (i = 0; i < 100; i++)
    {
        buff[i] = '\0';
        recv(fdl, buff, 100, 0);
        printf("%s\n", buff);
    }
    close(sockfd);
    return 0;
}