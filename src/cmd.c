#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "cmd.h"
#include "ipc.h"

void mon_cmd(int connfd, Monitor *mons);
void title_cmd(int connfd, Monitor *mons);
void subscribe_cmd(int connfd, Monitor *mons);
void set_cmd(int connfd, Monitor *mons);
void tag_cmd(int connfd, Monitor *mons);
void focus_cmd(int connfd, Monitor *mons);
void kill_cmd(int connfd, Monitor *mons);
void togglefloat_cmd(int connfd, Monitor *mons);
void unknown_cmd(int connfd);

int find_monitor(Monitor *mons, const char* mnum, Monitor **m) {
  // Attempts to match mnum against the given list of monitors
  // Upon success, 1 is returned and the matching monitor is
  // is returned in m. Zero is returned if m is NULL,
  // mnum is not a good integer, or mnum does not match any
  // of the monitors in the list
  if (!m || !mnum || *mnum == '\0' ) return 0;
  char *end;
  long num = strtol(mnum, &end, 10);
  if ( *end != '\0') return 0;
  for (*m = mons; *m; *m = (*m)->next) {
    if (num == (*m)->num) return 1;
  }
  *m = NULL;
  return 0;
}


void handle_connection(int connfd, Monitor *mons) {
  char *msg = receive_message(connfd);

  char *cmd = strtok(msg, " ");
  if (!cmd) {
    close(connfd);
    return;
  }

  if (!strcmp(cmd, "mons"))
    mon_cmd(connfd, mons);
  else if (!strcmp(cmd, "title"))
    title_cmd(connfd, mons);
  else if (!strcmp(cmd, "set"))
    set_cmd(connfd, mons);
  else if (!strcmp(cmd, "tags"))
    tag_cmd(connfd, mons);
  else
    unknown_cmd(connfd);

  close(connfd);
  free(msg);
}


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


void subscribe_cmd(int connfd, Monitor *mons) {
  (void) mons;
  send_message(connfd, "WARNING: not implemented"); //TODO
}


void set_mfact_cmd(int connfd, Monitor *mons);
void set_borderpx_cmd(int connfd, Monitor *mons);
void set_gappx_cmd(int connfd, Monitor *mons);
void set_reserved_cmd(int connfd, Monitor *mons);
void set_layout_cmd(int connfd, Monitor *mons);

void set_cmd(int connfd, Monitor *mons) {
  char *sub = strtok(NULL, " ");
  int empty = sub ? 0 : 1;
  if (!empty && !strcmp(sub, "mfact"))
    set_mfact_cmd(connfd, mons);
  else if (!empty && !strcmp(sub, "borderpx"))
    set_borderpx_cmd(connfd, mons);
  else if (!empty && !strcmp(sub, "gappx"))
    set_gappx_cmd(connfd, mons);
  else if (!empty && !strcmp(sub, "reserved"))
    set_reserved_cmd(connfd, mons);
  else if (!empty && !strcmp(sub, "layout"))
    set_layout_cmd(connfd, mons);
  else
    send_message(connfd, "ERROR: bad sub command");
}


void set_mfact_cmd(int connfd, Monitor *mons) {
  char *mon = strtok(NULL, " ");
  Monitor *m;
  if (!find_monitor(mons, mon, &m))
    goto set_mfact_err;

  float mf = strtof(strtok(NULL, " "), NULL);
  if (mf <= 0.0f || mf >= 1.0f)
    goto set_mfact_err;
  else {
    Arg a = { .f = mf + 1.0f };
    setmfact(&a);
    send_message(connfd, "");
    return;
  }

set_mfact_err:
  send_message(connfd, "usage: muctl set mfact (monitor-number) (value)");
}


void set_borderpx_cmd(int connfd, Monitor *mons) {
  char *sub = strtok(NULL, " ");
  char *end;
  long bp = strtol(sub, &end, 10);
  if (!sub || end == sub || *end != '\0' || bp < 0 || bp > 50) {
    send_message(connfd, "usage: muctl set borderpx (value)");
    return;
  }

  Monitor *m;
  Client *c;
  for (m = mons; m; m = m->next)
    for (c = m->clients; c; c = c->next) {
      c->bw = bp;
      configure(c);
    }
  send_message(connfd, "");
}


void set_gappx_cmd(int connfd, Monitor *mons) {
  (void) mons;
  send_message(connfd, "WARNING: set gappx not implemented");
}

void set_reserved_cmd(int connfd, Monitor *mons) {
  (void) mons;
  send_message(connfd, "WARNING: set reserved not implemented");
}

void set_layout_cmd(int connfd, Monitor *mons) {
  (void) mons;
  char *sub = strtok(NULL, " ");
  const Layout *l;
  Arg a;
  if (!(l = getlayout(sub)))
    send_message(connfd, "usage: muctl set layout (name)");
  else {
    a.v = l;
    setlayout(&a);
    send_message(connfd, "");
  }
}


void tag_cmd(int connfd, Monitor *mons) {
  (void) mons;
  send_message(connfd, "tags command called");
}

void unknown_cmd(int connfd) {
  send_message(connfd, "ERROR: unknown command");
}
