#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cmd.h"
#include "ipc.h"

void tag_list_cmd(int connfd, Monitor *mons);
void tag_view_cmd(int connfd, Monitor *mons);
void tag_toggleview_cmd(int connfd, Monitor *mons);
void tag_set_cmd(int connfd, Monitor *mons);
void tag_toggleset_cmd(int connfd, Monitor *mons);
void tag_mon_cmd(int connfd, Monitor *mons);
void tag_unknown_cmd(int connfd, Monitor *mons);

void tag_cmd(int connfd, Monitor *mons) {
  Command commands[] = {
    {"list", tag_list_cmd},
    {"view", tag_view_cmd},
    {"toggleview", tag_toggleview_cmd},
    {"set", tag_set_cmd},
    {"toggleset", tag_toggleset_cmd},
    {"mon", tag_mon_cmd},
    {"", tag_unknown_cmd},
  };

  parse_word(connfd, strtok(NULL, " "), mons, commands, LENGTH(commands));
}

// * tags
//   * list (mon)
//   * view (tag)
//   * toggleview (tag)
//   * set (tag)
//   * toggletag (tag)
//   * mon (prev|next)

// List the currently viewed tags on the given monitor
void tag_list_cmd(int connfd, Monitor *mons) {
  Monitor *m;
  if (!find_monitor(mons, strtok(NULL, " "), &m)) {
    send_message(connfd, "usage: muctl tag list (monitor number)");
    return;
  }

  char buf[1024];
  sprintf(buf, "seltags = %u, tagset[0] = %u, tagset[1] = %u", m->seltags, m->tagset[0], m->tagset[1]);

  send_message(connfd, buf); // TODO: figure out which is apropriate and change the message
}

// View a tag on the selected monitor
void tag_view_cmd(int connfd, Monitor *mons) {
  (void) mons;
  int tagnum;
  if (!parse_int(strtok(NULL, " "), &tagnum)) {
    send_message(connfd, "ERROR: bad tag number");
    return;
  }
  if (tagnum < 0 || tagnum > 9) {
    send_message(connfd, "");
    return;
  }
  Arg a = { .ui = tagnum ? 1 << tagnum : ~0 };
  view(&a);
  send_message(connfd, "");
  return;
}


// Toggle viewing a tag on the selected monitor
void tag_toggleview_cmd(int connfd, Monitor *mons) {
  (void) mons;
  int tagnum;
  if (!parse_int(strtok(NULL, " "), &tagnum)) {
    send_message(connfd, "ERROR: bad tag number");
    return;
  }
  if (tagnum <= 0 || tagnum > 9) {
    send_message(connfd, "");
    return;
  }

  Arg a = { .ui = 1 << tagnum };
  toggleview(&a);
  send_message(connfd, "");
  return;
}

// Set the tag of the selected window
void tag_set_cmd(int connfd, Monitor *mons) {
  (void) mons;
  int tagnum;
  if (!parse_int(strtok(NULL, " "), &tagnum)) {
    send_message(connfd, "ERROR: bad tag number");
    return;
  }
  if (tagnum < 0 || tagnum > 9) {
    send_message(connfd, "");
    return;
  }
  Arg a = { .ui = tagnum ? 1 << tagnum : ~0 };
  tag(&a);
  send_message(connfd, "");
  return;
}


// Toggle a tag being set for the selected window
void tag_toggleset_cmd(int connfd, Monitor *mons) {
  (void) mons;
  int tagnum;
  if (!parse_int(strtok(NULL, " "), &tagnum)) {
    send_message(connfd, "ERROR: bad tag number");
    return;
  }
  if (tagnum <= 0 || tagnum > 9) {
    send_message(connfd, "");
    return;
  }

  Arg a = { .ui = 1 << tagnum };
  toggletag(&a);
  send_message(connfd, "");
  return;
}

// Sends the selected client to the previous/next monitor
void tag_mon_prev(int connfd, Monitor *mons) {
  (void) mons;
  Arg a = { .i = -1 };
  tagmon(&a);
  send_message(connfd, "");
}
void tag_mon_next(int connfd, Monitor *mons) {
  (void) mons;
  Arg a = { .i = +1 };
  tagmon(&a);
  send_message(connfd, "");
}
void tag_mon_bad(int connfd, Monitor *mons) {
  (void) mons;
  send_message(connfd, "usage: muctl tag mon (prev|next)");
}
void tag_mon_cmd(int connfd, Monitor *mons) {
  Command commands[] = {
    {"prev", tag_mon_next},
    {"next", tag_mon_prev},
    {"", tag_mon_bad}
  };

  parse_word(connfd, strtok(NULL, " "), mons, commands, LENGTH(commands));
}

void tag_unknown_cmd(int connfd, Monitor *mons) {
  (void) mons;
  send_message(connfd, "usage: muctl tag (list|view|toggleview|set|toggleset|mon)");
}

