#include <iostream>
#include <cmath>
#include <curses.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#include "player.h"
#include "map.h"
#include "raycasting.h"
#include "output.h"

using namespace std;

int main()
{
  Player player;
  Map map;
  Output output;
  RayCasting rct;
  map.readWorldMapFromFile();

  while (1)
    {
      system("clear");
      rct.rayCast(player, map);
      output.rendering(player, rct);
      output.print(rct);
      //output.printMap(player, map);
      player.getInput(map);
      map.clean();
    }
  return 0;
}

