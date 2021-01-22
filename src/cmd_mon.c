#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cmd.h"
#include "ipc.h"


char* mons_list(Monitor *mons) {
  Monitor *m;
  char *buf = malloc(1024); // buffer overflow possible
  int pos = 0;
  for (m = mons; m; m = m->next)
    pos += sprintf(buf + pos, "%d ", m->num);

  return buf;
}


void mon_cmd(int connfd, Monitor *mons) {
  Arg arg;
  char *sub = strtok(NULL, " ");
  int empty = sub ? 0 : 1;
  if (!empty && !strcmp(sub, "prev")) {
    arg.i = -1;
    focusmon(&arg);
    send_message(connfd, "");
  } else if (!empty && !strcmp(sub, "next")) {
    arg.i = +1;
    focusmon(&arg);
    send_message(connfd, "");
  } else {
    char *rsp = mons_list(mons);
    send_message(connfd, rsp);
    free(rsp);
  }
}

