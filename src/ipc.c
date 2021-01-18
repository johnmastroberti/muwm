#include <sys/socket.h>
#include <sys/un.h>
#include <poll.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "ipc.h"
#include "util.h"

#define SOCKET_FMT "/tmp/muwm%s_%i_%i-socket"


// Perform common setup steps for create_socket and connect_to_socket
// The socked fd is returned and other parameters are writeen to sock
int socket_setup(int display, int screen, struct sockaddr_un *sock) {
  int sockfd;
  sock->sun_family = AF_UNIX;

  if ((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
    die("Failed to create socket");
  }

  int namelen = 21;
  char host[namelen];
  namelen = gethostname(host, namelen);
  host[namelen-1] = '\0';

  snprintf(sock->sun_path, sizeof(sock->sun_path),
      SOCKET_FMT, host, display, screen);
  return sockfd;
}


int create_socket(int display, int screen) {
  struct sockaddr_un sock_address;
  int sockfd = socket_setup(display, screen, &sock_address);

  unlink(sock_address.sun_path);
  if (bind(sockfd, (struct sockaddr *) &sock_address, sizeof(sock_address)) == -1)
    die("muwm ipc: could not bind socket:");

  if (listen(sockfd, SOMAXCONN) == -1)
    die("muwm ipc: could not listen on socket:");

  return sockfd;
}

int connect_to_socket(int display, int screen) {
  struct sockaddr_un sock_address;
  int sockfd = socket_setup(display, screen, &sock_address);

  if (connect(sockfd, (struct sockaddr *) &sock_address,
        sizeof(sock_address)) == -1)
    die("muwm ipc:");

  return sockfd;
}


int send_message(int sockfd, char *message) {
  return send(sockfd, message, strlen(message), 0);
}


char* receive_message(int sockfd) {
  int len = 32, pos = 0, nb;
  char *response = malloc(len);

  struct pollfd fds[] = {
    {sockfd, POLLIN, 0},
    {STDOUT_FILENO, POLLHUP, 0}
  };

  while (poll(fds, 2, -1) > 0) {
    if (fds[0].revents & POLLIN) {
      if ((nb = recv(sockfd, response+pos, len-pos, 0)) > 0) {
        pos += nb;
        len *= 2;
        response = realloc(response, len);
      }
    } else {
      break;
    }
  }
  response[pos] = '\0';

  return response;
}

