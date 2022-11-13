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

#define SERVERPORT 6002
#define MAXBUFFLEN 200

int main()
{
	int sockfd;
	struct sockaddr_in their_addr;
	struct sockaddr_in my_addr;
	socklen_t addr_len;

	char buf[MAXBUFFLEN];
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
