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

typedef vector <int> IVec;

class Halving {
  public:
    int minSteps(vector <int> a);
};

void all_breaks(int len, int breaks, map <int, int> &m)
{
	map <int, int>::iterator mitr;
	mitr = m.find(len);
	if(mitr != m.end() && mitr->second <= breaks)
		return;
	if(mitr == m.end())
		m[len] = breaks;
	else
		mitr->second = breaks;
	if(len > 1)
	{
		int half1 = len / 2;
		int half2 = len - half1;
		all_breaks(half1, breaks + 1, m);
		all_breaks(half2, breaks + 1, m);
	}
}

int Halving::minSteps(vector <int> a)
{
  map <int, int> m;
  map <int, int> canbreak, minbreaks;
  map <int, int>::iterator mit;
  int minval;
  size_t i;
}
