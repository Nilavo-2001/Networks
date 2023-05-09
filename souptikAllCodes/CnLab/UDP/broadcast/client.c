#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#define BUF_SIZE 256
#define BROADCAST_PORT 8888

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main()
{   char message [10];
    printf("Client started , Enter client number\n");
    scanf("%s", message);
    int sockfd;
    struct sockaddr_in broadcast_addr;
    char buffer[BUF_SIZE];
    int n, broadcastEnable = 1;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));

    bzero(&broadcast_addr, sizeof(broadcast_addr));
    broadcast_addr.sin_family = AF_INET;
    broadcast_addr.sin_port = htons(BROADCAST_PORT);
    broadcast_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);

    sprintf(buffer, message);

    n = sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&broadcast_addr, sizeof(broadcast_addr));
    if (n < 0)
        error("ERROR in sendto");

    printf("Broadcast message sent\n");

    close(sockfd);

    return 0;
}
