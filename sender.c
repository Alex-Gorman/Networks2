#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>
#include <time.h>

#define PORT 8888

struct myStruct {
	time_t timeSent;
	char message[100];
    int acked;
    int init;
};

int main(int argc, char *argv[]) {
    int sockfd, n;
    socklen_t len;
    // char buffer[1024];
    char *buffer = NULL;
    struct sockaddr_in servaddr;

    // create a UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // set up the server address and port
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    if (argc != 5) {
        fprintf(stderr,"usage: sender hostname port window-size timeout(s)\n");
		exit(1);
    }

    int windowSize = atoi(argv[3]);

    int timeoutLength = atoi(argv[4]);

    /* Sequence number count */
    int msgSeqCount = 0;

    /* current number of messages */
	int curNumOfMsgs = 0;

    /* message queue */
	struct myStruct myArray[200];

	/* message queue index */
	int msgQueueIndexToAppend = 0;
	int msgQueueIndexToRemove = 0;

    while(1) {
        
        /* Check poll() for acks */
        struct pollfd udp_fd;
		udp_fd.fd = sockfd;
		udp_fd.events = POLLIN;

		int result = poll(&udp_fd, 1, 500);

		if (result == -1) {
			perror("poll");
		} else if (result == 0) {
			// printf("Timeout occured\n");
		} else {
			if (udp_fd.events & POLLIN) {
				// printf("got here\n");
				n = recvfrom(sockfd, (char *)buffer, sizeof(buffer), MSG_WAITALL, (struct sockaddr *)&servaddr, &len);
                buffer[n] = '\0';

                // print the reply received from the server
                printf("Message received from server: %s\n", buffer);

                time_t new_time;
				time(&new_time);

                for (int i = msgQueueIndexToRemove; i <= msgQueueIndexToAppend; i++) {
                    if (&myArray[i] == NULL) {
                        ;
                    }
                    else if ((new_time - myArray[i].timeSent) > windowSize) {
                        // int first_character = buffer[0] - '0';


                        // printf("%d", first_character);
                        // printf("got here 2\n");

                        // printf("Message in array at %d(i) %s\n", i, myArray[i].message);

                        // printf("%d\n", myArray[i].message[0]);

                        if (myArray[i].message[0] == buffer[0] && myArray[i].acked == 0) {
                            // printf("got here \n");

                            myArray[i].acked = 1;

                            // printf("Message in array at %d(i) %s\n", i, myArray[i].message);

                            curNumOfMsgs--;

                            printf("i = %d. Message has been acked. %s", i, myArray[i].message);

                            break;
                        }
                    }
                }
                continue;


			}
		}

        /* Potentially send a message */
        // get input from the user
        printf("Enter a message: ");
        // fgets(buffer, sizeof(buffer), stdin);
        size_t line_max = 0;
        getline(&buffer, &line_max, stdin);

        if (strlen(buffer) <= 1) {
             continue;
        }

        char temp[10];
        sprintf(temp, "%d", msgSeqCount);
        strcat(temp, buffer);
        strcpy(buffer, temp);

        // /* Problem exists here */
        // if (1) {
        //     time_t new_time;
        //     time(&new_time);

        //     for (int i = 0; i <= msgQueueIndexToAppend; i++) {
        //         // if (myArray[i]==NULL) continue;
        //         // printf("%s\n", myArray[i].message);
        //         // printf("%d\n", myArray[i].init);

        //         // printf("newTime: %ld\n", new_time);
        //         // printf("oldTime: %ld\n", myArray[i].timeSent);
        //         // printf("difference: %ld\n", new_time - myArray[i].timeSent);

        //         if (myArray[i].init == 0) continue;
        //         if (myArray[i].acked == 1) continue;
        //         // printf("newTime: %ld\n", new_time);
        //         // printf("oldTime: %ld\n", myArray[i].timeSent);
        //         // printf("difference: %ld\n", new_time - myArray[i].timeSent);
        //         if ((new_time - myArray[i].timeSent)>timeoutLength) {
        //             // printf("newTime: %ld\n", new_time);
        //             // printf("oldTime: %ld\n", myArray[i].timeSent);
        //             // printf("difference: %ld\n", new_time - myArray[i].timeSent);
        //             // printf("got here \n");
        //             if (sendto(sockfd, (const char *)myArray[i].message, strlen(myArray[i].message), MSG_CONFIRM, (const struct sockaddr *)&servaddr, sizeof(servaddr))==-1) {
        //                 perror("2. talker: sendto");
        //                 exit(1);
		// 	        }
        //             // printf("Message sent: %s\n", myArray[i].message);
        //             printf("Message sent Again: %s", myArray[i].message);
        //             ;
        //         }
        //     }
        // }

		// printf()
		printf("Message sent: %s", buffer);



        if (curNumOfMsgs <= windowSize) {
			struct myStruct myVar;

			time_t cur_time;
			time(&cur_time);

			strcpy(myVar.message, buffer);

            myVar.acked = 0;

            myVar.init = 1;

            myVar.timeSent = cur_time;

            myArray[msgQueueIndexToAppend] = myVar;

			if (sendto(sockfd, (const char *)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *)&servaddr, sizeof(servaddr))==-1) {
				perror("2. talker: sendto");
				exit(1);
			}

            // printf("Message sent: %s\n", buffer);
            printf("Message sent: %s", buffer);

			curNumOfMsgs++;
			// printf("3. talker: sent %d bytes to %s\n", numbytes, argv[1]);

			msgSeqCount += 1;
			if (msgSeqCount == 8) msgSeqCount = 0;

            msgQueueIndexToAppend++;

		} else {
            struct myStruct myVar;

			time_t cur_time;
			time(&cur_time);

			strcpy(myVar.message, buffer);

            myVar.timeSent = cur_time;

            myVar.acked = 0;

            myVar.init = 1;

            myArray[msgQueueIndexToAppend] = myVar;

            msgSeqCount += 1;
			if (msgSeqCount == 8) msgSeqCount = 0;

            msgQueueIndexToAppend++;
        }

        // send the message to the server
        // sendto(sockfd, (const char *)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *)&servaddr, sizeof(servaddr));

        // receive a reply from the server
        // n = recvfrom(sockfd, (char *)buffer, sizeof(buffer), MSG_WAITALL, (struct sockaddr *)&servaddr, &len);
        // buffer[n] = '\0';

        // // print the reply received from the server
        // printf("Message received from server: %s\n", buffer);
    }

    // close the socket
    close(sockfd);

    return 0;
}
