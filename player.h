#pragma once

#include <cmath>
#include <curses.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <map.h>

class Player
{
public:
  Player();

  void getInput(Map &map);
  void moveForward(Map &map);
  void moveBack(Map &map);

  double get_x_position()    const;
  double get_y_position()    const;
  double get_point_of_view() const;
  double get_field_of_view() const;
  double get_step()          const;

  void set_x_position(double _x);
  void set_y_position(double _y);
  void set_point_of_view(double _view);
  void set_field_of_view(double _field);
  void set_step(double _step);

private:
  double x_position;            //позиция игрока по оси х
  double y_position;            //позиция игрока по оси у
  double point_of_view;         //направление взгляда
  double field_of_view;         //диапазон видимости
  double step;                  //шаг за ход
};

