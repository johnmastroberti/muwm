#include <stdexcept>
#include "error.hpp"
#include "xorg.hpp"

XorgDisplay::XorgDisplay() : d{XOpenDisplay(nullptr)} {
  if (!display) throw std::runtime_error("cannot open display");
  check_other_wm(d);
  set_error_handler(d);
}


XorgConnection::XorgConnection() : display{} {


}


