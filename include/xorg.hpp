#pragma once
#include "monitor.hpp"
#include "client.hpp"

class XorgDisplay {
  private:
    Display *display;

  public:
    XorgDisplay();
    ~XorgDisplay();

    friend class XorgScreen;
    friend class Drw;
    friend class XorgConnection;
};


class XorgScreen {
  private:
    int screen;
    int width, height;
    Window root;

  public:
    XorgScreen(XorgDisplay& display);
    ~XorgScreen();

    int get_screen();
    int get_width() const;
    int get_height() const;
    Window get_root();

    friend class Drw;
    friend class XorgConnection;
};


class Drw {
  private:
    XorgDisplay& display;
    XorgScreen& screen;
    Drawable drawable;
    GC gc;
    XftColor normal_border, select_border;

    Cursor cursor_normal, cursor_resize, cursor_move;


  public:
    Drw(XorgDisplay& d, XorgScreen& s);
    ~Drw();

    friend class XorgConnection;
};


// XorgConnection encapsulates all communication between
// muwm and the xorg server. Any communication to/from
// xorg should go through an instance of this class.
// Only one instance of XorgConnection should exist at a time.
class XorgConnection {
  private:
    XorgDisplay display;
    XorgScreen screen;
    Drw drw;

    // various standards compliance stuff that I dont understand:
    Atom wmatom[WMLast], netatom[NetLast];
    Window wmcheckwin;
    void setup_atoms();
    void netwm_check_setup();
    void ewmh_support();

    // Selecting the events that we want xorg to tell us about
    void select_events();

    // Keyboard input handling
    // TODO: substantial changes will likely be necessary here since we are
    // leaving keyboard shortcuts up to an external program
    int numlock_mask;
    void update_numlock_mask();

  public:
    XorgConnection();
    ~XorgConnection();

    // Helper function for MuWM to get monitor geometry
    int update_geometry(std::vector<Monitor>& mons);
    void focus_root();

    // Helper functions for Clients
};

