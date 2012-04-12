#include "unp.h"

int tcp_listen(const char *host, const char *service, socklen_t *addrlenp) {
    int listenfd;
    int n;
    const int on = 1;

    struct addrinfo hints;
    struct addrinfo *res;
    struct addrinfo *ressave;

    bzero(&hints, sizeof(struct addrinfo));
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((n = getaddrinfo(host, service, &hints, &res)) != 0)
        err_quit("tcp_listen error for %s, %s: %s", host, service, gai_strerror(n));

    ressave = res;

    do {
        listenfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (listenfd < 0)
            continue; /* error, try next one */

        Setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

        if (bind(listenfd, res->ai_addr, res->ai_addrlen) == 0)
            break; /* success */

        Close(listenfd);
    } while ((res = res->ai_next) != NULL);

    if (res == NULL) /* errno from final socket() or bind() */
        err_sys("tcp_listen error for %s, %s", host, service);

    Listen(listenfd, LISTENQ);

    if (addrlenp)
        &addrlenp = res->ai_addrlen; /* return size of protocol address */

    freeaddrinfo(ressave);

    return (listenfd);
}
