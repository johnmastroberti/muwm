#include "muwm.hpp"

MuWM::MuWM(int argc, char **argv) : m_xconn{} {

}



int main(int argc, char **argv) {
  MuWM muwm(argc, argv);
  muwm.run();
  return EXIT_SUCCESS;
}
