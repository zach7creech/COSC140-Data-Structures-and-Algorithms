/*
 * Author: Zachery Creech
 * COSC140 Fall 2019
 * Lab2: neg
 * This program reads in a PGM file, determines if it is valid, and if valid calculates the pixel values for the
 * negative of the image. The resultant PGM file is printed to standard output.
 * to standard output.
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
	//following 4 if statements check for various errors, output appropriate error message, and end program
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
	//for loop reads in the PGM file, making sure each pixel is within appropriate intensity range
	for(int i = 0; i < totPix; i++)
	{
		cin >> curPix;
		if(curPix > 255 || curPix < 0 || cin.fail())
		{
			fprintf(stderr, "Bad PGM file -- pixel %d is not a number between 0 and 255\n", i);
			return -1;
		}
		//changes each pixel to "negative" value and adds to vector of ints pgm
		pgm.push_back(255 - curPix);
	}
	if(cin >> curPix)
	{
		fprintf(stderr, "Bad PGM file -- Extra stuff after the pixels\n");
		return -1;
	}
	printf("P2\n%d %d\n255\n", column, row);
	for(int i = 0; i < totPix; i++)
		printf("%d\n", pgm.at(i));
	
	return 0;
}

