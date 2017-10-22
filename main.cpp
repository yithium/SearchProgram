/*
  Program to create a command line interface to use search class.
*/


#include "SearchClass.h" // for search interface
#include <iostream>      // for basic std::cout and std::cin
#include <string>        // for interfacing with searchClass


#include <ctime> // for benchmarking

//
#ifdef MEASURESPEED
  #define CLOCKDEFINE std::clock_t start; \
                      double duration = 0;

  #define SETTIME start = std::clock();
  #define GETDURATION duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
  #define PRINTDURATION(message) std::cout << message << ": "<< duration <<'\n';
#else
  #define CLOCKDEFINE
  #define SETTIME
  #define GETDURATION
  #define PRINTDURATION(message)
#endif

int main(int argc, char**argv)
{
  // initialize searchClass
  SearchClass<int> test;
  
  CLOCKDEFINE
  
  
  if(argc == 2)
  {
    SETTIME
    test.ExecuteCommand("Load " + std::string(argv[1]));
  }
  else
  {
    while(!test.HasData())
    {
      std::string fileName;
      std::cout << "Please specify a data file to load.\n";
      std::cin >> fileName;
      SETTIME
      test.ExecuteCommand("Load " + fileName);
    }
  }
  GETDURATION
  PRINTDURATION("Load Duration")
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
  PRINTDURATION("Execution duration: ")
  
  return 0;
}