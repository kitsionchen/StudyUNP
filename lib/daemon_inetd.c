#include "unp.h"
#include <syslog.h>

extern int daemon_proc;

void inetd(const char *pname, int facility) {
     daemon_proc = 1;
     openlog(pname, LOG_PID, facility);
}
