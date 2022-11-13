#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
int main()
{
    int sockfd, i;
    struct sockaddr_in sa;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr("127.0.0.1");
    sa.sin_port = htons(6002);

    i = connect(sockfd, (struct sockaddr *)&sa, sizeof(sa));
    printf("%d\n", i);

    int arr[] = {30, 10, 50, 8, 9, 20};
    int l = sizeof(arr);
    int size = l / sizeof(int);
    int s_arr[size];
    char buf[100];

    for (i = 0; i < 100; i++)
        buf[i] = '\0';

    strcpy(buf, "Hello from Client!!!");
    send(sockfd, buf, 100, 0);
    send(sockfd, &size, sizeof(int), 0);
    send(sockfd, &arr, l, 0);

    for (i = 0; i < 100; i++)
        buf[i] = '\0';

    recv(sockfd, buf, 100, 0);
    printf("%s\n", buf);
    recv(sockfd, &s_arr, sizeof(s_arr), 0);
    printf("\nSorted Array from server: ");
    int k;
    for (k = 0; k < size; k++)
        printf("%d ", s_arr[k]);
    printf("\n");

    close(sockfd);
    return 0;
}