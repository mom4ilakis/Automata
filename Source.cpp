#include "Automata.h"
#include <iostream>
#include <string>
#include <time.h>
#include <chrono>

int main()
{
	std::string filename("TEST_WORDS_2.txt");

	Automata A;
	
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now(); 
	try 
	{
		A.learnFromFile(filename);
	}
	catch (std::invalid_argument& e) { ; }

	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

	auto durr= std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

	std::cout << durr << '\n';

	std::cout << "STARTED !\n";

	A.dictionary();
	
	std::string command;
	
	std::cin >> command;

	while (command != "EXIT")
	{



		if (command == "ADD")
		{
			std::cin >> command;
			
			
			std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
			
			A.learnWord(command);
			
			std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

			auto durr = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

			std::cout << durr << '\n';
			
		}
		if (command == "CHECK")
		{
			std::cin >> command;

			if (A.recWord(command))
			{
				std::cout << command << " I know that one !\n";
			}
			else
				std::cout << command << " I don't know that one! \n";

		}
		


		std::cin >> command;
	}

	std::cout << "Want to save dictionary ?\n";
	std::cin >> command;

	if(command == "YES")
		A.SaveDict();



	A.saveAsDOT();

	std::cout << A.numberKnownWords() <<'\n';

	return 0;
}