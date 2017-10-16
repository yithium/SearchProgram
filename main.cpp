#include "Cryptography.h"
#include "SearchClass.h"
#include <iostream>
#include <string>
#include <fstream>
int main(int argc, char**argv)
{

	SearchClass test;
	std::cout << "Loading from: " << argv[1] << std::endl;
	bool result = test.Load(argv[1]);
	if(result)
	{
		std::cout << "successfully loaded" << std::endl;
	}
	else
	{
		std::cout << "failure to load" << std::endl;
	}
	
	std::string userInput = "";
	while(userInput != "exit")
	{
		//std::getline(std::cin,);
		std::cin >> userInput;
		test.ExecuteCommand(userInput);
	}
	
	return 0;
}