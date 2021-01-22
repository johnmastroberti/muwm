#pragma once
#include "muwm.h"

typedef void (*CommandFunc)(int, Monitor*);
typedef struct {
  const char *name;
  CommandFunc cmd;
} Command;

// Common
void handle_connection(int connfd, Monitor *mons);
void parse_word(int connfd, char *cmd, Monitor *mons, Command *commands, int ncmd);
int find_monitor(Monitor *mons, const char* mnum, Monitor **m);
int parse_int(const char* word, int *n);
int parse_float(const char* word, float *f);

// Commands
void mon_cmd(int connfd, Monitor *mons);
void title_cmd(int connfd, Monitor *mons);
void subscribe_cmd(int connfd, Monitor *mons);
void set_cmd(int connfd, Monitor *mons);
void tag_cmd(int connfd, Monitor *mons);
void focus_cmd(int connfd, Monitor *mons);
void kill_cmd(int connfd, Monitor *mons);
void togglefloat_cmd(int connfd, Monitor *mons);
void unknown_cmd(int connfd, Monitor *mons);
