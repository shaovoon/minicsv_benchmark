// CSVGenerator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include <ctime> 

using namespace std;

int main()
{
	const int TOTAL_LINES = 20000;
	const int TOTAL_COL = 20;
	const int TOTAL_COL_LENGTH = 12;
	srand(time(0));

	std::string text = "";
	std::ofstream myfile;
	myfile.open("d:\\example.csv");
	for (int line_no = 0; line_no < TOTAL_LINES; ++line_no)
	{
		for (int col_no = 0; col_no < TOTAL_COL; ++col_no)
		{
			text = "";
			for (int a = 0; a < TOTAL_COL_LENGTH; ++a)
			{
				text += 'a' + rand() % 26;
			}
			myfile << text.c_str();
			if(col_no < TOTAL_COL-1)
				myfile << ",";
		}
		myfile << "\n";
	}
	myfile.flush();
	myfile.close();

    std::cout << "Done!\n"; 

	return 0;
}

