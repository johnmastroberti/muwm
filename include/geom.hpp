#pragma once

struct Geom {
  int x, y, w, h;

  bool operator==(const Geom&) = default;
}
