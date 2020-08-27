#include <iostream>
#include <string>
#include <cmath>
#include <curses.h>

#include <iostream>
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

const int projection_plane = 320;   //видимая часть
const int cub_size         = 64;    //размер измерения куба из которого состоит мир
const double view_deep     = 30.0;  //глубина бросания лучей

double distance_projection_plane[projection_plane];   //расстояние до припятствий
//double tangens_values[0];                           //значение тангенсов углов

int worldMap[MAP_HEIGHT][MAP_WIDTH] = {               //карта мира с обозначением припятствий
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
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

void printMap(struct Player &player);
void setup(struct Player &player);
void getInput(struct Player &player);

int main()
{

  struct Player player;
  setup(player);
  while (1)
    {
       system("clear");
       printMap(player);
       getInput(player);
    }

//  initscr();                   // Переход в curses-режим
//  printw("Hello world!\n");    // Отображение приветствия в буфер
//  refresh();                   // Вывод приветствия на настоящий экран
//  nodelay(stdscr, true);
//  //noecho();
//  getch();                     // Ожидание нажатия какой-либо клавиши пользователем
//  endwin();                    // Выход из curses-режима. Обязательная команда.

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
          else if (worldMap[i][j] == 0)
            {
              cout << " ";
              continue;
            }
          cout << "1";
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
      case 65  :  {                                                                                                           //forward
          player.x_position += player.step * cos(player.point_of_view);
          player.y_position += player.step * (-sin(player.point_of_view));
          break;
          }
      case 66  :  {                                                                                                           //back
          player.x_position += (-player.step) * cos(player.point_of_view);
          player.y_position += (-player.step) * (-sin(player.point_of_view));
          break;
        }
      case 68  :  player.point_of_view += 0.1; if (player.point_of_view >= 2 * M_PI) player.point_of_view -= 2 * M_PI; break;           //right
      case 67  :  player.point_of_view -= 0.1; if (player.point_of_view < 0) player.point_of_view += 2 * M_PI;  break;             //left
    }
  cout << ch << endl;
}

//void rayCast(struct Player &player)
//{
//  int number_current_ray    = 0;                                                       //номер текущего луча
//  double angle_current_ray  = player.point_of_view - (player.field_of_view / 2);       //угол текущего луча
//  double delta_between_rays = player.field_of_view / projection_plane;                 //расстояние между лучами
//  double ray_step           = 10.0;                                                     //шаг луча за одну итерацию
//  double xCur_ray_pos       = player.x_position;                                       //текущая позиция луча по оси х
//  double yCur_ray_pos       = player.y_position;                                       //текущая позиция луча по оси y
//  double xStep;
//  double yStep;
//  double distance2wall;                                                                //расстояние до стены
//  bool is_wall              = false;                                                   //луч зашел в стену

//  for(int i = 0; i < projection_plane; ++i)
//    {
//      distance2wall = 0.0;
//      yStep = ray_step * sin(angle_current_ray);
//      xStep = ray_step * cos(angle_current_ray);
//      ++number_current_ray;

//      while(!is_wall && distance2wall < view_deep)
//        {
//          distance2wall += ray_step;
//          xCur_ray_pos += xStep;
//          yCur_ray_pos += yStep;

//          if (static_cast<int>(xCur_ray_pos))

//        }
//      angle_current_ray += delta_between_rays;
//    }
//}
