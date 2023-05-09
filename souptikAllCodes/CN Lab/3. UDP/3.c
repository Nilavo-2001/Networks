#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#define BUF_SIZE 256
#define BROADCAST_PORT 8888
#define MULTICAST_ADDR "224.0.0.1"
#define MULTICAST_PORT 9999
void error(char *msg)
{
    perror(msg);
    exit(1);
}
int main(int argc, char *argv[])
{
    printf("Kaninika Datta\n");
    int sockfd_broadcast, sockfd_multicast, n;
    struct sockaddr_in broadcast_addr, multicast_addr;
    struct in_addr localInterface;
    struct hostent *group;
    char buffer[BUF_SIZE];
    sockfd_broadcast = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd_broadcast < 0)
        error("ERROR opening broadcast socket");
    int broadcastEnable = 1;
    setsockopt(sockfd_broadcast, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));
    bzero(&broadcast_addr, sizeof(broadcast_addr));
    broadcast_addr.sin_family = AF_INET;
    broadcast_addr.sin_port = htons(BROADCAST_PORT);
    if (inet_aton("255.255.255.255", &broadcast_addr.sin_addr) == 0)
        error("ERROR setting broadcast address");

    sockfd_multicast = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd_multicast < 0)
        error("ERROR opening multicast socket");

    localInterface.s_addr = inet_addr("127.0.0.1");
    setsockopt(sockfd_multicast, IPPROTO_IP, IP_MULTICAST_IF, (char *)&localInterface, sizeof(localInterface));
    bzero(&multicast_addr, sizeof(multicast_addr));
    multicast_addr.sin_family = AF_INET;
    multicast_addr.sin_port = htons(MULTICAST_PORT);

    group = gethostbyname(MULTICAST_ADDR);

    if (group == NULL)
        error("ERROR getting multicast group");

    memcpy(&multicast_addr.sin_addr.s_addr, group->h_addr, group->h_length);
    sprintf(buffer, "Broadcast message from client");
    n = sendto(sockfd_broadcast, buffer, strlen(buffer), 0, (struct sockaddr *)&broadcast_addr, sizeof(broadcast_addr));

    if (n < 0)
        error("ERROR sending broadcast message");
    printf("Broadcast message sent\n");
    printf(buffer, "Multicast message from client");

    n = sendto(sockfd_multicast, buffer, strlen(buffer), 0, (struct sockaddr *)&multicast_addr, sizeof(multicast_addr));

    if (n < 0)
        error("ERROR sending multicast message");

    printf("Multicast message sent\n");
    close(sockfd_broadcast);
    close(sockfd_multicast);
    return 0;
}