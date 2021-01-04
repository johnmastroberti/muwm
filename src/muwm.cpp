#include "muwm.hpp"

MuWM::MuWM(int argc, char **argv) : xconn{}, monitors{} {
  xconn.update_geometry(monitors)
}



int main(int argc, char **argv) {
  MuWM muwm(argc, argv);
  muwm.run();
  return EXIT_SUCCESS;
}
