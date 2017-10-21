
#include "SearchClass.h" // for search interface
#include <iostream>      // for basic std::cout and std::cin
#include <string>        // for interfacing with searchClass

int main(int argc, char**argv)
{
	// initialize searchClass
	SearchClass<int> test;
	
	//std::cout << "Loading from: " << argv[1] << std::endl;
	test.ExecuteCommand("searchSequence 1 42");
	test.ExecuteCommand("Load " + std::string(argv[1]));
	// Print result of data file load
	//if(result)
	//{
	//	std::cout << "successfully loaded" << std::endl;
	//}
	//else
	//{
	//	std::cout << "failure to load" << std::endl;
	//}
	
	// for getting input from user
	std::string userInput = "";
	while(userInput != "exit")
	{
		// *** should operate on get line instead.
		std::getline(std::cin, userInput);
		//std::cin >> userInput;
		if(userInput.at(userInput.size()-1) == '\r')
		{
			userInput = userInput.substr(0,userInput.size()-1);
		}
		
		
		// execute user command
		if(userInput == "exit")
			break;
		test.ExecuteCommand(userInput);
		
	}
	
	return 0;
}