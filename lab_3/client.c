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

#define SERVERPORT 6000

int main()
{
	int sockfd;
	struct sockaddr_in their_addr;
	int numbytes;
	char arg[30];

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		perror("Socket not created!");
		exit(1);
	}

	their_addr.sin_family = AF_INET;
	their_addr.sin_port = htons(SERVERPORT);
	their_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // This is a lookback IP address!
	printf("Enter the message: ");
	gets(arg);

	if ((numbytes = sendto(sockfd, arg, strlen(arg), 0, (struct sockaddr *)&their_addr, sizeof their_addr)) == -1)
	{
		perror("Send Error");
		exit(1);
	}

	printf("Sent %d bytes to %s\n", numbytes, inet_ntoa(their_addr.sin_addr));
	close(sockfd);
	return 0;
}
