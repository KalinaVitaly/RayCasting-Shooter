#pragma once

#include "player.h"
#include "map.h"

class RayCasting
{
private:
  static constexpr int projection_plane_x {240};
  static constexpr int projection_plane_y {80};
  static constexpr double view_deep       {500};
  double distance_projection_plane[projection_plane_x];

public:
  void rayCast(Player &player, Map &map);

  int get_projection_plane_y() const;
  int get_projection_plane_x() const;

  double get_distance_projection_plane(int _x) const;
};

