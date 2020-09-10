#include "map.h"

void Map::clean()
{
  // переделать
  for(int i = 0; i < c_map_height; ++i)
    {
      for(int j = 0; j < c_map_width; ++j)
        {
          if (world_map[i][j] == 8)
            world_map[i][j] = 0;
        }
    }
}

void Map::readWorldMapFromFile()
{
  std::string inform;
  std::ifstream file_with_maps("/home/vitaly/QtProject/RayCastingShooter/maps.txt", std::ios::in);
  int j = 0;
  int i = 0;

  auto convert2Int = [](char a) {
    std::stringstream s;
    int int_number;
    s << a;
    s >> int_number;
    return int_number;
  };

  if (!file_with_maps.is_open())
    {
      std::cout << "map dont open!\n";
      std::exit(1);
    }

  while(file_with_maps)
    {
      std::getline(file_with_maps, inform);
      for(const auto& k : inform)
        {
          if (k == '0' || k == '1')
            {
              world_map[i][j++] = convert2Int(k);

              if(j == 24)
                {
                  ++i;
                  j = 0;
                }
            }
        }
    }
}
