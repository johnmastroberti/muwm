#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>

#include "ipc.h"
#include "util.h"

void usage() {
  fputs("Usage: muctl command...\n", stderr);
}


int main(int argc, char **argv) {
  if (argc == 1) {
    usage();
    exit(EXIT_FAILURE);
  }

  Display *dpy;
  int screen;
  if (!(dpy = XOpenDisplay(NULL)))
    die("muctl: cannot open display");
  screen = DefaultScreen(dpy);

  int sockfd = connect_to_socket(display_number(), screen);

  int message_len = 0;
  for (int i=1; i<argc; i++)
    message_len += strlen(argv[i]) + 1;

  char *message = malloc(message_len);
  int pos = 0;
  for (int i=1; i<argc; i++) {
    sprintf(message + pos, "%s ", argv[i]);
    pos += strlen(argv[i]) + 1;
  }
  message[pos] = '\0';

  if (send_message(sockfd, message) < 0)
    die("muctl: message failed to send:");

  char *response = receive_message(sockfd);
  puts(response);

  free(message);
  free(response);
  return EXIT_SUCCESS;
}


