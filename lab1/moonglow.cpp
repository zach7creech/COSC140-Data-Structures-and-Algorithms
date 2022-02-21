/*
 * Author: Zachery Creech
 * COSC140 Fall 2019
 * Lab1: Moonglow
 * 09/02/19
 */
#include <iostream>

using namespace std;

int main()
{
	double curGrade, totalGrade = 0;
	string isString, name;
	
	while(!(cin.eof()))
	{
		/*
		 * The program tries to read in a number. If it fails, it tries to read the input as a string. If the program reads in a
		 * string that matches a command, it saves the name or computes the average.
		 */
		if(cin >> curGrade)
		{
			totalGrade += curGrade;
			continue;
		}
		else
			cin.clear();
		if(cin >> isString)
		{
			if(isString == "NAME")
			{
				if(cin >> name)
					continue;
				else
					cin.clear();
				continue;
			}
			/*
			 * If "AVERAGE" is encountered, the program tries to read in numbers. It finds the average of the detected numbers.
			 * If no numbers are detected, the program ignores the "AVERAGE" and tries to read the next input.
			 */
			else if(isString == "AVERAGE")
			{
				int count = 0;
				double average = 0;
				while(cin >> curGrade)
				{
					average += curGrade;
					count++;
				}
				if(count == 0)
				{
					cin.clear();
					continue;
				}
				cin.clear();
				totalGrade += (average / count);
			}
		}
	}
	cout << name << ' ' << totalGrade << '\n';
	return 0;
}
