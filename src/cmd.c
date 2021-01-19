#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "cmd.h"
#include "ipc.h"

void mon_cmd(int connfd, char *msg, Monitor *mons);
void title_cmd(int connfd, char *msg, Monitor *mons);
void subscribe_cmd(int connfd, char *msg, Monitor *mons);
void set_cmd(int connfd, char *msg, Monitor *mons);
void tag_cmd(int connfd, char *msg, Monitor *mons);
void focus_cmd(int connfd, char *msg, Monitor *mons);
void kill_cmd(int connfd, Monitor *mons);
void togglefloat_cmd(int connfd, Monitor *mons);
void unknown_cmd(int connfd);


void handle_connection(int connfd, Monitor *mons) {
  char *msg = receive_message(connfd);

  char *cmd = strtok(msg, " ");
  if (!cmd) {
    close(connfd);
    return;
  }

  if (!strcmp(cmd, "mons"))
    mon_cmd(connfd, msg, mons);
  else if (!strcmp(cmd, "title"))
    title_cmd(connfd, msg, mons);
  else if (!strcmp(cmd, "tags"))
    tag_cmd(connfd, msg, mons);
  else
    unknown_cmd(connfd);

  close(connfd);
}


char* mons_list(Monitor *mons) {
  Monitor *m;
  char *buf = malloc(1024); // buffer overflow possible
  int pos = 0;
  for (m = mons; m; m = m->next)
    pos += sprintf(buf + pos, "%d ", m->num);

  return buf;
}


void mon_cmd(int connfd, char *msg, Monitor *mons) {
  Arg arg;
  char *sub = strtok(msg, " ");
  if (!strcmp(sub, "prev")) {
    arg.i = -1;
    focusmon(&arg);
    send_message(connfd, "");
  } else if (!strcmp(sub, "next")) {
    arg.i = +1;
    focusmon(&arg);
    send_message(connfd, "");
  } else {
    char *rsp = mons_list(mons);
    send_message(connfd, rsp);
    free(rsp);
  }
}


void title_cmd(int connfd, char *msg, Monitor *mons) {
  char *sub = strtok(msg, " ");
  if (!sub) goto title_err;
  int num = atoi(sub);
  for (Monitor* m = mons; m; m = m->next) {
    if (num == m->num) {
      send_message(connfd, m->sel->name);
      return;
    }
  }
title_err:
  send_message(connfd, "ERROR: bad monitor number");
}


void subscribe_cmd(int connfd, char *msg, Monitor *mons) {
  (void) msg;
  (void) mons;
  send_message(connfd, "WARNING: not implemented"); //TODO
}


void set_mfact_cmd(int connfd, char *msg, Monitor *mons);
void set_borderpx_cmd(int connfd, char *msg);
void set_gappx_cmd(int connfd, char *msg);
void set_reserved_cmd(int connfd, char *msg, Monitor *mons);
void set_layout_cmd(int connfd, char *msg, Monitor *mons);

void set_cmd(int connfd, char *msg, Monitor *mons) {
  char *sub = strtok(msg, " ");
  if (!strcmp(sub, "mfact"))
    set_mfact_cmd(connfd, msg, mons);
  else if (!strcmp(sub, "borderpx"))
    set_borderpx_cmd(connfd, msg);
  else if (!strcmp(sub, "gappx"))
    set_gappx_cmd(connfd, msg);
  else if (!strcmp(sub, "reserved"))
    set_reserved_cmd(connfd, msg, mons);
  else if (!strcmp(sub, "layout"))
    set_layout_cmd(connfd, msg, mons);
  else
    send_message("ERROR: bad sub command");
}



void tag_cmd(int connfd, char *msg, Monitor *mons) {
  (void) msg;
  (void) mons;
  send_message(connfd, "tags command called");
}

void unknown_cmd(int connfd) {
  send_message(connfd, "ERROR: unknown command");
}
