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

class Cryptography {
  public:
    long long encrypt(vector <int> numbers);
};

long long Cryptography::encrypt(vector <int> numbers)
{
	int compare = 1000, smallIndex;
	for(int i = 0; i < numbers.size(); i++)
	{	
		if(numbers.at(i) < compare)
		{
			compare = numbers.at(i);
			smallIndex = i;
		}	
	}
	numbers.at(smallIndex) = numbers.at(smallIndex) + 1;
	long long  product = numbers.at(0);
	for(int i = 1; i < numbers.size(); i++)
		product *= numbers.at(i);

	return product;
}
