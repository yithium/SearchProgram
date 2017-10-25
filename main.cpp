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
  #define GET_DURATION duration_outer = ( std::clock() - start_program ) / (double) CLOCKS_PER_SEC;
  #define PRINT_DURATIONPROG(message) std::cout << message << ": "<< duration_outer <<'\n';
#else
  #define CLOCKDEFINE
  #define SETTIME
  #define GET_DURATION
  #define PRINT_DURATIONPROG(message)
#endif


int main(int argc, char**argv)
{
  // initialize searchClass
  SearchClass<int> test;
  CLOCKDEFINE
  SETTIME
  if(argc == 2)
  {
    test.Initialize("Load " + std::string(argv[1]));
  }
  else
  {
    while(!test.Good())
    {
      std::string fileName;
      std::cout << "Please specify a data file to load.\n";
      std::cin >> fileName;
      test.ExecuteCommand("Load " + fileName);
    }
  }
  std::string userInput = "";
  std::getline(std::cin, userInput);
  if(userInput.at(userInput.size()-1) == '\r')
  {
    userInput = userInput.substr(0,userInput.size()-1);
  }
  while(userInput != "exit")
  {
    test.ExecuteCommand(userInput);
    std::getline(std::cin, userInput);
    if(userInput.at(userInput.size()-1) == '\r')
    {
      userInput = userInput.substr(0,userInput.size()-1);
    }
  }
  GET_DURATION
  PRINT_DURATION("Total function execution")
  PRINT_DURATIONPROG("Total Program execution")
  
  return 0;
}