#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#define PORT 8080
#define BUFSIZE 512

void *receive_handler(void *sock_id)
{
    int sid = *(int *)sock_id;
    char buffer[BUFSIZE];
    memset(buffer, 0, BUFSIZE);
    while (recv(sid, buffer, BUFSIZE, 0) > 0)
    {
        printf("%s\n", buffer);
        memset(buffer, 0, BUFSIZE);
    }
    pthread_exit(NULL);
}

int main()
{
    int sid, iret1;
    pthread_t rthread;

    if ((sid = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in servaddr;
    memset(&servaddr, '0', sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);

    if (connect(sid, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }

    // Thread to receive
    int *ptr = malloc(sizeof(int *));
    *ptr = sid;
    if ((iret1 = pthread_create(&rthread, NULL, receive_handler, (void *)ptr)))
    {
        fprintf(stderr, "Error in pthread_create (receive) : %d\n", iret1);
        exit(EXIT_FAILURE);
    }

    // Send
    char message[BUFSIZE];
    memset(message, 0, BUFSIZE);
    while (1)
    {
        printf("\n");
        fgets(message, BUFSIZE, stdin);
        message[strlen(message) - 1] = '\0';
        if (send(sid, message, BUFSIZE, 0) < 0)
        {
            perror("send failed");
        }
        if (strcmp(message, "logout") == 0)
        {
            printf("\nYou are now offline.\n\n");
            break;
        }
        memset(message, 0, BUFSIZE);
    }

    pthread_join(rthread, NULL);

    close(sid);
    exit(EXIT_SUCCESS);
}

// what does this above code do?


