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

class FauxPalindromes {
  public:
    string classifyIt(string word);
};

bool isPalindrome(string s)
{
	for(int i = 0; i < s.size() / 2; i++)
	{
		if(s[i] != s[s.size()-i-1])
			return false;
	}
	return true;
}
bool isFaux(string s)
{
	string edit = "";
	edit += s[0];
	int curChar = 0;
	for(int i = 1; i < s.size(); i++)
	{
		if(s[i] == edit[curChar])
			continue;
		edit += s[i];
		curChar++;
	}
	return isPalindrome(edit);
}
string FauxPalindromes::classifyIt(string w)
{
	if(isPalindrome(w))
		return "PALINDROME";
	else if(isFaux(w))
		return "FAUX";

	return "NOT EVEN FAUX";
}
