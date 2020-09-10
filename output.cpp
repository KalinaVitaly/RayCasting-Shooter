#include "output.h"

void Output::printMap(const Player &player, Map &map)
{
  using namespace std;

  cout << "X position: " << player.get_x_position() << "   Y position: " << player.get_y_position() <<
          "   POV: " << player.get_point_of_view() << endl;

  for(int i = 0; i < map.get_height(); ++i)
    {
      for(int j = 0; j < map.get_width(); ++j)
        {
          if (j == static_cast<int>(player.get_x_position()) && i == static_cast<int>(player.get_y_position()))
            {
              cout << "P";
              continue;
            }
          else if (map.getMap(i, j) == 0)
            {
              cout << " ";
              continue;
            }
          else if (map.getMap(i, j) == 1)
            {
              cout << "1";
            }
          else if (map.getMap(i, j) == 8)
            cout << "8";
        }
      cout << "\n";
    }
}



void Output::print(const RayCasting &rct)
{
    for(int i = 0; i < rct.get_projection_plane_y(); ++i)
      {
        for(int j = 0; j < rct.get_projection_plane_x(); ++j)
          {
            printf("%c", buffer[i][j]);
          }
        printf("\n");
      }
}

void Output::rendering(const Player &player, const RayCasting &rct)
{
  double sky_bound;
  double floor_bound;
  double wall_width;

  for(int i = 0; i < rct.get_projection_plane_x(); ++i)
      {
        wall_width  = rct.get_projection_plane_y() / (rct.get_distance_projection_plane(i) + 0.5);
        sky_bound   = (rct.get_projection_plane_y() - wall_width) / 2;
        floor_bound = sky_bound + wall_width;

        for(int j = 0; j < rct.get_projection_plane_y(); ++j)
          {

            if (j <= sky_bound) buffer[j][i] = ' ';
            else if (j < floor_bound)
              {
                if (rct.get_distance_projection_plane(i) <= 10)      buffer[j][i] = '|';
                else if (rct.get_distance_projection_plane(i) <= 15) buffer[j][i] = ':';
                else                                                 buffer[j][i] = '.';
              }
            else buffer[j][i] = '_';
           }

      }
}
