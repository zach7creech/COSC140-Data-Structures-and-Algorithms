/* Author: Zachery Creech
 * COSC140 Fall 2019
 * Lab 9: ss_solver.cpp
 * This program defines and implements the Shifter class and member functions. It also
 * contains main() that takes one grid as input on the command line, then reads in
 * associated shapes on standard input. It then uses the Shifter class member funcitons
 * to find a solution to the given puzzle, if one exists.
 * 11/14/2019 */

#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <sstream>

using namespace std;

class Shifter
{
	public:
		bool Read_Grid_And_Shapes(int argc, const char** argv);
		bool Apply_Shape(int index, int r, int c);
		bool Find_Solution(size_t index);
		void Print_Solution() const;
	protected:
		vector <string> Grid;
		vector < vector <string> > Shapes;
		deque <int> Solution_Rows;
		deque <int> Solution_Cols;
};

int main(int argc, const char** argv)
{
	//ss_solver must have at least two arguments, the filename and one row of a grid
	if(argc < 2)
		return false;
	
	Shifter Game;
	
	Game.Read_Grid_And_Shapes(argc, argv);

	//starts recursive calls to find a solution with shape (0)
	if(Game.Find_Solution(0))
	{	
		Game.Print_Solution();
		return 0;
	}
	else
		return -1;
}

//reads in the grid from the command line and reads in the associated shapes from standard input using getline()
bool Shifter::Read_Grid_And_Shapes(int argc, const char** argv)
{
	istringstream ss;
	string row;
	
	//loops through argv to get each row of the grid
	for(int i = 1; i < argc; i++)
	{
		ss.clear();
		ss.str(argv[i]);
		ss >> row;
		Grid.push_back(row);
	}

	vector<string> shapePieces;
	string shape;
	string piece = "";
	
	//read in each shape from standard input, stores all "rows" of shape in one string separted by spaces in shape variable
	while(getline(cin, shape))
	{
		//after getting a shape, search through the string to extract each "row" of the shape and push_back each piece
		for(size_t i = 0; i < shape.size(); i++)
		{
			//"rows" are delimited by ' ', so add every non-whitespace piece to piece variable
			if(shape[i] != ' ')
				piece += shape[i];
			if(shape[i] == ' ' || i == shape.size() - 1)
			{
				shapePieces.push_back(piece);
				piece = "";
			}
		}
		//push_back the whole shape
		Shapes.push_back(shapePieces);
		shapePieces.clear();
	}

	return true;
}

//"applies" the current shape to the grid and flips appropriate values from 0 to 1 or 1 to 0
bool Shifter::Apply_Shape(int index, int r, int c)
{
	vector<string> shape = Shapes[index];
	//error checking to see if current shape can fit at current grid index
	if(shape[0].size() + c > Grid[0].size() || shape.size() + r > Grid.size())
		return false;
	
	//these loop through each index of the current shape to flip the appropriate elements of the grid
	for(size_t row = 0; row < shape.size(); row++)
	{
		for(size_t col = 0; col < shape[0].size(); col++)
		{	
			//only time an element in the grid will flip is when the applied shape element has value 1
			if(shape[row][col] == '1')
			{
				if(Grid[row + r][col + c] == '1')
					Grid[row + r][col + c] = '0';
				else
					Grid[row + r][col + c] = '1';
			}
		}
	}

	return true;
}

/* Recursive function that tries every possible combination of shape application to find a solution for the given puzzle.
 * The function starts by applying the first shape in its first possible position, then the next shape in its first possible
 * position, and so on, until the last shape. On the last shape, the function tries all possible applications of that shape. If
 * no solution is found, the function returns to the previous shape and moves that previous shape to its next possible position,
 * then tries the final shape in all its possible positions again. If no solution is found again, the function returns to the
 * previous shape again, moves it to its next possible position again, and tries all positions of the final shape. This continues
 * for every shape, working back up to the first shape, until every combination is tried or a solution is found. */
bool Shifter::Find_Solution(size_t index)
{
	//if index equals the number of shapes in Shapes, all shapes have been applied and the solution must be tested for correctness
	if(index == Shapes.size())
		return true;
	//starting at 0,0 in the grid, this tries to apply the current shape (at Shapes[index]) to the grid
	for(size_t row = 0; row < Grid.size(); row++)
	{
		for(size_t col = 0; col < Grid[0].size(); col++)
		{	
			//if the shape can be applied, move on to more conditionals. If not, just try the next position 
			if(Apply_Shape(index, row, col))
			{			
				//if the current shape is NOT the last shape
				if(index != Shapes.size() - 1)
				{	
					//if the current shape does not yield a solution, reapply the shape to reset the grid before checking next position
					if(!Find_Solution(index + 1))
						Apply_Shape(index, row, col);
					else
					{	
						/* If Find_Solution() returns true, then a solution has been found and current shape position data must be stored.
						 * Because the function works backwards from the last shape, each successive addition to the solution vectors must
						 * go on the FRONT of the deque, which is why I chose not to use a vector for the solutions. */
						Solution_Rows.push_front(row);
						Solution_Cols.push_front(col);
						return true;
					}
				}
				else
				{
					//this else is ONLY executed after the last shape is applied - it checks for correctness
					bool solved = true;
					//these for loop traverse the entire grid to make sure every element is a '1', meaning the puzzle is solved
					for(size_t i = 0; i < Grid.size(); i++)
					{	
						for(size_t j = 0; j < Grid[0].size(); j++)
						{
							if(Grid[i][j] != '1') 
							{	
								//solved sentinel exists to break out of this loop early and decide if the final shape's position data needs to be stored
								solved = false;
								//undoes the final shape's application if the solution is incorrect
								Apply_Shape(index, row, col);
								break;
							}
						}
						if(!solved)
							break;
					}
					if(solved)
					{	
						Solution_Rows.push_front(row);
						Solution_Cols.push_front(col);
						return true;
					}
				}
			}	
		}
	}
	return false;
}

//prints the solution in correct shape application order; first prints the shape, then a space, then the row and column to apply it to in the grid
void Shifter::Print_Solution() const
{
	for(size_t shape = 0; shape < Shapes.size(); shape++)
	{
		for(size_t piece = 0; piece < Shapes[shape].size(); piece++)
			cout << Shapes[shape][piece] << ' ';
		cout << Solution_Rows[shape] << ' ' << Solution_Cols[shape] << '\n'; 
	}
}
