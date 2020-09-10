#pragma once

#include <cstring>
#include <string>
#include <iostream> //delete

#include "player.h"
#include "raycasting.h"

class Output
{
private:
  char buffer[80][240];
public:
  void rendering(const Player &player,  const RayCasting &rct);
  void print(const RayCasting &rct);

  void printMap(const Player &player, Map &map); //delete
};
