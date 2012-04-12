#include "unp.h"

int udp_client(const char *host, const char *service, SA **saptr, socklen_t *lenp) {

    int sockfd;
    int n;
    struct addrinfo hints;
    struct addrinfo *res;
    struct addrinfo *ressave;

    bzero(&hints, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    if ((n = getaddrinfo(host, service, &hints, &res)) != 0)
        err_quit("udp_client error for %s, %s: %s", host, service, gai_strerror(n));

    ressave = res;

    do {
        sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (sockfd >= 0)
            break; /* success */
    } while ((res = res->ai_next) != NULL);

    if (res == NULL) /* errno set from final socket */
        err_sys("udp_client error for %s, %s", host, service);

    *saptr = Malloc(res->ai_addrlen);
    memcpy(*saptr, res->ai_addr, res->ai_addrlen);
    *lenp = res->ai_addrlen;

    freeaddrinfo(ressave);
    return (sockfd);
}
