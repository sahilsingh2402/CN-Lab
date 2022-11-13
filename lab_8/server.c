#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h> 
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#define PORT 8080
#define BUFSIZE 512
#define MSGSIZE 50
#define MAXCLIENTS 20
#define ADDRSIZE 100

struct client_info
{
    int sock_id;
    char *clientAddr;
    int status;
};

struct client_info activeClients[MAXCLIENTS + 1];
int maxClientID;

void list_online_clients()
{
    int i;
    printf("\n========= ONLINE CLIENTS =========\n");
    for (i = 1; i <= maxClientID; i++)
    {
        if (activeClients[i].status == 1)
        {
            printf("Client %d\n", i);
        }
    }
    printf("==================================\n");
}

void *connection_handler(void *sock_id)
{
    int sid_recv = *(int *)sock_id, clientID_recv, sid_send, clientID_send, i;
    char buffer[BUFSIZE], message[BUFSIZE], clientAddr[ADDRSIZE], dest[3];
    memset(buffer, 0, BUFSIZE);
    memset(message, 0, BUFSIZE);

    for (i = 1; i <= MAXCLIENTS; i++)
    {
        if (activeClients[i].sock_id == sid_recv)
        {
            clientID_recv = i;
            strcpy(clientAddr, activeClients[i].clientAddr);
            break;
        }
    }
    while (1)
    {
        if (recv(sid_recv, buffer, BUFSIZE, 0) > 0)
        {
            if (strcmp(buffer, "logout") == 0)
            {
                printf("\n(log) Client %d disconnected.\n", clientID_recv);
                activeClients[clientID_recv].status = 0;
                break;
            }
            else
            {
                printf("\n(rcv) Client %d : %s\n", clientID_recv, buffer);
                if (strcmp(buffer, "online?") == 0)
                {
                    list_online_clients(); // list on server side
                    strcat(message, "========= ONLINE CLIENTS =========\n");
                    char client[10];
                    for (i = 1; i <= maxClientID; i++)
                    {
                        if (activeClients[i].status == 1)
                        {
                            if (i != clientID_recv)
                            {
                                sprintf(client, "- Client %d\n", i);
                            }
                            else
                            {
                                sprintf(client, "- Client %d (YOU)\n", i);
                            }
                            strcat(message, client);
                        }
                    }
                    strcat(message, "==================================\n");
                    if (send(sid_recv, message, BUFSIZE, 0) < 0)
                    {
                        perror("(log) send failed");
                    }
                    memset(message, 0, BUFSIZE);
                }
                else
                {
                    for (i = 0; buffer[i] != ' '; i++)
                    {
                        dest[i] = buffer[i];
                    }
                    dest[i] = '\0';
                    clientID_send = atoi(dest);
                    sid_send = activeClients[clientID_send].sock_id;
                    if (clientID_send > 0)
                    {
                        if (clientID_send != clientID_recv)
                        {
                            if (clientID_send > maxClientID || activeClients[clientID_send].status == 0)
                            {
                                sprintf(message, "\nClient %d is offline.\n", clientID_send);
                                if (send(sid_recv, message, BUFSIZE, 0) < 0)
                                {
                                    perror("(log) send failed");
                                }
                                printf("(log) %s\n", message);
                            }
                            else
                            {
                                char temp[BUFSIZE];
                                int j;
                                for (j = i + 1; j < strlen(buffer); j++)
                                {
                                    temp[j - i - 1] = buffer[j];
                                }
                                temp[j - i - 1] = '\0';
                                sprintf(message, "Client %d : %s", clientID_recv, temp);
                                if (send(sid_send, message, BUFSIZE, 0) < 0)
                                {
                                    perror("(log) send failed");
                                }
                                printf("(log) Message sent from Client %d to Client %d.\n", clientID_recv, clientID_send);
                                memset(temp, 0, BUFSIZE);
                            }
                        }
                        else
                        {
                            strcpy(message, "! INVALID MESSAGE\n! Self-message not allowed\n! Enter 'online?' for list of online clients\n");
                            if (send(sid_recv, message, BUFSIZE, 0) < 0)
                            {
                                perror("(log) send failed");
                            }
                            printf("(sen) %s\n", message);
                        }
                    }
                    else
                    {
                        strcpy(message, "! INVALID MESSAGE\n! VALID FORMAT: <clientID><space><message>\n! Enter 'online?' for list of online clients\n");
                        if (send(sid_recv, message, BUFSIZE, 0) < 0)
                        {
                            perror("(log) send failed");
                        }
                        printf("(sen) %s\n", message);
                    }
                    memset(message, 0, BUFSIZE);
                }
            }
            memset(buffer, 0, BUFSIZE);
        }
    }
    close(sid_recv);
    pthread_exit(0);
}

int main()
{
    int sid, sid2, addrlen, opt = 1, clientID = 0, iret, i;
    pthread_t threads[MAXCLIENTS + 1];
    char clientAddr[ADDRSIZE], message[MSGSIZE];
    memset(message, 0, MSGSIZE);

    if ((sid = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(sid, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in clientaddr;
    clientaddr.sin_family = AF_INET;
    clientaddr.sin_port = htons(PORT);
    clientaddr.sin_addr.s_addr = INADDR_ANY;
    addrlen = sizeof(clientaddr);

    if (bind(sid, (struct sockaddr *)&clientaddr, addrlen) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(sid, 5) < 0)
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    while ((sid2 = accept(sid, (struct sockaddr *)&clientaddr, (socklen_t *)&addrlen)) >= 0)
    {
        inet_ntop(AF_INET, &(clientaddr.sin_addr), clientAddr, ADDRSIZE);
        maxClientID = clientID = clientID + 1;
        activeClients[clientID].clientAddr = strdup(clientAddr);
        activeClients[clientID].sock_id = sid2;
        activeClients[clientID].status = 1;
        printf("\n(log) Client %d (%s) connected.\n\n", clientID, clientAddr);
        sprintf(message, "========== Hello Client %d =========\n", clientID);
        if (send(sid2, message, MSGSIZE, 0) < 0)
        {
            perror("(log) send failed");
            exit(EXIT_FAILURE);
        }
        int *ptr = malloc(sizeof(int *));
        *ptr = sid2;
        if ((iret = pthread_create(&threads[clientID], NULL, connection_handler, (void *)ptr)))
        {
            fprintf(stderr, "pthread_create error : %d\n", iret);
            exit(EXIT_FAILURE);
        }
    }
    for (i = 1; i <= MAXCLIENTS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    exit(EXIT_SUCCESS);
}

// what does this above code do?

// what does this above code do?

// It's a chat server. It allows multiple clients to connect to it and chat with each other.
