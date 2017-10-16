#include "Cryptography.h"
#include "SearchClass.h"
#include <iostream>
#include <string>
#include <fstream>
int main(int argc, char**argv)
{
/*
	std::string key = "this";
	std::string message = "To be encrypted\nTo be decrypted\nTest Ended\n";
	Cryptography crypto(key);
	std::cout << message << std::endl;
	message = crypto.Encrypt(message);
	std::cout << message << std::endl;
	message = crypto.Decrypt(message);
	std::cout << message << std::endl;
*/

	SearchClass test;
	std::cout <<argv[1];
	bool result = test.Load(argv[1]);
	if(result)
	{
		std::cout << "successfully loaded" << std::endl;
	}
	else
	{
		std::cout << "failure to load" << std::endl;
	}
	
	std::vector<int> testVec;
	std::vector<int> testVec2;
	testVec.push_back(38);
	//testVec.push_back(37);
	testVec2.push_back(327);
	testVec2.push_back(327);
	testVec2.push_back(327);
	//testVec.push_back(20);
	//test.SearchUnordered(testVec);
	//test.SearchUnordered(testVec2);
	test.SearchClosest(testVec);
	std::string userInput = "";
	while(userInput != "exit")
	{
		std::getline(std::cin,);
		std::cin >> userInput;
		std::cout<<std::endl << "echoing: "<< userInput <<std::endl;
		ExecuteCommand(userInput);
	}
	
	return 0;
}