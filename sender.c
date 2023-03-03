/*
** talker.c -- a datagram "client" demo
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
#include "list.h"
#include <pthread.h>
#include <poll.h>
#include <time.h>

#define SERVERPORT "4950"	// the port users will be connecting to
#define MAXBUFLEN 100

LIST *messages;

struct myStruct {
	time_t timeSent;
	char message[100];
};

int main(int argc, char *argv[])
{
	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	int numbytes;
	socklen_t addr_len;

	if (argc != 5) {
		fprintf(stderr,"usage: sender hostname port window-size timeout(s)\n");
		exit(1);
	}

    int windowSize = atoi(argv[3]);

    // int timeoutValue = atoi(argv[4]);

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET6; // set to AF_INET to use IPv4
	hints.ai_socktype = SOCK_DGRAM;

    if ((rv = getaddrinfo(NULL, argv[2], &hints, &servinfo)) != 0) {
	// if ((rv = getaddrinfo(argv[1], SERVERPORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and make a socket
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("talker: socket");
			continue;
		}
		break;
	}

	if (p == NULL) {
		fprintf(stderr, "talker: failed to create socket\n");
		return 2;
	}

	/* Sequence number count */
    int msgSeqCount = 0;

	/* Message to be sent */
    char *msg = NULL;

	/* Queue of messages */
	// LIST messages[20];
	// messages = ListCreate();

	/* Poll struct */
	struct pollfd mypoll;

	/* Init mypoll */
	memset(&mypoll, 0, sizeof(mypoll));
	mypoll.fd = sockfd;
	mypoll.events = POLLIN;

	/* */
	char buf[MAXBUFLEN];

	addr_len = sizeof p;


	/* current number of messages */
	int curNumOfMsgs = 0;

	/* message queue */
	struct myStruct myArray[200];

	/* message queue index */
	int msgQueueIndexToAppend = 0;
	int msgQueueIndexToRemove = 0;


    while (1) {

		/* Poll() to check for acks */
		struct pollfd udp_fd;
		udp_fd.fd = sockfd;
		udp_fd.events = POLLIN;

		int result = poll(&udp_fd, 1, 500);

		if (result == -1) {
			perror("poll");
		} else if (result == 0) {
			// printf("Timeout occured");
		} else {
			if (udp_fd.events & POLLIN) {
				printf("got here\n");
				if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1, 0, (struct sockaddr*)&p->ai_addr, &addr_len)) == -1) {
            		perror("recvfrom");
            		exit(1);
	    		}
				continue;
			}
		}

		/* potentially send a message */
        printf("Enter message: ");
        size_t line_max = 0;
        getline(&msg, &line_max, stdin);

        if (strlen(msg) <= 1) {
             continue;
        }

        char temp[10];
        sprintf(temp, "%d", msgSeqCount);
        strcat(temp, msg);
        strcpy(msg, temp);

		if (msgQueueIndexToAppend > msgQueueIndexToRemove) {

				time_t new_time;
				time(&new_time);

				for (int i = msgQueueIndexToRemove; i < msgQueueIndexToAppend+1; i++) {
					if (&myArray[i] == NULL) {
						continue;
					}
					else if ((new_time - myArray[i].timeSent) > windowSize) {
						myArray[i].timeSent = new_time;
						if ((numbytes = sendto(sockfd, myArray[i].message, strlen(msg)+1, 0, p->ai_addr, p->ai_addrlen)) == -1) {
							perror("1. talker: sendto");
							exit(1);
						}
						continue;
					}
				}
		}
		else if (curNumOfMsgs <= windowSize) {
			struct myStruct myVar;

			time_t cur_time;
			time(&cur_time);

			strcpy(myVar.message, msg);

			if ((numbytes = sendto(sockfd, myVar.message, strlen(msg)+1, 0, p->ai_addr, p->ai_addrlen)) == -1) {
				perror("2. talker: sendto");
				exit(1);
			}

			curNumOfMsgs++;
			printf("3. talker: sent %d bytes to %s\n", numbytes, argv[1]);

			msgSeqCount += 1;
			if (msgSeqCount == 8) msgSeqCount = 0;

		}
		else {
			strcpy(myArray[msgQueueIndexToAppend].message, msg);
			msgQueueIndexToAppend++;
		}

		// /* Poll() to check for acks */
		// struct pollfd udp_fd;
		// udp_fd.fd = sockfd;
		// udp_fd.events = POLLIN;

		// int result = poll(&udp_fd, 1, 500);

		// if (result == -1) {
		// 	perror("poll");
		// } else if (result == 0) {
		// 	printf("Timeout occured");
		// } else {
		// 	if (udp_fd.events & POLLIN) {
		// 		printf("got here\n");
		// 		if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1, 0, (struct sockaddr*)&p->ai_addr, &addr_len)) == -1) {
        //     		perror("recvfrom");
        //     		exit(1);
	    // 		}
		// 	}
		// }



		// if (poll(&mypoll, 1, 100) >= 1) {
		// 	if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1, 0, (struct sockaddr*)&p->ai_addr, &addr_len)) == -1) {
        //     	perror("recvfrom");
        //     	exit(1);
	    // 	}
		// 	printf("poll() called\n");
		// 	printf("%s\n", buf);

		// 	// if
		// }

		// 	} 
		// 	else {
		// 		struct myStruct myVar;
		// 		time(&myVar.timeSent);
		// 		strcpy(myVar.message, msg);

		// 		if ((numbytes = sendto(sockfd, myVar.message, strlen(msg)+1, 0, p->ai_addr, p->ai_addrlen)) == -1) {
		// 			perror("talker: sendto");
		// 			exit(1);
		// 		}

		// 		printf("talker: sent %d bytes to %s\n", numbytes, argv[1]);
		// 	}

		// } else {
		// 	strcpy(myArray[msgQueueIndexToAppend].message, msg);
		// 	// printf("%s", queue[0]);

		// 	msgQueueIndexToAppend++;
		// }

		// msgSeqCount += 1;
		// if (msgSeqCount == 8) msgSeqCount = 0;

		// curNumOfMsgs++;

        

		// if (poll(&mypoll, 1, 100) == 1) {
		// 	if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1, 0, (struct sockaddr*)&p->ai_addr, &addr_len)) == -1) {
        //     	perror("recvfrom");
        //     	exit(1);
	    // 	}
		// 	printf("%s", buf);
		// }
    }

	// close(sockfd);

	return 0;
}
