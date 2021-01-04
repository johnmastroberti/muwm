#pragma once
class MuWM {
  private:
    XorgConnection m_xconn;

  public:
    MuWM(int argc, char **argv);
    ~MuWM();
};
