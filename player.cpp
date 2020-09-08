#include "player.h"

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
          //move_back
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
