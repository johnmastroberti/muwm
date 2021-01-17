#pragma once

typedef struct {
  XftColor normal, selected;
} Colors;

void update_colors(Display *dpy, int screen, Colors *c);
