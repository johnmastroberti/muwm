#include "monitor.hpp"

static Layout stack_layout = {
  .name = "stack",
  .symbol = "[]=",
  .arrange = [](Monitor *) {
    // TODO
  }
}

static Layout grid_layout = {
  .name = "grid",
  .symbol = "[+]",
  .arrange = [](Monitor *) {
    // TODO
  }
}

Monitor::Monitor()
  : layouts{stack_layout, grid_layout},
    selected_layout{&layouts[0]} {}

Monitor::~Monitor() {
  // TODO
}


void Monitor::set_screen_size(int x, int y, int w, int h) {
  screen_size = { .x = x, .y = y, .w = w, .h = h};
}


void Monitor::update_window_area(int tr, int lr, int br, int rr) {
  window_area = screen_size;
  window_area.h -= tr + br; // top and bottom eat into height
  window_area.w -= lr + rr; // left and right eat into width
  window_area.x += lr; // left offsets x
  window_area.y += tr; // top offsets y
}


void Monitor::arrange() {
  selected_layout->arrange(this);
}
