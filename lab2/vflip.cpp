/*
 * Author: Zachery Creech
 * COSC140 Fall 2019
 * Lab2: vflip
 * This program reads in a PGM file, determines if it valid, and if valid flips the image along
 * the horizontal axis. The resultant PGM file is printed to standard output.
 * 09/08/19
 */

#include <iostream>
#include <cstdio>
#include <vector>
#include <string>

using namespace std;

int main()
{
	int column, row, is255, totPix, curPix;
	string P2;
	vector<int> pgm;
	cin >> P2 >> column >> row >> is255;
	//following 4 if statements check for various errors
	if(P2 != "P2")
	{
		fprintf(stderr, "Bad PGM file -- first word is not P2\n");
		return -1;
	}
	if(row <= 0)
	{
		fprintf(stderr, "Bad PGM file -- No row specification\n");
		return -1;
	}
	if(column <= 0)
	{
		fprintf(stderr, "Bad PGM file -- No column specification\n");
		return -1;
	}
	if(is255 != 255)
	{
		fprintf(stderr, "Bad PGM file -- No 255 following the rows and columns\n");
		return -1;
	}
	totPix = row * column;
	//for loop reads in each pixel following 255 header. If invalid pixel, print error message
	for(int i = 0; i < totPix; i++)
	{
		cin >> curPix;
		if(curPix > 255 || curPix < 0 || cin.fail())
		{
			fprintf(stderr, "Bad PGM file -- pixel %d is not a number between 0 and 255\n", i);
			return -1;
		}
		pgm.push_back(curPix);
	}
	//if cin continues after all pixels are read in, then there's extra stuff after the pixels
	if(cin >> curPix)
	{
		fprintf(stderr, "Bad PGM file -- Extra stuff after the pixels\n");
		return -1;
	}
	//standard PGM header
	printf("P2\n%d %d\n255\n", column, row);
	//controls which row to parse, starting with the bottom row
	for(int i = totPix - column; i >= 0; i -= column)
	{
		//prints current row from beginning to end to standard output
		for(int j = i; j < i + column; j++)
			printf("%d\n", pgm.at(j));
	}
	
	return 0;
}

