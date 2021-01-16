#pragma once
#include "xorg.hpp"
#include "monitor.hpp"

class MuWM {
  private:
    XorgConnection xconn;
    std::list<Monitor> monitors;
    Monitor* selected_monitor;

    // Part of initial setup, this gets all of the already open windows
    // and adds them to the appropriate monitor's list of clients
    void scan_for_clients();

  public:
    MuWM(int argc, char **argv);
    ~MuWM();
};
