#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "cmd.h"
#include "ipc.h"


void title_cmd(int connfd, Monitor *mons) {
  char *sub = strtok(NULL, " ");
  Monitor *m;
  if (!find_monitor(mons, sub, &m)) {
    send_message(connfd, "ERROR: bad monitor number");
    return;
  }
  if (m->sel)
    send_message(connfd, m->sel->name);
  else
    send_message(connfd, "");
}


void subscribe_reset_cmd(int connfd, Monitor *mons) {
  (void) mons;
  reset_subscribers();
  send_message(connfd, "subscribers reset");
}
void subscribe_add_cmd(int connfd, Monitor *mons) {
  (void) mons;
  int pid;
  if (!parse_int(strtok(NULL, " "), &pid)) {
    send_message(connfd, "usage: muctl subscribe (pid|reset)");
    return;
  }
  if (!add_subscriber(pid)) {
    send_message(connfd, strerror(errno));
  } else {
    char buf[1024];
    sprintf(buf, "pid %d added to subscribers", pid);
    send_message(connfd, buf);
  }
}
void subscribe_cmd(int connfd, Monitor *mons) {
  Command commands[] = {
    {"reset", subscribe_reset_cmd},
    {"", subscribe_add_cmd},
  };
  parse_word(connfd, strtok(NULL, " "), mons, commands, LENGTH(commands));
}

void kill_cmd(int connfd, Monitor *mons) {
  (void) mons;
  killclient(NULL);
  send_message(connfd, "");
}

void togglefloat_cmd(int connfd, Monitor *mons) {
  (void) mons;
  togglefloating(NULL);
  send_message(connfd, "");
}

void focus_prev_cmd(int connfd, Monitor *mons);
void focus_next_cmd(int connfd, Monitor *mons);
void focus_monprev_cmd(int connfd, Monitor *mons);
void focus_monnext_cmd(int connfd, Monitor *mons);
void focus_unknown_cmd(int connfd, Monitor *mons);

void focus_cmd(int connfd, Monitor *mons) {
  Command commands[] = {
    {"prev", focus_prev_cmd},
    {"next", focus_next_cmd},
    {"monprev", focus_monprev_cmd},
    {"monnext", focus_monnext_cmd},
    {"", focus_unknown_cmd}
  };

  parse_word(connfd, strtok(NULL, " "), mons, commands, LENGTH(commands));
}

void focus_prev_cmd(int connfd, Monitor *mons) {
  (void) mons;
  Arg a = {.i = -1};
  focusstack(&a);
  send_message(connfd, "");
}
void focus_next_cmd(int connfd, Monitor *mons) {
  (void) mons;
  Arg a = {.i = +1};
  focusstack(&a);
  send_message(connfd, "");
}
void focus_monprev_cmd(int connfd, Monitor *mons) {
  (void) mons;
  Arg a = {.i = -1};
  focusmon(&a);
  send_message(connfd, "");
}
void focus_monnext_cmd(int connfd, Monitor *mons) {
  (void) mons;
  Arg a = {.i = +1};
  focusstack(&a);
  send_message(connfd, "");
}
void focus_unknown_cmd(int connfd, Monitor *mons) {
  (void) mons;
  send_message(connfd, "usage: muctl focus (next|prev|monnext|monprev)");
}
