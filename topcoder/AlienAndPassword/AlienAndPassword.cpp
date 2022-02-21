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
#include <cstring>
using namespace std;

class AlienAndPassword {
  public:
    int getNumber(string S);
};

int AlienAndPassword::getNumber(string S)
{
	char compare = S[0];
	int duplicate = 0;
	for(int i = 1; i < S.size(); i++)
	{
		if(S[i] == compare)
			duplicate++;
		else
			compare = S[i];
	}
	return S.size() - duplicate;
}
