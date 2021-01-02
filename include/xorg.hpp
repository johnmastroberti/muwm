#pragma once

typedef int (*XorgErrorHandlerFunc)(Display*, XErrorEvent*);


class XorgScreen {
  private:
    Screen *screen;
    int width, height;
};

class XorgDisplay {
  private:
    Display *d;

  public:
    XorgDisplay();
    ~XorgDisplay();
};


// XorgConnection encapsulates all communication between
// muwm and the xorg server. Any communication to/from
// xorg should go through an instance of this class.
// Only one instance of XorgConnection should exist at a time.
class XorgConnection {
  private:
    Display *display;
    XorgErrorHandlerFunc *default_error_handler;

    // Throws an exception if another window manager is already running
    void check_other_wm();
    // Supporting function used by check_other_wm
    int xerrorstart(Display*, XErrorEvent*);
    // Error handling function for xorg to call
    int xerror(Display*, XErrorEvent*);

  public:
    XorgConnection();
    ~XorgConnection();
};

