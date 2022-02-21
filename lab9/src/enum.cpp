/* Author: Zachery Creech
 * COSC140 Fall 2019
 * Lab9: enum.cpp
 * This program implements the do_enumeration() function and uses it in main() to recursively enumerate
 * strings that contain a given number of 1s and 0s.
 * 11/14/2019 */

#include <iostream>
#include <string>
#include <sstream>

using namespace std;

/* Recursive function that enumerates the given string. Begins by putting the 1s as
 * far to the end of the string as possible. From there, the function returns to the
 * most recent instance of itself and tries to insert a 1 instead of a zero, then builds
 * the rest of the string with appropriate number of 1s and 0s. It repeats this until 
 * the 1s are as far to the beginning of the string as possible. */
void do_enumeration(string &s, int index, int n_ones)
{
	//if the current index equals the size of the string, the end has been reached and the string needs to be printed
	//before beginning the next enumeration
	if(index == (int) s.size())
	{
		cout << s << '\n';
		return;
	}
	//for filling the string with 0s, calls do_enumeration again on the next index to see what needs to be added next
	if(index < (int) s.size() - n_ones)
	{
		s[index] = '0';
		do_enumeration(s, index + 1, n_ones);
	}
	//if this part is reached, that means the all of the 0s for the first enumeration have been added
	if(n_ones != 0)
	{
		s[index] = '1';
		do_enumeration(s, index + 1, n_ones - 1);
	}
}

//reads in the length of the string and the number of ones from the command line; calls the first instance of do_enumeration()
int main(int argc, char** argv)
{
	if(argc != 3)
		return -1;
	istringstream ss;
	int length, n_ones;

	ss.clear();
	ss.str(argv[1]);
	ss >> length;
	ss.clear();
	ss.str(argv[2]);
	ss >> n_ones;
	ss.clear();
	
	//create a string of input length and fill it with '-'
	string s(length, '-');
	do_enumeration(s, 0, n_ones);
	return 0;
}
