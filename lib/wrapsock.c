/*
 * Socket wrapper functoins.
 */
#include "unp.h"

/* include listen */
void Listen(int fd, int backlog) {
  char *ptr;
  /* can override 2nd argument with environment variable */
  if ((ptr = getenv("LISTENQ")) != NULL)
    backlog = atoi(ptr);

  if (listen(fd, backlog) < 0)
    err_sys("listen error");
}
