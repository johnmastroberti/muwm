#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xresource.h>
#include <X11/Xft/Xft.h>

#include "util.h"
#include "colors.h"

void create_color(Display *dpy, int screen, XftColor *c, const char *name) {
  if (!dpy || !c || !name) return;
  if (!XftColorAllocName(dpy, DefaultVisual(dpy, screen),
        DefaultColormap(dpy, screen), name, c))
    die("error, cannot allocate color '%s'", name);
}


int valid_hex_char(char c) {
  return BETWEEN(C, '0', '9')
    || BETWEEN(C, 'A', 'F')
    || BETWEEN(C, 'a', 'f');
}


int valid_hex_color(const char *name) {
  if (!name || strnlen(name, 8) != 7 || name[0] != '#')
    return 0;
  for (int i=1; i<7; i++)
    if (!valid_hex_char(name[i])) return 0;
  return 1;
}


void xrdb_load_color(XrmDatabase *xrdb, const char *color_name, char **color_out) {
  XrmValue value;
  if (!XrmGetResource(*xrdb, color_name, NULL, NULL, &value)) return;
  if (!valid_hex_color(value.addr)) return;
  strncpy(*color_out, value.addr, 7);
  (*color_out)[7] = '\0';
}


void update_colors(Display *dpy, int screen, Colors *c) {
  if (!dpy || !c) return;

  char *resm;
  XrmDatabase xrdb;
  char normal[] = "#000000";
  char selected[] = "#FFFFFF";

  if (resm = XResourceManagerString(dpy)) {
    if (xrdb = XrmGetStringDatabase(resm)) {
      xrdb_load_color("muwm.normborder", &normal);
      xrdb_load_color("muwm.selborder", &selected);
    }
  }

  create_color(dpy, screen, &(c->normal), normal);
  create_color(dpy, screen, &(c->selected), selected);
}

