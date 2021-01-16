#pragma once
#include <string>
#include "geom.hpp"

class Monitor;

class Client {
  private:
    std::string name;
    float mina, maxa;
    Geom geom, old_geom;
    int basew, baseh, incw, inch, maxw, maxh, minw, minh;
    int bw, oldbw;
    unsigned tags;
    bool is_fixed, is_floating, is_urgent, never_focus, old_state, is_fullscreen;
    Monitor *mon;
    Window win;

    // Constructor is private because only XorgConnection should be
    // constructing Clients
    Client(Window, XWindowAttributes*); // compare to manage() in dwm

  public:
    ~Client(); // compare to unmanage() in dwm

    friend class XorgConnection;
};
