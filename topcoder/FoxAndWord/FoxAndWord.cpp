#include <string>
#include <vector>
#include <list>
#include <cmath>
#include <algorithm>
#include <map>
#include <set>
#include <iostream>
#include <cstdio>
#include <cstdlib>
using namespace std;

class FoxAndWord {
  public:
    int howManyPairs(vector <string> words);
};

int FoxAndWord::howManyPairs(vector <string> words)
{
	int interesting = 0;
	for(int i = 0; i < words.size(); i++)
	{	
		string word = words.at(i);
		for(int j = 1; j < word.size(); j++)
		{
			string partA = word.substr(0, j);
			string partB = word.substr(j);
			string newWord = partB + partA;
			cerr << partA << '\n' << partB << '\n' << newWord << '\n';
			for(int k = 0; k < words.size(); k++)
				if(newWord == words.at(k) && words.at(k) != words.at(i))
				{	
					cerr << '\n' << newWord << '\n' << words.at(i) << '\n';
					interesting++;
					cerr << "Found one!\n";
				}
		}
	}
	return interesting / 2;
}
