#include <stdexcept>
#include <span>
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

bool XorgConnection::update_geometry(std::vector<Monitor>& mons) {
  bool dirty = false;

  if (XineramaIsActive(display.display)) {
    int n, nn;
    Client *c;
    Monitor *m;
    XineramaScreenInfo *info = XineramaQueryScreens(display.display, &nn);
    std::span<XineramaScreenInfo> info_span{info, nn};
    std::vector<XineramaScreenInfo> unique_screens;

    n = mons.size();
    // only consider unique geometries as separate screens
    auto screen_not_in_unique = [&unique_screens](XineramaScreenInfo s1) {
      return unique_screens.end() == std::find_if(unique_screens.begin(),
              unique_screens.end(), [](XineramaScreenInfo s2) {
                  return s1.x_org == s2.x_org && s1.y_org == s2.y_org
                    && s1.width == s2.width && s1.height == s2.height; })};

    std::copy_if(info_span.begin(), info_span.end(),
        std::back_inserter(unique_screens),
        screen_not_in_unique);

    XFree(info); // DO NOT USE info_span AFTER THIS POINT
    nn = unique_screens.size();
    if (n <= nn) { // new monitors available
      mons.resize(nn);
      for (int i = 0; auto& m : mons) {
        m.num = i;
        auto g = Geom{
            .x = unique_screens[i].x_org,
            .y = unique_screens[i].y_org,
            .w = unique_screens[i].width,
            .h = unique_screens[i].height
        };
        if (m.get_screen_size() != g) {
          dirty = true;
          m.update_screen_size(g.x, g.y, g.w, g.h);
          m.update_window_area(0, 0, 0, 0);
        }
        i++;
      }
    } else { // less monitors available (nn < n)
      for (i=nn; i<n; i++) {
        dirty = true;
        m = mons[i];
        auto old_clients_size = mons[0].clients.size();
        std::move(m.clients.begin(), m.clients.end(),
            std::back_inserter(mons[0].clients));
        auto new_clients_size = mons[0].clients.size();
        for (unsigned j=old_clients_size; j<new_clients_size; j++)
          mons[0].stack.push_back(&(mons[0].clients[j]));
      }
    }
  } else {
    if (mons.size() == 0) {
      mons.resize(1);
    }
    if (mons.screen_size.w != screen.get_width()
        || mons.screen_size.h != screen.get_height()) {
      m.update_screen_size(0, 0, screen.get_width(), screen.get_height());
      m.update_window_area(0, 0, 0, 0);
    }
  }

  if (dirty) {
    // Update selected monitor (TODO)
  }
  return dirty;
}


void XorgConnection::setup_atoms() {
	wmatom[WMProtocols] = XInternAtom(display.display,
      "WM_PROTOCOLS", False);
	wmatom[WMDelete] = XInternAtom(display.display,
      "WM_DELETE_WINDOW", False);
	wmatom[WMState] = XInternAtom(display.display,
      "WM_STATE", False);
	wmatom[WMTakeFocus] = XInternAtom(display.display,
      "WM_TAKE_FOCUS", False);
	netatom[NetActiveWindow] = XInternAtom(display.display,
      "_NET_ACTIVE_WINDOW", False);
	netatom[NetSupported] = XInternAtom(display.display,
      "_NET_SUPPORTED", False);
	netatom[NetWMName] = XInternAtom(display.display,
      "_NET_WM_NAME", False);
	netatom[NetWMState] = XInternAtom(display.display,
      "_NET_WM_STATE", False);
	netatom[NetWMCheck] = XInternAtom(display.display,
      "_NET_SUPPORTING_WM_CHECK", False);
	netatom[NetWMFullscreen] = XInternAtom(display.display,
      "_NET_WM_STATE_FULLSCREEN", False);
	netatom[NetWMWindowType] = XInternAtom(display.display,
      "_NET_WM_WINDOW_TYPE", False);
	netatom[NetWMWindowTypeDialog] = XInternAtom(display.display,
      "_NET_WM_WINDOW_TYPE_DIALOG", False);
	netatom[NetClientList] = XInternAtom(display.display,
      "_NET_CLIENT_LIST", False);
}

void XorgConnection::netwm_check_setup() {
	/* supporting window for NetWMCheck */
	wmcheckwin = XCreateSimpleWindow(display.display, screen.root,
      0, 0, 1, 1, 0, 0, 0);
	XChangeProperty(display.display, wmcheckwin, netatom[NetWMCheck],
      XA_WINDOW, 32, PropModeReplace, (unsigned char *) &wmcheckwin, 1);
	Atom utf8string = XInternAtom(display.display, "UTF8_STRING", False);
	XChangeProperty(display.display, wmcheckwin, netatom[NetWMName],
      utf8string, 8, PropModeReplace, (unsigned char *) "dwm", 3);
	XChangeProperty(display.display, root, netatom[NetWMCheck],
      XA_WINDOW, 32, PropModeReplace, (unsigned char *) &wmcheckwin, 1);
}

void XorgConnection::ewmh_support() {
	/* EWMH support per view */
	XChangeProperty(display.display, screen.root, netatom[NetSupported],
      XA_ATOM, 32, PropModeReplace, (unsigned char *) netatom, NetLast);
	XDeleteProperty(display.display, screen.root, netatom[NetClientList]);
}


