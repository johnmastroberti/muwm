#include <stdexcept>
#include "error.hpp"
// Default error handler saved as a fallback
static XorgErrorHandlerFunc default_error_handler;

int xerrorstart(Display*, XErrorEvent*) {
  throw std::runtime_error("another window manager is already running");
  return -1;
}

int xerror(Display *dpy, XErrorEvent *ee) {
  if (ee->error_code == BadWindow
  || (ee->request_code == X_SetInputFocus && ee->error_code == BadMatch)
  || (ee->request_code == X_PolyText8 && ee->error_code == BadDrawable)
  || (ee->request_code == X_PolyFillRectangle && ee->error_code == BadDrawable)
  || (ee->request_code == X_PolySegment && ee->error_code == BadDrawable)
  || (ee->request_code == X_ConfigureWindow && ee->error_code == BadMatch)
  || (ee->request_code == X_GrabButton && ee->error_code == BadAccess)
  || (ee->request_code == X_GrabKey && ee->error_code == BadAccess)
  || (ee->request_code == X_CopyArea && ee->error_code == BadDrawable))
    return 0;
  fprintf(stderr, "muwm: fatal error: request code=%d, error code=%d\n",
      ee->request_code, ee->error_code);
  return default_error_handler(dpy, ee); /* may call exit */
}

void check_other_wm(Display *dpy) {
  default_error_handler = XSetErrorHandler(xerrorstart);
  XSelectInput(dpy, DefaultRootWindow(dpy),
      SubstructureRedirectMask);
  XSync(dpy, False);
}

void set_error_handler(Display *dpy) {
	XSetErrorHandler(xerror);
	XSync(dpy, False);
}
