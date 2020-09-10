#pragma once

#include <array>
#include <fstream>
#include <iostream>
#include <sstream>

class Map
{
private:
  constexpr static int c_map_width  = 24;
  constexpr static int c_map_height = 24;

  std::array<std::array<int, c_map_height>, c_map_width> world_map;
public:
  Map();

  void clean();
  void readWorldMapFromFile();

  int get_width()          const { return c_map_width; }
  int get_height()         const { return c_map_height; }
  int& getMap(int i, int j)      {return world_map[i][j];}
};

