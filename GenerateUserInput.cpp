#include <fstream>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <ctime>
#include <string>

#define MININPUT 1
#define RANGE 10000
int main(int argc, char**argv)
{
	size_t in_x = 5000,in_y = 10;
	size_t command_sel = 0;
	std::string outFile = "UserInputUnordered.txt";
	if(argc >= 3)
	{
		in_x = std::atoi(argv[1]);
		in_y = std::atoi(argv[2]);
	}
	if(argc >= 4)
	{
		command_sel = std::atoi(argv[3]);
	}
	if(argc >= 5 )
	{
		outFile = argv[4];
	}
	
	srand (time(NULL));
	std::string command[3] = {"searchSequence","searchUnordered","searchClosest"};
	std::ofstream file (outFile.c_str(), std::ios::binary);
	
	for(size_t x = 0; x < in_x ; ++x)
	{
		size_t y = rand()%in_y + MININPUT;
		file << command[command_sel] << ' ';
		for(size_t yIdx = 0; yIdx < y ; ++yIdx)
		{
			file << rand() % RANGE - RANGE/2 << ' ';
		}
		file << '\n';
	}
	file << "exit\n";
	
	file.close();
	
	return 0;
}