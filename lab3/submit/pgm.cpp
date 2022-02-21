/*
 * Author: Zachery Creech
 * COSC140 Fall 2019
 * Lab3: pgm.cpp
 * This .cpp file contains functions that manipulate a PGM file. Write() writes the vector Pixel
 * to an output file. Create() creates Pixel containing specified number of pixels of value pv.
 * Clockwise() rotates the input PGM file clockwise. Cclockwise() rotates the input PGM file
 * counterclockwise. Pad() adds a border of thickness w of pixels of value pv. Panel() creates
 * a collage of the input PGM file, repeated in a number of rows and columns r and c. Crop()
 * removes the specified pixels from the input PGM file.
 * 09/18/19
 */

#include "pgm.hpp"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

bool Pgm::Read(const std::string &file)
{
  ifstream fin;
  string s;
  size_t i, j, r, c, v;

  fin.open(file.c_str());
  if (fin.fail()) return false;
  
  if (!(fin >> s)) return false;
  if (s != "P2") return false;
  if (!(fin >> c) || c <= 0) return false;
  if (!(fin >> r) || r <= 0) return false;
  if (!(fin >> i) || i != 255) return false;

  Pixels.resize(r);
  for (i = 0; i < r; i++) {
    Pixels[i].clear();
    for (j = 0; j < c; j++) {
      if (!(fin >> v) || v > 255) return false;
      Pixels[i].push_back(v);
    }
  }
  if (fin >> s) return false;
  fin.close();
  return true;
}

bool Pgm::Write(const std::string &file) const
{
	if(Pixels.size() == 0 || Pixels[0].size() == 0)
		return false;

	ofstream fout;
	fout.open(file.c_str());
	fout << "P2\n" << Pixels[0].size() << " " << Pixels.size() << '\n' << "255\n";
	//lineSize keeps track of how many pixels have been added per line. Once it reaches 20, add a \n
	int lineSize = 0;
	for(int i = 0; i < Pixels.size(); i++)
	{
		for(int j = 0; j < Pixels[0].size(); j++)
		{		
			lineSize++;
			//first if statement triggers on last pixel
			if(i == Pixels.size() - 1 && j == Pixels[0].size() - 1)
			{
				fout << Pixels[i][j] << '\n';
				continue;
			}
			//triggers every 20 pixels
			if(lineSize % 20 == 0)
				fout << Pixels[i][j] << '\n';
			else
				fout << Pixels[i][j] << ' ';
		}
	}
	fout.close();
	
	return true;
}

bool Pgm::Create(size_t r, size_t c, size_t pv)
{
	
	if(pv > 255)
		return false;
	Pixels.clear();
	Pixels.resize(r);
	//adds each pixel to each row after resizing to appropriate size
	for(int i = 0; i < Pixels.size(); i++)
	{	
		for(int j = 0; j < c; j++)
			Pixels[i].push_back(pv);
	}
	
	return true;
}

bool Pgm::Clockwise()
{
	if(Pixels.size() == 0 || Pixels[0].size() == 0)
		return false;
	vector < vector <int> > newPixels;
	newPixels.resize(Pixels[0].size());
	//copies original Pixel vector to newPixel vector by column, starting at bottom of first column
	for(int i = 0; i < Pixels[0].size(); i++)
	{
		for(int j = Pixels.size() - 1; j >= 0; j--)
			newPixels[i].push_back(Pixels[j][i]);
	}
	Pixels = newPixels;

	return true;
}

bool Pgm::Cclockwise()
{
	if(Pixels.size() == 0 || Pixels[0].size() == 0)
		return false;
	
	vector < vector <int> > newPixels;
	newPixels.resize(Pixels[0].size());
	//copies original Pixel vector to newPixel vector by column, starting at top of last column
	for(int i = Pixels[0].size() - 1; i >= 0; i--)
	{
		for(int j = 0; j < Pixels.size(); j++)
			newPixels[Pixels[0].size() - 1 - i].push_back(Pixels[j][i]);
	}
	Pixels = newPixels;
	
	return true;
}

bool Pgm::Pad(size_t w, size_t pv)
{
	if(Pixels.size() == 0 || Pixels[0].size() == 0)
		return false;
	if(pv > 255)
		return false;
	
	int rows = Pixels.size();
	int cols = Pixels[0].size();
	//creates newPixel vector of size that includes padding with original picture in top left "corner" of newPixel
	Pixels.resize(Pixels.size() + (2 * w));
	for(int i = 0; i < Pixels.size(); i++)
		Pixels[i].resize(cols + (2 * w), pv);
	//"moves" original picture w pixels to the right and down to correct position within padding
	for(int i = rows; i >= 0; i--)
	{
		for(int j = cols; j >= 0; j--)
		{
			Pixels[i + w][j + w] = Pixels[i][j];
			//replaces pixel that was just "moved" with padding pixel
			Pixels[i][j] = pv;
		}
	}
	
	return true;
}

bool Pgm::Panel(size_t r, size_t c)
{
	if(Pixels.size() == 0 || Pixels[0].size() == 0)
		return false;
	if(r == 0 || c == 0)
		return false;

	vector < vector <int> > newPixels;
	int rows = Pixels.size();
	int cols = Pixels[0].size();
	int curRowInd = 0;
	int curColInd = 0;
	newPixels.resize(rows * r);
	for(int i = 0; i < newPixels.size(); i++)
		newPixels[i].resize(cols * c);
	for(int i = 0; i < newPixels.size(); i++)
	{
		//resets index of pixel to copy from Pixel to first row after one instance of the picture is finished being copied
		if(i % rows == 0)
			curRowInd = 0;
		//copies original Pixel vector into newPixel, each row at a time
		for(int j = 0; j < newPixels[0].size(); j++)
		{
			//resets index of pixel to copy from Pixel to first column after one instance of the picture is finished being copied
			if(j % cols == 0)
				curColInd = 0;
			newPixels[i][j] = Pixels[curRowInd][curColInd];
			curColInd++;
		}
		curRowInd++;
	}
	Pixels = newPixels;
	
	return true;
}

bool Pgm::Crop(size_t r, size_t c, size_t rows, size_t cols)
{
	if(r + rows > Pixels.size() || c + cols > Pixels[0].size())
		return false;
	if(Pixels.size() == 0 || Pixels[0].size() == 0)
		return false;
	
	vector < vector <int> > newPixels;
	newPixels.resize(rows);
	for(int i = 0; i < newPixels.size(); i++)
		newPixels[i].resize(cols);
	//these variables keep track of current pixel to be copied from Pixel to newPixel
	int curRowInd = 0;
	int curColInd = 0;
	//copies Pixel vector to newPixel within boundaries provided to Crop() function by row
	for(int i = r; i < rows + r; i++)
	{
		curColInd = 0;
		//iterator in these for loops represents position in newPixel, which is always a smaller vector than Pixel
		for(int j = c; j < cols + c; j++)
		{
			newPixels[curRowInd][curColInd] = Pixels[i][j];
			curColInd++;
		}
		curRowInd++;
	}
	Pixels = newPixels;

	return true;
}        
