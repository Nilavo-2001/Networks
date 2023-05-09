#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8888
#define BUF_SIZE 256

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main()
{
    printf("Server started\n");

    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    char buffer[BUF_SIZE];
    int n, len;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
        error("ERROR on binding");

    printf("Server waiting for broadcast messages...\n");

    while (1) {
        len = sizeof(cliaddr);
        n = recvfrom(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr *)&cliaddr, &len);
        if (n < 0)
            error("ERROR in recvfrom");

        printf("Received broadcast message from %s:%d\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
        printf("Message: %s\n", buffer);
    }

    close(sockfd);

    return 0;
}
