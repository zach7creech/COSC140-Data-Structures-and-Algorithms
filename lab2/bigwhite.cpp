/*
 * Author: Zachery Creech
 * COSC140 Fall 2019
 * Lab2: bigwhite
 * This program takes 2 inputs from command line arguments, the row and column dimensions of a
 * PGM file. If the inputs are valid, a PGM file with the given dimensions of white pixels is
 * printed to standard output.
 * 09/05/19
 */

#include <iostream>
#include <cstdio>
#include <string>
#include <sstream>

using namespace std;

int main(int argc, char **argv)
{
	//checks for correct number of command line arguments
	if(argc != 3)
	{
		fprintf(stderr, "usage: bigwhite rows cols\n");
		return -1;
	}
	int row, column, totPix;
	istringstream ss;
	ss.str(argv[1]);
	//checks if given value for row dimension is an integer and greater than 0
	if(!(ss >> row) || row <= 0)
	{
		fprintf(stderr, "usage: bigwhite rows cols\n");
		return -1;
	}
	ss.clear();
	ss.str(argv[2]);
	//checks if given value for column dimension is an integer and greater than 0
	if(!(ss >> column) || column <= 0)
	{
		fprintf(stderr, "usage: bigwhite rows cols\n");
		return -1;
	}
	totPix = row * column;
	//standard header for PGM file
	printf("P2\n%d %d\n255\n", column, row);
	//prints necessary number of white pixels to standard output
	for(int i = 0; i < totPix; i++)
		printf("255\n");

	return 0;
}
