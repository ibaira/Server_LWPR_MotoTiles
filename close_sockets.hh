#include <arpa/inet.h>
#include "close_sockets.hh"
#include <cstdio>
#include <cstdlib>
#include <errno.h>
#include <ios>
#include <iostream>
#include <math.h>
#include <netdb.h>
#include <netinet/in.h>
#include <Poco/Exception.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

int getSO_ERROR(int fd) {
  int err = 1;
  socklen_t len = sizeof err;
  if (-1 == getsockopt(fd, SOL_SOCKET, SO_ERROR, (char *)&err, &len))
    perror("getSO_ERROR");
    // FatalError("getSO_ERROR");
  if (err)
    errno = err;              // set errno to the socket SO_ERROR
  return err;
}

void closeSocket(int fd) {      // *not* the Windows closesocket()
  if (fd >= 0) {
    getSO_ERROR(fd); // first clear any errors, which can cause close to fail
    if (shutdown(fd, SHUT_RDWR) < 0) // secondly, terminate the 'reliable' delivery
      if (errno != ENOTCONN && errno != EINVAL) // SGI causes EINVAL
        perror("shutdown");
    if (close(fd) < 0) // finally call close()
      perror("close");
  }
}
