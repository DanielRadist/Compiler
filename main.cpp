#include <iostream>
#include <fstream>
#include "Syntaxes/SyntaxAnalyser.h"
#include "Magazine.h"

int main()
{
	setlocale(LC_ALL, "rus");
	std::ofstream fout("output.txt");
	std::ifstream fin("test_files/test1.cpp");

	SyntaxAnalyser analyser(fin);
	try
	{
		analyser.run();
	}
	catch (std::exception& e)
	{
		std::cout << e.what();
		return 1;
	}
	std::cout << "Analysis success" << std::endl;
	
	return 0;
}
