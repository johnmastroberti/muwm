#pragma once
#include "xorg.hpp"
#include "monitor.hpp"

class MuWM {
  private:
    XorgConnection xconn;
    std::vector<Monitor> monitors;

  public:
    MuWM(int argc, char **argv);
    ~MuWM();
};
