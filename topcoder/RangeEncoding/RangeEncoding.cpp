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

class RangeEncoding {
  public:
    int minRanges(vector <int> arr);
};

int RangeEncoding::minRanges(vector <int> arr)
{
	int ranges = 1;
	int lastNum = arr[0];
	for(int i = 1; i < arr.size(); i++)
	{
		if(arr[i] == lastNum + 1)
		{	
			lastNum = arr[i];
			continue;
		}
		else
		{	
			ranges++;
			lastNum = arr[i];
		}
	}

  return ranges;
}
