#include "player.h"

void Player::moveForward()
{
//  x_position += step * cos(point_of_view);
//  y_position += step * (-sin(point_of_view));
  double stepX = step * cos(point_of_view);
  double stepY = step * (-sin(point_of_view));

  //if (x_position + stepX)

}

void Player::getInput()
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
          x_position += step * cos(point_of_view);
          y_position += step * (-sin(point_of_view));
          //move_forward
          break;
          }
      case 66  :  {                                                                                                   //back
          x_position += (-step) * cos(point_of_view);
          y_position += (-step) * (-sin(point_of_view));
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
