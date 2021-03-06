/*
  To generate 2D matrix of a fix range
*/

#include <fstream>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <ctime>
#include <string>

#define RANGE 10000
#define FORMAT_SPACING 8

int main(int argc, char**argv)
{
  size_t in_x,in_y,width = FORMAT_SPACING;
  
  std::string outputName = "TestMatrix2D.txt";
  
  if(argc >= 3)
  {
    in_x = std::atoi(argv[1]); // rows
    in_y = std::atoi(argv[2]); // columns
  }
  if (argc >= 4)
  {
    outputName = argv[3]; // output name
  }
  srand (time(NULL));
  std::ofstream file (outputName.c_str(), std::ios::binary);
  if(file)
  {
    for(size_t x = 0; x < in_x ; ++x)
    {
      for(size_t y = 0; y < in_y ; ++y)
      {
        file << std::setw(width) << rand() % RANGE - RANGE/5;
      }
      file << '\n';
    }
    file.close();
  }
  
  return 0;
}