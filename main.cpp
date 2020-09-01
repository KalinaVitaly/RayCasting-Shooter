#include <iostream>
#include <cmath>
#include <curses.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#define MAP_WIDTH 24    //ширина карты
#define MAP_HEIGHT 24   //высота карты

using namespace std;

struct Player{
  double x_position;            //позиция игрока по оси х
  double y_position;            //позиция игрока по оси у
  double point_of_view;         //направление взгляда
  double field_of_view;         //диапазон видимости
  double step;                  //шаг за ход
};

const int projection_plane_x = 240;   //видимая часть по y
const int projection_plane_y = 80;   //видимая часть по x
const double view_deep       = 500;   //глубина бросания лучей
int count = 0;

double distance_projection_plane[projection_plane_x];   //расстояние до припятствий
//double tangens_values[0];                           //значение тангенсов углов

int worldMap[MAP_HEIGHT][MAP_WIDTH] = {               //карта мира с обозначением припятствий
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,0,0,0,1,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};

int c_worldMap[MAP_HEIGHT][MAP_WIDTH];

void printMap(struct Player &player);
void setup(struct Player &player);
void getInput(struct Player &player);
void rayCast(struct Player &player);
void print3D();

void copyMap();

int main()
{
  struct Player player;
  setup(player);

  while (1)
    {
       system("clear");
       rayCast(player);
//      printMap(player);
//       cout << endl << endl;
//       for(int i = 0; i < projection_plane_x; i += 5)
//         cout << fixed << distance_projection_plane[i] << " ";
//       cout << endl << endl;
       copyMap();
       print3D();
       getInput(player);
    }

  return 0;
}

void setup(struct Player &player)
{
  //установка стандартных значений
  player.x_position     = 12;
  player.y_position     = 12;
  player.point_of_view  = 0;
  player.field_of_view  = M_PI / 3;
  player.step           = 0.1;
}

void printMap(struct Player &player)
{
  cout << "X position: " << player.x_position << "   Y position: " << player.y_position <<
          "   POV: " << player.point_of_view << endl;

  for(int i = 0; i < MAP_HEIGHT; ++i)
    {
      for(int j = 0; j < MAP_WIDTH; ++j)
        {
          if (j == static_cast<int>(player.x_position) && i == static_cast<int>(player.y_position))
            {
              cout << "P";
              continue;
            }
          else if (c_worldMap[i][j] == 0)
            {
              cout << " ";
              continue;
            }
          else if(c_worldMap[i][j] == 1)
            {
              cout << "1";
            }
          else if (c_worldMap[i][j] == 8)
            cout << "8";
        }
      cout << "\n";
    }
}

void getInput(struct Player &player)
{
  struct termios oldt,
  newt;
  int ch;
  tcgetattr( STDIN_FILENO, &oldt );
  newt = oldt;
  newt.c_lflag &= ~( ICANON | ECHO );
  tcsetattr( STDIN_FILENO, TCSANOW, &newt );
  ch = getchar();
  tcsetattr( STDIN_FILENO, TCSANOW, &oldt );

    switch (ch) {
      case 65  :  {                                                                                                                //forward
          player.x_position += player.step * cos(player.point_of_view);
          player.y_position += player.step * (-sin(player.point_of_view));
          break;
          }
      case 66  :  {                                                                                                                //back
          player.x_position += (-player.step) * cos(player.point_of_view);
          player.y_position += (-player.step) * (-sin(player.point_of_view));
          break;
        }
      case 67  :  player.point_of_view += 0.1; if (player.point_of_view >= 2 * M_PI) player.point_of_view -= 2 * M_PI; break;      //right
      case 68  :  player.point_of_view -= 0.1; if (player.point_of_view < 0) player.point_of_view += 2 * M_PI;  break;             //left
    }
}

void rayCast(struct Player &player)
{
  double ray_step           = 0.1;
  double delta_between_rays = player.field_of_view / projection_plane_x;                 //расстояние между лучами
  double xCur_ray_pos       = player.x_position;                                       //текущая позиция луча по оси х
  double yCur_ray_pos       = player.y_position;
  int number_ray_distance   = 0;
  double xStep;
  double yStep;
  int step;

  auto abs = [](double a, double b) -> double{
      double result = a - b;
      if (result < 0) result = -result;
      return result;
    };

  for (double cur_ray = player.point_of_view - (player.field_of_view / 2); cur_ray < player.point_of_view + (player.field_of_view / 2); cur_ray += delta_between_rays)
    {
      xStep        = ray_step * cos(cur_ray);
      yStep        = ray_step * (-sin(cur_ray));
      xCur_ray_pos = player.x_position;
      yCur_ray_pos = player.y_position;
      step = 0;

      while (step <= view_deep)
        {
          xCur_ray_pos += xStep;
          yCur_ray_pos += yStep;
          ++step;

          if (c_worldMap[static_cast<int>(yCur_ray_pos)][static_cast<int>(xCur_ray_pos)] == 1)
            break;

          c_worldMap[static_cast<int>(yCur_ray_pos)][static_cast<int>(xCur_ray_pos)] = 8;
        }

      distance_projection_plane[number_ray_distance] = abs(xCur_ray_pos, player.x_position) + abs(yCur_ray_pos, player.y_position);
      //distance_projection_plane[number_ray_distance] = sqrt(pow(abs(xCur_ray_pos, player.x_position), 2) + pow(abs(yCur_ray_pos, player.y_position), 2));
      ++number_ray_distance;
    }
}

void copyMap()
{
  for(int i = 0; i < MAP_HEIGHT; ++i)
    {
      for(int j = 0; j < MAP_WIDTH; ++j)
        {
          c_worldMap[i][j] = worldMap[i][j];
        }
    }
}


void print3D()
{
  double y1;
  double y2;
  double wall_width;
  char buffer[projection_plane_y][projection_plane_x];

  for(int i = 0; i < projection_plane_y; ++i)
    {
      for(int j = 0; j < projection_plane_x; ++j)
        {
          buffer[i][j] = '0';
        }
    }

  for(int i = 0; i < projection_plane_x; i++)
    {
      wall_width = projection_plane_y / distance_projection_plane[i];
      y1 = (projection_plane_y - wall_width) / 2;   //до этой координаты идет потолок
      y2 = y1 + wall_width;                         //с этой координаты идет пол

      for(int j = 0; j < projection_plane_y; ++j)
        {
          if      (j < y1)              buffer[j][i] = ' ';
          else if (j >= y1 && j <= y2)  buffer[j][i] = '|';
          else                          buffer[j][i] = '-';
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
