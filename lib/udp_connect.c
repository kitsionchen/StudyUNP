#include "unp.h"

int udp_connect(const char *host, const char *service) {
    int sockfd;
    int n;
    struct addrinfo hints;
    struct addrinfo *res;
    struct addrinfo *ressave;

    bzero(&hints, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if ((n = getaddrinfo(host, service, &hints, &res)) != 0)
        err_quit("udp_connect error for %s, %s: %s", host, service, gai_strerror(n));

    ressave = res;

    do {
        sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (sockfd < 0)
            continue;

        if (connect(sockfd, res->ai_addr, res->ai_addrlen) == 0)
            break;

        Close(sockfd);
    } while ((res = res->ai_next) != NULL);

    if (res == NULL)
        err_sys("udp_connect error for %s, %s", host, service);

    freeaddrinfo(ressave);

    return (sockfd);
}
