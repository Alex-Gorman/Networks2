#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8888

struct myStruct {
	int printed;
	char message[100];
    int delivered;
    int init;
    int index;
    int acked;
};

int main(int argc, char *argv[]) {
    int sockfd, n;
    socklen_t len;
    char buffer[1024];
    struct sockaddr_in servaddr, cliaddr;

    // create a UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // set up the server address and port
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // bind the socket to the server address and port
    bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr));

    /* message queue */
	struct myStruct myArray[200];

    /* message queue index */
    // int msgQueueIndexToAppend = 0;

    int rWindow = atoi(argv[1]);

    while(1) {
        len = sizeof(cliaddr);

        // receive a message from the client
        n = recvfrom(sockfd, (char *)buffer, sizeof(buffer), MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);
        buffer[n] = '\0';

        // print the message received from the client
        // printf("Message received from client: %s\n", buffer);
        // printf("Message received from client: %s", buffer);

        /* check if should send ack */
	    char *y_for_send_ack = NULL;

        printf("Delivered: %s", buffer);

        // int rWindowMin = 0;
        // for (int i = 0; i < 200; i++) {
        //     if (myArray[i].init == 1);
        //     else break;
        //     if (myArray[i].acked == 0) rWindowMin++;
        // }

        // int rWindowMax = 0;
        // for (int i = 0; i < 200; i++) {
        //     if (myArray[i].init == 1);
        //     else break;
        //     if (myArray[i].acked == 0) rWindowMax++;
        // }



        printf("Enter Y if you want to send an ack back: \n ");
        size_t line_max = 0;
        getline(&y_for_send_ack, &line_max, stdin);

		// printf("Delivered: %s\n", buffer);

        // for (int i = 0; i < 200; i++) {
        //     if (myArray[i].init);
        //     else break;
        //     if (myArray[i].printed)
        // }



        struct myStruct myVar;

        strcpy(myVar.message, buffer);

        myVar.init = 1;

        myVar.delivered = 0;

        int num = buffer[0] - '0';
        myVar.index = num;
        myVar.printed = 0;

        myVar.acked = 0;

        // myArray[msgQueueIndexToAppend] = myVar;
	    // msgQueueIndexToAppend++;

        if (y_for_send_ack[0]=='Y') {

            myVar.delivered = 1;
            myVar.printed = 1;
            myVar.acked = 1;


            for (int i = myVar.index; i < 200; i = i + 8) {
                if (myArray[i].init == 1) continue;
                else {
                    myArray[i] = myVar;
                    break;
                }
            }

            int p = 200;

            for (int i = 0; i < 200; i++) {
                if ((myArray[i].init) == 1);
                else break;
                if (myArray[i].printed == 0) p = i;
            }

            for (int i = p; i < 200; i++) {
                if (myArray[i].init == 1);
                else break;
                if (myArray[i].acked == 0) break;
                myArray[i].printed = 1;
                printf("Delivered 2: %s", myArray[i].message);
            }

            // printf("buffer: %s", buffer);

            // myVar.delivered = 1;

			char seqNum = buffer[0];

			switch (seqNum) {
				case '0':
					// char *ackNum0 = "0";
					sendto(sockfd, (const char *)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
					break;
				case '1':
					// char *ackNum1 = "1";
					sendto(sockfd, (const char *)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
					break;
				case '2':
					// char *ackNum2 = "2";
					sendto(sockfd, (const char *)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
					break;
				case '3':
					// char *ackNum = "3";
					sendto(sockfd, (const char *)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
					break;
				case '4':
					// char *ackNum = "4";
					sendto(sockfd, (const char *)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
					break;
				case '5':
					// char *ackNum = "5";
					sendto(sockfd, (const char *)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
					break;
				case '6':
					// char *ackNum = "6";
					sendto(sockfd, (const char *)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
					break;
				case '7':
					// char *ackNum = "7";
					sendto(sockfd, (const char *)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
					break;
				default:
					printf("shouldnt be here \n");
					break;
			}


			// printf("Delivered: %s", buffer);
			// char* value = "test";
			// sendto(sockfd, value, 1024, 0, (struct sockaddr*)&their_addr, sizeof(their_addr));
		} else {
            for (int i = myVar.index; i < 200; i = i + 8) {
                if (myArray[i].init == 1) continue;
                else {
                    myArray[i] = myVar;
                    break;
                }
            }
        }

        // myArray[msgQueueIndexToAppend] = myVar;
	    // msgQueueIndexToAppend++;

        // for (int i = myVar.index; i < 200; i = i + 8) {
        //     if (myArray[i].init == 1) continue;
        //     else {
        //         myArray[i] = myVar;
        //         break;
        //     }
        // }

        // for (int i = 0; i < 200; i++) {
        //     if (myArray[i].init);
        //     else break;
        //     if 
        // }





        // char buffer2[] = "buffer 2\n";
        // char buffer3[] = "buffer 3\n";

        // // send a reply to the client
        // sendto(sockfd, (const char *)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
        // sendto(sockfd, (const char *)buffer2, strlen(buffer2), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
        // sendto(sockfd, (const char *)buffer3, strlen(buffer3), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
    }

    // close the socket
    close(sockfd);

    return 0;
}
