#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>

#define MYPORT 6000
#define MAXBUFFLEN 200

int main()
{
    int sockfd;
    struct sockaddr_in my_addr;
    struct sockaddr_in their_addr;
    socklen_t addr_len;

    int numbytes;
    char buf[MAXBUFFLEN];

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("Socket not created");
        exit(1);
    }

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(MYPORT);
    my_addr.sin_addr.s_addr = INADDR_ANY;

    if ((bind(sockfd, (struct sockaddr *)&my_addr, sizeof(my_addr)) == -1))
    {
        perror("Binding Error");
        exit(1);
    }

    addr_len = sizeof their_addr;
    if ((numbytes = recvfrom(sockfd, buf, MAXBUFFLEN - 1, 0, (struct sockaddr *)&their_addr, &addr_len)) == -1)
    {
        perror("Recieving Error");
        exit(1);
    }
    printf("Recived Packet from %s\n", inet_ntoa(their_addr.sin_addr));
    printf("Packet is %d bytes long\n", numbytes);
    buf[numbytes] = '\0';
    printf("Packet Contains \"%s\"\n", buf);
    close(sockfd);

    return 0;
}
