#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include "Cryptography.h"

#define BUFFERSIZE 10000

int main(int argc, char**argv)
{
	std::string key = "unlock";
	std::string inputFile  = argv[1];
	std::ifstream file (inputFile.c_str(), std::ios::binary);
	
	char buffer[BUFFERSIZE] = {0};
	
	std::string temp = buffer;
	std::istringstream strStream(buffer);
	std::cout << "file opened \n";
	
	
	if(argc == 2)
	{
		key = argv[2];
	}
	
	Cryptography crypto(key);
	
	std::string fileData;
	
	file.seekg (0, file.end);
	size_t length = file.tellg();
	file.seekg (0, file.beg);
	fileData.reserve(length);
	// read data into string using buffer of BUFFERSIZE 
	while(length > 0 && file.good())
	{
		char buffer[BUFFERSIZE] = {0};
		size_t readSize = length > BUFFERSIZE ? BUFFERSIZE:length;
		
		file.read(buffer, readSize);
		fileData.append(buffer,readSize);
		
		length -= readSize;
	}
	file.close();
	
	fileData = crypto.Encrypt(fileData);
	
	std::ofstream fileout (inputFile.insert(inputFile.size() - 4, "_encrypted").c_str(), std::ios::binary);
	fileout << fileData;
	fileout.close();
	return 0;
}