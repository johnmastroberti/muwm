#include "muwm.hpp"

MuWM::MuWM(int argc, char **argv) : xconn{}, monitors{} {
  xconn.update_geometry(monitors);
  // TODO: use xresources to get color schemes
  scan_for_clients();
}



int main(int argc, char **argv) {
  MuWM muwm(argc, argv);
  muwm.run();
  return EXIT_SUCCESS;
}
