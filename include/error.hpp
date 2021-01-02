#pragma once

typedef int (*XorgErrorHandlerFunc)(Display*, XErrorEvent*);


// Throws an exception if another window manager is running
void check_other_wm(Display *dpy);
// Sets the main error handling function
void set_error_handler(Display *dpy);
// Sets a dummy error handler (ignores errors)
void set_dummy_handler(Display *dpy);
