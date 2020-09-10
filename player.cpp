#include "player.h"

double Player::get_x_position()    const {return x_position;}
double Player::get_y_position()    const {return y_position;}
double Player::get_point_of_view() const {return point_of_view;}
double Player::get_field_of_view() const {return field_of_view;}
double Player::get_step()          const {return step;}

void Player::set_x_position(double _x)       {x_position = _x;}
void Player::set_y_position(double _y)       {y_position = _y;}
void Player::set_point_of_view(double _view) {point_of_view = _view;}
void Player::set_field_of_view(double _field){field_of_view = _field;}
void Player::set_step(double _step)          {step = _step;}


void Player::moveForward(Map &map)
{
  double stepX = step * cos(point_of_view);
  double stepY = step * (-sin(point_of_view));

  if (map.getMap(floor(y_position + stepY), floor(x_position + stepX)) != 1)
  {
      x_position += stepX;
      y_position += stepY;
  }
}

void Player::moveBack(Map &map)
{
  double stepX = (-step) * cos(point_of_view);
  double stepY = (-step) * (-sin(point_of_view));

  if (map.getMap(floor(y_position + stepY), floor(x_position + stepX)) != 1)
  {
      x_position += stepX;
      y_position += stepY;
  }
}

void Player::getInput(Map &map)
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
      case 65  :  {                                                                                                   //forward
          moveForward(map);
          break;
          }
      case 66  :  {                                                                                                   //back
          moveBack(map);
          break;
        }
      case 67  :  point_of_view += 0.2; if (point_of_view >= 2 * M_PI) point_of_view -= 2 * M_PI; break;              //right
      case 68  :  point_of_view -= 0.2; if (point_of_view < 0)         point_of_view += 2 * M_PI;  break;             //left
    }
}

Player::Player()
{
  //установка стандартных значений
  x_position     = 12;
  y_position     = 12;
  point_of_view  = 0;
  field_of_view  = M_PI / 3;
  step           = 0.1;
}
