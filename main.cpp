/*
  Program to create a command line interface to use search class.
*/


#include "SearchClass.h" // for search interface
#include <iostream>      // for basic std::cout and std::cin
#include <string>        // for interfacing with searchClass





#ifdef MEASURESPEED

  #define CLOCKDEFINE std::clock_t start_program; \
                      double duration_outer = 0;

  #define SETTIME start_program = std::clock();
  #define GETDURATION duration_outer = ( std::clock() - start_program ) / (double) CLOCKS_PER_SEC;
	#define PRINTDURATIONPROG(message) std::cout << message << ": "<< duration_outer <<'\n';
#else
  #define CLOCKDEFINE
  #define SETTIME
  #define GETDURATION
  #define PRINTDURATIONPROG(message)
#endif


int main(int argc, char**argv)
{
  // initialize searchClass
  SearchClass<int> test;
  CLOCKDEFINE
  SETTIME
  if(argc == 2)
  {
    test.ExecuteCommand("Load " + std::string(argv[1]));
  }
  else
  {
    while(!test.HasData())
    {
      std::string fileName;
      std::cout << "Please specify a data file to load.\n";
      std::cin >> fileName;
      test.ExecuteCommand("Load " + fileName);
    }
  }
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
    {
      break;
    }
    test.ExecuteCommand(userInput);
    
  }
	GETDURATION
  PRINTDURATION("Total function execution")
	PRINTDURATIONPROG("Total Program execution")
  
  return 0;
}