#pragma once

class XorgDisplay {
  private:
    Display *display;

  public:
    XorgDisplay();
    ~XorgDisplay();

    friend class XorgScreen;
    friend class Drw;
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
};


class Drw {
  private:
    XorgDisplay& display;
    XorgScreen& screen;
    Drawable drawable;
    GC gc;
    XftColor normal_border, select_border;

  public:
    Drw(XorgDisplay& d, XorgScreen& s);
    ~Drw();
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

    int update_geometry();

  public:
    XorgConnection();
    ~XorgConnection();
};

