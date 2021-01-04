#pragma once
#include <string>
#include <vector>
#include "geom.hpp"
#include "client.hpp"

class Monitor;

class Layout {
  public:
    std::string name, symbol;
    void (*arrange)(Monitor*);
};

class Monitor {
  private:
    float mfact = 0.5;
    int num;
    Geom screen_size, window_area;
    unsigned selected_tags;
    unsigned selected_layout;
    unsigned tagset[2] = {1,1};
    std::vector<Client> clients;
    std::vector<Client*> stack;
    Client* selected_client;
    std::vector<Layout> layouts;
    Layout* selected_layout;

  public:
    Monitor();
    ~Monitor();

    void arrange();
    void set_screen_size(int x, int y, int w, int h);
    void update_window_area(int top_reserved, int left_reserved, int bottom_reserved, int right_reserved);

    inline Geom get_screen_size() const { return screen_size; }
    inline Geom get_window_area() const { return window_area; }

    friend class Layout;
    friend XorgConnection::update_geometry(std::vector<Monitor>&);
};

