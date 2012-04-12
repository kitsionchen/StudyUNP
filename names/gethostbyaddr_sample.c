#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

/*
 * reports the error and exits back to the shell.
 */
static void displayError(const char *msg) {
    if (errno != 0) {
        fputs(strerror(errno), stderr);
        fputs(": ", stderr);
    }
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char **argv) {
    FILE *logfp;
    char *serv_addr = NULL;
    char *serv_port = "9099";
    struct sockaddr_in serv_addr_in;
    struct sockaddr_in cli_addr_in;
    int listenfd;
    int connfd;
    socklen_t len;
    struct hostent *hp;
    time_t td;
    char tdbuf[128];
    int n;

    if ((logfp = fopen("gethostbyaddr.log", "w")) < 0) {
        displayError("fopen(3)");
    }

    if (argc >= 2) {
        serv_addr = argv[1];
    } else {
        serv_addr = "127.0.0.1";
    }

    if (argc >= 3) {
        serv_port = argv[2];
    }

    /* create a socket */
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    /* create a server socket address */
    memset(&serv_addr_in, 0, sizeof(serv_addr_in));
    serv_addr_in.sin_family = AF_INET;
    serv_addr_in.sin_port = htons(atoi(serv_port));

    if (strcmp(serv_addr, "*") != 0) {

        serv_addr_in.sin_addr.s_addr = inet_addr(serv_addr);

        if (serv_addr_in.sin_addr.s_addr == INADDR_NONE)
            displayError("bad address.");

    } else {
        serv_addr_in.sin_addr.s_addr = INADDR_ANY;
    }

    /* bind the server address */
    if (bind(listenfd, (struct sockaddr *)&serv_addr_in, sizeof(serv_addr_in)) < 0) 
        displayError("bind(2)");

    /* listen socket */
    if (listen(listenfd, 10) < 0)
        displayError("listen(2)");

    /* start the server loop */
    for ( ; ; ) {
        len = sizeof(cli_addr_in);
        if ((connfd = accept(listenfd, (struct sockaddr *)&cli_addr_in, &len)) < 0) 
            displayError("accept(2)");

        /* Log the address of the client */
        fprintf(logfp, "Client %s: ", inet_ntoa(cli_addr_in.sin_addr));
        if ((hp = gethostbyaddr((char *)&cli_addr_in.sin_addr, sizeof(cli_addr_in.sin_addr), cli_addr_in.sin_family)) == NULL)
            fprintf(logfp, "Error: %s\n", hstrerror(h_errno));
        else
            fprintf(logfp, "%s\n", hp->h_name);

        fflush(logfp);

        time(&td);
        n = (int)strftime(tdbuf, sizeof(tdbuf), "%A %b %d %H:%M:%S %Y\n", localtime(&td));

        if (write(connfd, tdbuf, n) < 0)
            displayError("write(2)");

        close(connfd);
    }

    return 0;
}


