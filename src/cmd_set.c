#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cmd.h"
#include "ipc.h"


void set_mfact_cmd(int connfd, Monitor *mons);
void set_borderpx_cmd(int connfd, Monitor *mons);
void set_gappx_cmd(int connfd, Monitor *mons);
void set_reserved_cmd(int connfd, Monitor *mons);
void set_layout_cmd(int connfd, Monitor *mons);
void set_unknown_cmd(int connfd, Monitor *mons);

void set_cmd(int connfd, Monitor *mons) {
  Command commands[] = {
    {"mfact", set_mfact_cmd},
    {"borderpx", set_borderpx_cmd},
    {"gappx", set_gappx_cmd},
    {"reserved", set_reserved_cmd},
    {"layout", set_layout_cmd},
    {"", set_unknown_cmd}
  };

  parse_word(connfd, strtok(NULL, " "), mons, commands, LENGTH(commands));
}

void set_unknown_cmd(int connfd, Monitor *mons) {
  (void) mons;
  send_message(connfd, "ERROR: bad sub command");
}


void set_mfact_cmd(int connfd, Monitor *mons) {
  char *mon = strtok(NULL, " ");
  Monitor *m;
  if (!find_monitor(mons, mon, &m))
    goto set_mfact_err;

  float mf = 0.0f;
  parse_float(strtok(NULL, " "), &mf);
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
