/*
 * Author: Zachery Creech
 * COSC140 Fall 2019
 * Lab1: Checkerboard
 * 09/02/19
 */
#include <iostream>

using namespace std;

int main()
{
	int rows, columns, cycle, width;
	char startChar;
	string checkerboard = "";

	cin >> rows >> columns >> startChar >> cycle >> width;
	
	/*
	 * The following two if statements check for errors, and if necessary, output appropriate messages and exit the program
	 */
	if(cin.fail())
	{
		cerr << "usage: checkerboard  - stdin should contain R, C, SC, CS and W\n";
		return -1;
	}
	if(rows <= 0 || columns <= 0 || cycle <= 0 || width <= 0 || (startChar + cycle) > 127)
		return -1;

	/*
	 * Nested for loops build the checkerboard output in one string.
	 * Starting with the first row, it adds the starting character to the string curLine enough times to satisfy the width requirement.
	 * It then moves to the next column, calculates the next character, and adds it enough times to satisfy the width requirement.
	 * Once every column is filled with the correct character, it adds the newly built line (curLine) to the checkerboard string
	 * enough times to satisfy the width requirement (W*W), each separated by a new line.
	 * The program then moves to the next row, and repeats.
	 */
	char curChar = startChar;
	for(int curRow = 0; curRow < rows; curRow++)
	{
		string curLine = "";
		for(int curCol = 0; curCol < columns; curCol++)
		{
			for(int grid = 0; grid < width; grid++)
			{
				if(curRow == 0 && curCol == 0)
				curLine += curChar;
				else
				{
					curChar = startChar + ((curRow + curCol) % cycle);
					curLine += curChar;
				}
			}
		}
		curLine += '\n';
		for(int grid = 0; grid < width; grid++)
			checkerboard += curLine;
	}

	cout << checkerboard;

	return 0;
}
