#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "cmd.h"
#include "ipc.h"

void mon_cmd(int connfd, char *msg, Monitor *mons);
void tag_cmd(int connfd, char *msg, Monitor *mons);
void unknown_cmd(int connfd, char *cmd);

void handle_connection(int connfd, Monitor *mons) {
  char *msg = receive_message(connfd);

  char *cmd = strtok(msg, " ");
  if (!cmd) {
    close(connfd);
    return;
  }

  if (!strcmp(cmd, "mons"))
    mon_cmd(connfd, msg, mons);
  else if (!strcmp(cmd, "tags"))
    tag_cmd(connfd, msg, mons);
  else
    unknown_cmd(connfd, cmd);

  close(connfd);
}



void mon_cmd(int connfd, char *msg, Monitor *mons) {
  (void) msg;
  (void) mons;
  send_message(connfd, "mons command called");
}
void tag_cmd(int connfd, char *msg, Monitor *mons) {
  (void) msg;
  (void) mons;
  send_message(connfd, "tags command called");
}

void unknown_cmd(int connfd, char *cmd) {
  char buf[1024];
  snprintf(buf, 1024, "unknown command called: %s", cmd);
  send_message(connfd, buf);
}
