#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "cmd.h"
#include "ipc.h"


void parse_word(int connfd, char *cmd, Monitor *mons, Command *commands, int ncmd) {
  // Checks cmd against all of the Commands listed in commands,
  // and calls the associated function for that command.
  // If no match is found, the last command in the list is called
  // (assuming the list is non-empty)
  if (!cmd || !commands) return;
  for (int i=0; i<ncmd; i++) {
    if (!strcmp(cmd, commands[i].name)) {
      (commands[i].cmd)(connfd, mons);
      return;
    }
  }
  (commands[ncmd-1].cmd)(connfd, mons);
}

int parse_int(const char* word, int *n) {
  if (!word || !n || *word == '\0') return 0;
  char *end = NULL;
  long ans = strtol(word, &end, 10);
  if ( *end != '\0' ) return 0;
  *n = (int) ans;
  return 1;
}

int parse_float(const char* word, float *f) {
  if (!word || !f || *word == '\0') return 0;
  char *end = NULL;
  float ans = strtof(word, &end);
  if ( *end != '\0' ) return 0;
  *f = ans;
  return 1;
}

int find_monitor(Monitor *mons, const char* mnum, Monitor **m) {
  // Attempts to match mnum against the given list of monitors
  // Upon success, 1 is returned and the matching monitor is
  // is returned in m. Zero is returned if m is NULL,
  // mnum is not a good integer, or mnum does not match any
  // of the monitors in the list
  int num;
  if (!parse_int(mnum, &num)) return 0;

  for (*m = mons; *m; *m = (*m)->next) {
    if (num == (*m)->num) return 1;
  }
  *m = NULL;
  return 0;
}


void handle_connection(int connfd, Monitor *mons) {
  char *msg = receive_message(connfd);

  Command commands[] = {
    {"mons", mon_cmd}, // implemented
    {"title", title_cmd}, // implemented
    {"set", set_cmd}, // partially implemented
    {"tags", tag_cmd}, // implemented
    {"subscribe", subscribe_cmd},
    {"focus", focus_cmd}, // implemented
    {"killclient", kill_cmd}, // implemented
    {"togglefloat", togglefloat_cmd}, // implemented
    {"", unknown_cmd}
  };

  parse_word(connfd, strtok(msg, " "), mons, commands, LENGTH(commands));

  close(connfd);
  free(msg);
}

void unknown_cmd(int connfd, Monitor *mons) {
  (void) mons;
  send_message(connfd, "ERROR: unknown command");
}
