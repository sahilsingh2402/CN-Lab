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
#define PORT 60018

int main()
{
    int sockfd, i;
    char buff[100];
    struct sockaddr_in sa;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("socket not created");
        exit(1);
    }

    sa.sin_family = AF_INET;
    sa.sin_port = PORT;
    sa.sin_addr.s_addr = inet_addr("127.0.0.1");

    i = connect(sockfd, (struct sockaddr *)&sa, sizeof(sa));
    for (i = 0; i < 100; i++)
    {
        recv(sockfd, buff, 100, 0);
        printf("%s\n", buff);
    }
    for (i = 0; i < 100; i++)
    {
        buff[i] = '\0';
        strcpy(buff, "Message from client");
        send(sockfd, buff, 100, 0);
    }
    close(sockfd);
    return 0;
}