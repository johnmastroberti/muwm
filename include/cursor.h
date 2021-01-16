#pragma once
#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>

typedef struct {
  Cursor normal, resize, move;
} Cursors;

void create_cursors(Display *dpy, Cursors *c);
void free_cursors(Display *dpy, Cursors *c);
