/*
 * Author: Zachery Creech
 * COSC140 Fall 2019
 * Lab2: pgminfo
 * This program reads in a PGM file and determines if it has the correct format for a PGM file. If not,
 * it prints the appropriate error message to standard output.
 * 09/05/19
 */

#include <iostream>
#include <cstdio>
#include <string>

using namespace std;

int main()
{
	double column, row, is255, totPix, curPix, pixSum = 0, pixAve;
	string P2;
	cin >> P2 >> column >> row >> is255;
	//following 4 if statements error check for various errors
	if(P2 != "P2")
	{
		cerr << "Bad PGM file -- first word is not P2\n";	
		return -1;
	}
	if(row <= 0)
	{	
		cerr << "Bad PGM file -- No row specification\n";
		return -1;
	}
	if(column <= 0)
	{
		cerr << "Bad PGM file -- No column specification\n";
		return -1;
	}
	if(is255 != 255)
	{
		cerr << "Bad PGM file -- No 255 following the rows and columns\n";
		return -1;
	}
	totPix = column * row;
	//for loop reads in each pixel following 255 header. If invalid pixel, print error message. If not, add pixel value to pixSum
	for(int i = 0; i < totPix; i++)
	{
		cin >> curPix;
		if(curPix > 255 || curPix < 0 || cin.fail())
		{
			cerr << "Bad PGM file -- pixel " << i << " is not a number between 0 and 255\n";
			return -1;
		}
		pixSum += curPix;
	}
	//if cin continues after all pixels are read in, then there's extra stuff after the pixels
	if(cin >> curPix)
	{
		cerr << "Bad PGM file -- Extra stuff after the pixels\n";
		return -1;
	}
	pixAve = pixSum / totPix;
	printf("# Rows:    %8.0f\n# Columns: %8.0f\n# Pixels:  %8.0f\nAvg Pixel: %8.3f\n", row, column, totPix, pixAve);
	return 0;
}
