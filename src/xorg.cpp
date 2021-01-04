#include <stdexcept>
#include "error.hpp"
#include "xorg.hpp"

XorgDisplay::XorgDisplay()
  : d{XOpenDisplay(nullptr)} {
  if (!display) throw std::runtime_error("cannot open display");
  check_other_wm(d);
  set_error_handler(d);
}


XorgScreen::XorgScreen(XorgDisplay& d)
  : screen{DefaultScreen(d.display)},
    width{DisplayWidth(d.display, screen)},
    height{DisplayHeight(d.display, screen)},
    root{RootWindow(d.display, screen)} {}


Drw::Drw(XorgDisplay& d, XorgScreen& s)
  : display{d}, screen{s},
    drawable{XCreatePixmap(d.display, s.root, s.width, s.height,
        DefaultDepth(d.display, s.screen))},
    gc{XCreateGC(d.display, s.root, 0, nullptr)} {
  XSetLineAttributes(d.display, gc, 1, LineSolid, CapButt, JoinMiter);
}

Drw::~Drw() {
  XFreePixmap(display.display, drawable);
  XFreeGC(display.display, gc);
}


XorgConnection::XorgConnection()
  : display{}, screen{display},
    drw{display, screen} {
  update_geometry();
}

int XorgConnection::update_geometry() {
}
