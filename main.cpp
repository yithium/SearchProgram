
#include "SearchClass.h" // for search interface
#include <iostream>      // for basic std::cout and std::cin
#include <string>        // for interfacing with searchClass

int main(int argc, char**argv)
{
	// initialize searchClass
	SearchClass test;
	
	std::cout << "Loading from: " << argv[1] << std::endl;
	bool result = test.Load(argv[1]);
	
	// Print result of data file load
	if(result)
	{
		std::cout << "successfully loaded" << std::endl;
	}
	else
	{
		std::cout << "failure to load" << std::endl;
	}
	
	// for getting input from user
	std::string userInput = "";
	while(userInput != "exit")
	{
		// *** should operate on get line instead.
		//std::getline(std::cin,);
		std::cin >> userInput;
		// execute user command
		test.ExecuteCommand(userInput);
	}
	
	return 0;
}