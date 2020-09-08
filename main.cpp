#include <iostream>
#include <cmath>
#include <curses.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#include "player.h"
#include "map.h"

using namespace std;

const int projection_plane_x = 240;   //видимая часть по y
const int projection_plane_y = 80;   //видимая часть по x
const double view_deep       = 500;   //глубина бросания лучей

double distance_projection_plane[projection_plane_x];   //расстояние до припятствий

void printMap(const Player &player, Map &map);
void rayCast(Player &player, Map &map);
void print3D();

int main()
{
  Player player;
  Map map;
  map.readWorldMapFromFile();

  while (1)
    {
      system("clear");
      rayCast(player, map);
      printMap(player, map);
      map.clean();
      print3D();
      player.getInput(map);
    }

  return 0;
}

void printMap(const Player &player, Map &map)
{
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

void rayCast(Player &player, Map &map)
{
  double ray_step           = 0.1;
  double delta_between_rays = player.get_field_of_view() / projection_plane_x;                 //расстояние между лучами
  double xCur_ray_pos       = player.get_x_position();                                         //текущая позиция луча по оси х
  double yCur_ray_pos       = player.get_y_position();
  int number_ray_distance   = 0;

  double xStep;
  double yStep;
  int step;

  auto abs = [](double a, double b) -> double{
      double result = a - b;
      if (result < 0) result = -result;
      return result;
    };

  for (double cur_ray = player.get_point_of_view() - (player.get_field_of_view() / 2); cur_ray < player.get_point_of_view() +
       (player.get_field_of_view() / 2); cur_ray += delta_between_rays)
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

          if (map.getMap(static_cast<int>(yCur_ray_pos), static_cast<int>(xCur_ray_pos)) == 1)
            break;

          map.getMap(static_cast<int>(yCur_ray_pos), static_cast<int>(xCur_ray_pos)) = 8;
        }

      distance_projection_plane[number_ray_distance] = abs(xCur_ray_pos, player.get_x_position()) + abs(yCur_ray_pos, player.get_y_position());
      ++number_ray_distance;
    }
}

void print3D()
{
  double y1;
  double y2;
  double wall_width;
  char buffer[projection_plane_y][projection_plane_x];

  for(int i = 0; i < projection_plane_x; i++)
    {
      wall_width = projection_plane_y / (distance_projection_plane[i] + 0.5);
      y1 = (projection_plane_y - wall_width) / 2;                                 //до этой координаты идет потолок
      y2 = y1 + wall_width;                                                       //с этой координаты идет пол

      for(int j = 0; j < projection_plane_y; ++j)
        {
          if      (j <= y1)              buffer[j][i] = ' ';
          else if (j > y1 && j < y2)
            {
              if (distance_projection_plane[i] <= 10)      buffer[j][i] = '|';
              else if (distance_projection_plane[i] <= 15) buffer[j][i] = ':';
              else                                         buffer[j][i] = '.';
            }
          else
            {
              buffer[j][i] = '_';
            }
        }
    }

  for(int i = 0; i < projection_plane_y; ++i)
    {
      for(int j = 0; j < projection_plane_x; ++j)
        {
          printf("%c", buffer[i][j]);
        }
      printf("\n");
    }
}
