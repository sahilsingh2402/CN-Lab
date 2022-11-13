#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
void sel_sort(int arr[], int n)
{
    int i, j, k, temp;
    for (i = 0; i < n; i++)
    {
        k = i;
        for (j = i + 1; j < n; j++)
        {
            if (arr[k] > arr[j])
                k = j;
        }
        temp = arr[i];
        arr[i] = arr[k];
        arr[k] = temp;
    }
}
int main()
{
    int sockfd, fd1, i;
    struct sockaddr_in sa, ta1;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = INADDR_ANY;
    sa.sin_port = htons(6002);

    i = bind(sockfd, (struct sockaddr *)&sa, sizeof(sa));
    printf("test %d %d\n", sockfd, i);

    listen(sockfd, 5);

    int length = sizeof(sa);
    fd1 = accept(sockfd, (struct sockaddr *)&ta1, &length);

    char buf[100];
    for (i = 0; i < 100; i++)
        buf[i] = '\0';
    int size;
    recv(fd1, buf, 100, 0);
    recv(fd1, &size, sizeof(int), 0);

    int arr[size];
    recv(fd1, &arr, sizeof(arr), 0);

    printf("%s\n", buf);
    printf("\nArray from client: ");
    int k;
    for (k = 0; k < size; k++)
        printf("%d ", arr[k]);
    printf("\n");

    sel_sort(arr, size);
    for (i = 0; i < 100; i++)
        buf[i] = '\0';

    strcpy(buf, "Hello from Server");
    send(fd1, buf, 100, 0);
    send(fd1, &arr, sizeof(arr), 0);

    close(fd1);
    return 0;
}