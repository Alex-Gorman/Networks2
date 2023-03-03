/*
** listener.c -- a datagram sockets "server" demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
// #include "list.h"

#define MYPORT "4950"	// the port users will be connecting to

#define MAXBUFLEN 100

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[])
{

    if (argc != 3) {
		fprintf(stderr,"usage: receiver port window-size \n");
		exit(1);
	}

    if (atoi(argv[1]) < 30000 || atoi(argv[1]) > 40000) {
        fprintf(stderr,"only user ports 30000-40000 \n");
		exit(1);
    }

    // int windowSize = atoi(argv[2]);

	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	int numbytes;
	struct sockaddr_storage their_addr;
	char buf[MAXBUFLEN];
	socklen_t addr_len;
	// char s[INET6_ADDRSTRLEN];

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET6; // set to AF_INET to use IPv4
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if ((rv = getaddrinfo(NULL, argv[1], &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("listener: socket");
			continue;
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("listener: bind");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "listener: failed to bind socket\n");
		return 2;
	}

	freeaddrinfo(servinfo);

	printf("listener: waiting to recvfrom...\n");

	addr_len = sizeof their_addr;

	/* check if should send ack */
	char *y_for_send_ack = NULL;



    while (1) {

        if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0, (struct sockaddr *)&their_addr, &addr_len)) == -1) {
            perror("recvfrom");
            exit(1);
	    }

	    // printf("listener: got packet from %s\n", inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s));
        // printf("listener: packet is %d bytes long\n", numbytes);
        buf[numbytes] = '\0';
        // printf("listener: packet contains \"%s\"\n", buf);

		// if (buf[1]=='Y') {
		// 	printf("Delivered: %s", buf);
		// }

        // printf("Delivered: %s", buf);

		printf("Enter Y if you want to send an ack back\n: ");
        size_t line_max = 0;
        getline(&y_for_send_ack, &line_max, stdin);

		if (y_for_send_ack[0]=='Y') {

			char seqNum = buf[0];

			switch (seqNum) {
				case '0':
					char *ackNum0 = "0";
					sendto(sockfd, ackNum0, 1024, 0, (struct sockaddr*)&their_addr, sizeof(their_addr));
					break;
				case '1':
					char *ackNum1 = "1";
					sendto(sockfd, ackNum1, 1024, 0, (struct sockaddr*)&their_addr, sizeof(their_addr));
					break;
				case '2':
					char *ackNum2 = "2";
					sendto(sockfd, ackNum2, 1024, 0, (struct sockaddr*)&their_addr, sizeof(their_addr));
					break;
				// case '3':
				// 	char *ackNum = "3";
				// 	sendto(sockfd, ackNum, 1024, 0, (struct sockaddr*)&their_addr, sizeof(their_addr));
				// 	break;
				// case '4':
				// 	char *ackNum = "4";
				// 	sendto(sockfd, ackNum, 1024, 0, (struct sockaddr*)&their_addr, sizeof(their_addr));
				// 	break;
				// case '5':
				// 	char *ackNum = "5";
				// 	sendto(sockfd, ackNum, 1024, 0, (struct sockaddr*)&their_addr, sizeof(their_addr));
				// 	break;
				// case '6':
				// 	char *ackNum = "6";
				// 	sendto(sockfd, ackNum, 1024, 0, (struct sockaddr*)&their_addr, sizeof(their_addr));
				// 	break;
				// case '7':
				// 	char *ackNum = "7";
				// 	sendto(sockfd, ackNum, 1024, 0, (struct sockaddr*)&their_addr, sizeof(their_addr));
				// 	break;
				default:
					printf("shouldnt be here \n");
					break;
			}


			printf("Delivered: %s", buf);
			// char* value = "test";
			// sendto(sockfd, value, 1024, 0, (struct sockaddr*)&their_addr, sizeof(their_addr));
		}

		// char* value = "test";

		// sendto(sockfd, value, 1024, 0, (struct sockaddr*)&their_addr, sizeof(their_addr));

		// recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr*)&client_addr, &addr_size);
		// sendto(sockfd, value, 1024, 0, (struct sockaddr*)&client_addr, sizeof(client_addr));



        
    }

	close(sockfd);

	return 0;
}