/*
* Author: Zachery Creech
* CS140 Fall 2019
* Lab5: bitmatrix.cpp
* This .cpp file contains definitions for numerous functions that manipulate bitmatrices. 
* 10/07/19
*/

#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <iostream>
#include "bitmatrix.hpp"
using namespace std;

//constructor for Bitmatrix class when integers for rows and columns are provided, creates bitmatrix M
Bitmatrix::Bitmatrix(int rows, int cols)
{
	if(rows <= 0)
		throw((string)"Bad rows");
	if(cols <= 0)
		throw((string)"Bad cols");
	M.resize(rows);
	string column(cols, '0');

	for(int i = 0; i < rows; i++)
		M[i] = column;
}
           
//constructor for Bitmatrix class when a filename is provided, creates bitmatrix M
Bitmatrix::Bitmatrix(const string &fn)
{
	string curRow = "";
	string rowPiece;
	unsigned int cols = 0;
	ifstream fin;
	fin.open(fn.c_str());
	if(fin.fail())
		throw((string)"Can't open file");
	while(!(fin.eof()))
	{
		getline(fin, rowPiece);
		//if rowPiece contains at least one non-whitespace character then it's a row
		if(rowPiece != "")
		{	
			//parses the row and finds if all elements in the row are '1' or '0'
			for(unsigned int i = 0; i < rowPiece.size(); i++)
			{	
				if(rowPiece[i] == '0' || rowPiece[i] == '1')
					curRow += rowPiece[i];
				else if(rowPiece[i] != ' ')
					throw((string)"Bad file format");
			}
			//finds column size based on first row found
			if(M.size() == 0)
				cols = curRow.size();
			//if the current row isn't the right size, throw error
			if(curRow.size() != cols)
				throw((string)"Bad file format");
			if(curRow.size() != 0)
				M.push_back(curRow);
			curRow = "";
		}
	}
	fin.close();
}
        
Bitmatrix *Bitmatrix::Copy() const
{
	Bitmatrix *duplicate;
	if(M.size() != 0 && M[0].size() != 0)
		duplicate = new Bitmatrix(M.size(), M[0].size()); 
	duplicate->M = M;
	return duplicate;
}
                 
bool Bitmatrix::Write(const string &fn) const
{
	ofstream fout;
	fout.open(fn.c_str());
	if(fout.fail())
		return false;
	for(unsigned int i = 0; i < M.size(); i++)
		fout << M[i] << '\n';
	return true;
}
 
void Bitmatrix::Print(size_t w) const
{	
	if(w > M[0].size())
	{	
		for(unsigned int i = 0; i < M.size(); i++)
		{	
			cout << M[i] << '\n';
			if((i + 1) % w == 0 && i + 1 != M.size())
				cout << '\n';
		}
		return;
	}
	if(w == 0)
	{	
		for(unsigned int i = 0; i < M.size(); i++)
			cout << M[i] << '\n';
		return;
	}
	for(unsigned int i = 0; i < M.size(); i++)
	{
		for(unsigned int j = 0; j < M[i].size(); j++)
		{
			cout << M[i][j];
			if((j + 1) % w == 0 && j + 1 != M[i].size())
				cout << ' ';
		}
		cout << '\n';
		if((i + 1) % w == 0 && i + 1 != M.size())
			cout << '\n';
	}
}
                 
bool Bitmatrix::PGM(const string &fn, int p, int border) const
{
	if(p <= 0 || border < 0)
		return false;
	ofstream fout;
	fout.open(fn.c_str());
	if(fout.fail())
		return false;
	int newRow, newCol;

	//new dimensions of pgm file with border and bit p x p size included
	newRow = (M.size() * p) + ((M.size() + 1) * border);
	newCol = (M[0].size() * p) + ((M[0].size() + 1) * border);
	
	//fill in header of pgm file
	fout << "P2\n" << newCol << ' ' << newRow << '\n' << "255\n";
	char color; //w == 255; g == 100
	Bitmatrix pgm(newRow, newCol);
	
	//i and j keep track of where you are in the matrix
	for(unsigned int i = 0; i < M.size(); i++)
	{
		for(unsigned int j = 0; j < M[0].size(); j++)
		{
			//once index of matrix is established, decide the color to put in pgm
			if(M[i][j] == '0')
				color = 'w';
			else
				color = 'g';
			//calculates each intiial index in pgm for each p x p square, which always begins at [border, border]
			int rowInd = ((i + 1) * border) + (i * p);
			int colInd = ((j + 1) * border) + (j * p);
			//for looping back to first column of p x p square in next row
			int initRow = rowInd;
			int initCol = colInd;
			//fills pgm with one p x p square of color of matrix index
			for(; rowInd < initRow + p; rowInd++)
			{	
				for(; colInd < initCol + p; colInd++)
					pgm.M[rowInd][colInd] = color;
				colInd = initCol;
			}
		}
	}
	//prints appropriate color value in pgm file based on pgm bitmatrix
	for(unsigned int i = 0; i < pgm.M.size(); i++)
		for(unsigned int j = 0; j < pgm.M[0].size(); j++)
		{
			if(pgm.M[i][j] == '0')
				fout << "0\n";
			else if(pgm.M[i][j] == 'w')
				fout << "255\n";
			else if(pgm.M[i][j] == 'g')
				fout << "100\n";
		}
	fout.close();
	return true;
}

//returns row size
int Bitmatrix::Rows() const
{
	return M.size();
}

//returns column size
int Bitmatrix::Cols() const
{
	return M[0].size();
}

//returns the value in a bitmatrix at a given [i, j] index
char Bitmatrix::Val(int row, int col) const
{
	if(row > Rows() - 1 || col > Cols() - 1)
		return 'x';
	return M[row][col];
}
            
//sets the value in a matrix at a given [i, j] index to val
bool Bitmatrix::Set(int row, int col, char val)
{
	if(row < 0 || col < 0 || row >= (int)M.size() || col >= (int)M[0].size())
		return false;
	if(val == '0' || val == '1')
		M[row][col] = val;
	else
		return false;
	return true;
}
        
//swaps r1 and r2 in a bitmatrix
bool Bitmatrix::Swap_Rows(int r1, int r2)
{
	if(r1 >= (int)M.size() || r2 >= (int)M.size())
		return false;
	string row2;
	row2 = M[r2];
	M[r2] = M[r1];
	M[r1] = row2;
	return true;
}
 
//performs the operation row1 = row1 + row2 in a bitmatrix
bool Bitmatrix::R1_Plus_Equals_R2(int r1, int r2)
{
	if(r1 >= (int)M.size() || r2 >= (int)M.size())
			return false;
	for(unsigned int i = 0; i < M[r1].size(); i++)
	{
		//the only time the addition of two elements will result in '1' is when exclusively one of them == '1'
		if((M[r1][i] == '0' && M[r2][i] == '1') || (M[r1][i] == '1' && M[r2][i] == '0'))
			M[r1][i] = '1';
		else
			M[r1][i] = '0';
	}
	return true;
}
      

Bitmatrix *Sum(const Bitmatrix *a1, const Bitmatrix *a2)
{
	if(a1->Rows() != a2->Rows() || a1->Cols() != a2->Cols())
		return NULL;
	Bitmatrix *bm = new Bitmatrix(a1->Rows(), a1->Cols());
	for(int i = 0; i < a1->Rows(); i++)
	{
		for(int j = 0; j < a1->Cols(); j++)
			if((a1->Val(i, j) == '0' && a2->Val(i, j) == '1') || (a1->Val(i, j) == '1' && a2->Val(i, j) == '0'))
				bm->Set(i, j, '1');
	}
	return bm;
}

Bitmatrix *Product(const Bitmatrix *a1, const Bitmatrix *a2)
{
	if(a1->Cols() != a2->Rows())
		return NULL;
	Bitmatrix *bm = new Bitmatrix(a1->Rows(), a2->Cols());
	for(int row1 = 0; row1 < a1->Rows(); row1++)
	{
		for(int col2 = 0; col2 < a2->Cols(); col2++)
		{
			int product = 0;
			for(int col1 = 0; col1 < a1->Cols(); col1++)
				if(a1->Val(row1, col1) == '1' && a2->Val(col1, col2) == '1')
					product++;
			if(product % 2 == 1)
				bm->Set(row1, col2, '1');
			else
				bm->Set(row1, col2, '0');
		}

	}
	return bm;
}

Bitmatrix *Sub_Matrix(const Bitmatrix *a1, const vector <int> &rows)
{
	if(rows.size() == 0)
		return NULL;
	for(unsigned int i = 0; i < rows.size(); i++)
		if(rows[i] >= a1->Rows())
			return NULL;
	Bitmatrix *bm = new Bitmatrix(rows.size(), a1->Cols());
	for(unsigned int i = 0; i < rows.size(); i++)
		for(int j = 0; j < a1->Cols(); j++)
			bm->Set(i, j, a1->Val(rows[i], j));
	return bm;
}

//creates an identity matrix with given dimensions
Bitmatrix *Identity(int row, int col)
{
	Bitmatrix *bm = new Bitmatrix(row, col);
	for(int i = 0; i < row; i++)
		bm->Set(i, i, '1');
	return bm;
}

Bitmatrix *Inverse(const Bitmatrix *m)
{
	if(m->Rows() != m->Cols())
		return NULL;
	Bitmatrix *inverse;
	inverse = Identity(m->Rows(), m->Cols());
	Bitmatrix *original = m->Copy();

	for(int row = 0; row < original->Rows(); row++)
	{	
		//if a diagonal element does not equal '1', try to find a row below and in same column that == '1'
		if(original->Val(row, row) != '1')
		{
			for(int findRow = row + 1; findRow <= original->Rows(); findRow++)
			{	
				//this triggers if bottom row is reached and no '1' is found
				if(findRow == original->Rows())
					return NULL;
				//searching for a '1'
				if(original->Val(findRow, row) == '1')
				{	
					original->Swap_Rows(row, findRow);
					inverse->Swap_Rows(row, findRow);
					break;
				}
				if(findRow == original->Rows() - 1)
					return NULL;
			}
		}
		for(int findRow = row + 1; findRow < m->Rows(); findRow++)
			if(original->Val(findRow, row) == '1')
			{
				original->R1_Plus_Equals_R2(findRow, row);
				inverse->R1_Plus_Equals_R2(findRow, row);
			}
	}
	for(int row = original->Rows() - 1; row >= 0; row--)
	{
		for(int j = row + 1; j < original->Cols(); j++)
			if(original->Val(row, j) == '1')
			{
				original->R1_Plus_Equals_R2(row, j);
				inverse->R1_Plus_Equals_R2(row, j);
			}
	}

	return inverse;
}

//constructor sets up the hash table
BM_Hash::BM_Hash(int size)
{
	if(size <= 0)
		throw((string)"Bad size");
	Table.resize(size);
}

//calculates a djb_hash
unsigned int djb_hash(const string &key)
{
	unsigned int h = 5381;
	for(size_t i = 0; i < key.size(); i++)
		h = (h << 5) + h + key[i];
	return h;
}

//stores a key and bitmatrix pair
bool BM_Hash::Store(const string &key, Bitmatrix *bm)
{
	HTE entry;
	entry.key = key;
	entry.bm = bm;
	unsigned int hashInd = djb_hash(key) % Table.size();
	if(Table[hashInd].size() != 0)
		for(unsigned int i = 0; i < Table[hashInd].size(); i++)
			if(Table[hashInd][i].key == key)
				return false;
	Table[hashInd].push_back(entry);
	return true;
}
    
//recalls a stored bitmatrix from given key if it exists in the table
Bitmatrix *BM_Hash::Recall(const string &key) const
{
	unsigned int hashInd = djb_hash(key) % Table.size();
	for(unsigned int i = 0; i < Table[hashInd].size(); i++)
		if(Table[hashInd][i].key == key)
			return Table[hashInd][i].bm;
	return NULL;
}
      
//prints out all elements of the hash table
vector <HTE> BM_Hash::All() const
{
	vector<HTE> rv;
	for(unsigned int i = 0; i < Table.size(); i++)
		for(unsigned int j = 0; j < Table[i].size(); j++)
			rv.push_back(Table[i][j]);
	return rv;
}
