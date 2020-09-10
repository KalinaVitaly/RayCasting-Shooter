#include "raycasting.h"

int RayCasting::get_projection_plane_y()                 const { return projection_plane_y; }
int RayCasting::get_projection_plane_x()                 const { return projection_plane_x; }
double RayCasting::get_distance_projection_plane(int _x) const { return distance_projection_plane[_x]; }

void RayCasting::rayCast(Player &player, Map &map)
{
  double ray_step           {0.1};
  double delta_between_rays {player.get_field_of_view() / projection_plane_x};                 //расстояние между лучами
  double xCur_ray_pos       {player.get_x_position()};                                         //текущая позиция луча по оси х
  double yCur_ray_pos       {player.get_y_position()};
  int number_ray_distance   {0};

  double xStep;
  double yStep;
  int step;

  auto deltaABS = [](double a, double b) -> double{
      double result = a - b;
      if (result < 0) result = -result;
      return result;
    };

  for (double cur_ray = player.get_point_of_view() - (player.get_field_of_view() / 2);
       cur_ray < player.get_point_of_view() + (player.get_field_of_view() / 2);
       cur_ray += delta_between_rays)
    {
      xStep        = ray_step * cos(cur_ray);
      yStep        = ray_step * (-sin(cur_ray));
      xCur_ray_pos = player.get_x_position();
      yCur_ray_pos = player.get_y_position();
      step         = 0;

      while (step <= view_deep)
        {
          xCur_ray_pos += xStep;
          yCur_ray_pos += yStep;
          ++step;

          if (map.getMap(floor(yCur_ray_pos), floor(xCur_ray_pos)) == 1)
            break;

          map.getMap(floor(yCur_ray_pos), floor(xCur_ray_pos)) = 8;
        }

      distance_projection_plane[number_ray_distance] = deltaABS(xCur_ray_pos, player.get_x_position()) + deltaABS(yCur_ray_pos, player.get_y_position());
      ++number_ray_distance;
    }
}
