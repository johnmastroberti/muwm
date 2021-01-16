#include "cursor.h"

enum { CurNormal, CurResize, CurMove };

void create_cursors(Display *dpy, Cursors *c) {
  if (!dpy || !c) return;
  c->normal = XCreateFontCursor(dpy, CurNormal);
  c->resize = XCreateFontCursor(dpy, CurResize);
  c->move   = XCreateFontCursor(dpy, CurMove);
}

void free_cursors(Display *dpy, Cursors *c) {
  if (!dpy || !c) return;
  XFreeCursor(dpy, CurNormal);
  XFreeCursor(dpy, CurResize);
  XFreeCursor(dpy, CurMove);
}
