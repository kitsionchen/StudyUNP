#include "unp.h"

int main(int argc, char **argv) {
    struct hostent *hp;
    struct servent *sp;
    struct in_addr inetaddr;
    struct in_addr *inetaddrp[2];
    struct in_addr **pptr;
    int sockfd;
    int n;
    struct sockaddr_in servaddr;
    char recvline[MAXLINE+1];

    if (argc != 3) {
        err_quit("usage: daytimetcpcli1 <hostname> <service>");
    }


    if ((hp = gethostbyname(argv[1])) == NULL) {
        if (inet_aton(argv[1], &inetaddr) == 0) {
            err_quit("hostname error for %s: %s", argv[1], hstrerror(h_errno));
        } else {
            inetaddrp[0] = &inetaddr;
            inetaddrp[1] = NULL;
            pptr = inetaddrp;
        }
    } else {
        pptr = (struct in_addr **)hp->h_addr_list;
    }

    if ((sp = getservbyname(argv[2], "tcp")) == NULL)
        err_quit("getservbyname error for %s", argv[2]);

    for ( ; *pptr != NULL; pptr++) {
        sockfd = Socket(AF_INET, SOCK_STREAM, 0);
        bzero(&servaddr, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = sp->s_port;
        memcpy(&servaddr.sin_addr, *pptr, sizeof(struct in_addr));
        printf("trying %s\n", Sock_ntop((SA *)&servaddr, sizeof(servaddr)));

        if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) == 0)
            break;

        err_ret("connect error");
        close(sockfd);
    }

    if (*pptr == NULL)
        err_quit("unable to connect");

    while ((n = Read(sockfd, recvline, MAXLINE)) > 0) {
        recvline[n] = 0;
        Fputs(recvline, stdout);
    }

    exit(0);
}
