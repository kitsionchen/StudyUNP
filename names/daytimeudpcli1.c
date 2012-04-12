#include "unp.h"

int main(int argc, char **argv) {
    int sockfd;
    int n;
    char recvline[MAXLINE+1];
    socklen_t salen;
    struct sockaddr *sa;

    if (argc != 3)
        err_quit("usage: daytimeudpcli <hostname/IPaddress> <service/port#>");

    sockfd = Udp_client(argv[1], argv[2], (void **)&sa, &salen);

    printf("sending to %s\n", Sock_ntop_host(sa, salen));

    Sendto(sockfd, "", 1, 0, sa, salen); /* send 1-byte datagram */

    n = Recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL);
    recvline[n] = '\0';
    Fputs(recvline, stdout);

    exit(0);
}
