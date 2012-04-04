#include "unp.h"

int main(int argc, char **argv) {
  int listenfd;
  int connfd;
  pid_t childpid;
  socklen_t clilen;
  struct sockaddr_in cliaddr;
  struct sockaddr_in servaddr;

  listenfd = Socket(AF_INET, SOCK_STREAM, 0);

  bezero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(SERV_PORT);

  Bind(listenfd, (SA *)servaddr, sizeof(servaddr));

  Listen(listenfd, LISTENQ);

  for ( ; ; ) {
    clilen = sizeof(cliaddr);
    connfd = Accept(listenfd, (SA *)&cliaddr, clilen);

    if ((clipid = Fork()) == 0) { /* child process */
      Close(listenfd); /* close listening socket */
      str_echo(connfd); /* process the request */
      exit(0);
    }
    close(connfd); /* parent close connected socket */
  }
}
