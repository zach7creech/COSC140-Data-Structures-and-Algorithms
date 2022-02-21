/*
 * Author: Zachery Creech
 * COSC140 Fall 2019
 * Lab4: hash_140.cpp
 * This .cpp file contains functions that set up, modify, and parse a hash table.
 * 09/27/19
 */

#include "hash_140.hpp"
#include <sstream>
#include <cstdio>

using namespace std;

int Last7(const string &key);
int XOR(const string &key);

string Hash_140::Set_Up(size_t table_size, const std::string &fxn, const std::string &collision)
{
	//error checking
	if(Keys.size() > 0)
		return "Hash table already set up";
	if(table_size <= 0)
		return "Bad table size";
	if(fxn != "Last7" && fxn != "XOR")
		return "Bad hash function";
	if(collision != "Linear" && collision != "Double")
		return "Bad collision resolution strategy";
	
	Keys.resize(table_size);
	Vals.resize(table_size);
	for(int i = 0; i < table_size; i++)
	{
		Keys[i] = "";
		Vals[i] = "";
	}
	Nkeys = 0;
	if(fxn == "Last7")
		Fxn = 0;
	else
		Fxn = 1;
	if(collision == "Linear")
		Coll = 0;
	else
		Coll = 1;
	Nprobes = 0;

	return "";
}

int XOR(const string &key)
{
	istringstream ss;
	ss.clear();
	int curInt;
	if(key.size() < 8)
	{
		ss.str(key);
		ss >> hex >> curInt;
		return curInt;
	}
	//finds first "word" of 7 characters
	string curWord = key.substr(0, 7);
	ss.clear();
	ss.str(curWord);
	ss >> hex >> curInt;
	string nextWord = "";
	int nextInt;
	//does the XOR operation with each word until reaching the end
	for(int i = 7; i < key.size(); i+=7)
	{
		ss.clear();
		nextWord = key.substr(i, 7);
		ss.str(nextWord);
		ss >> hex >> nextInt;
		curInt ^= nextInt;
	}
	return curInt;
}

int Last7(const string &key)
{
	string hashKey = "";
	string reverseKey = "";
	int hashHex;
	//starting from the end, build a new string of 7 characters
	for(int i = key.size() - 1; i >= 0; i--)
	{
		hashKey += key[i];
		if(i == key.size() - 7)
			break;
	}
	//reverse the order to get correct hex sequence
	for(int i = hashKey.size() - 1; i >= 0; i--)
		reverseKey += hashKey[i];
	istringstream ss;
	ss.clear();
	ss.str(reverseKey);
	ss >> hex >> hashHex;
	return hashHex;
}

string Hash_140::Add(const string &key, const string &val)
{
	//error checking
	if(Keys.size() == 0)
		return "Hash table not set up";
	if(key == "")
		return "Empty key";
	for(int i = 0; i < key.size(); i++)
		if(key[i] < 48 || (key[i] > 57 && key[i] < 65) || (key[i] > 70 && key[i] < 97) || key[i] > 102)
			return "Bad key (not all hex digits)";
	if(val == "")
		return "Empty val";
	if(Nkeys == Keys.size())
		return "Hash table full";

	int origColl = Coll;
	int hashKey;
	//gets first hash to try
	if(Fxn == 0)
		hashKey = Last7(key);
	else
		hashKey = XOR(key);

	int index;
	int tries = 0;
	//which while loop that triggers is determined by collision resolution strategy
	//tries keeps track of how many times the while loop has tried to find an empty index. In hindsight a for loop would probably be better
	while(Coll == 0)
	{
		if(tries > Keys.size())
			return "Cannot insert key";
		//finds index based on hashkey
		index = hashKey % Keys.size();
		//if linear collision resolution has reached the end of the hash table from starting index, also check starting from the beginning
		if(index == Keys.size())
			index = 0;
		//linear collision resolution just adds one to original hash and tries again
		if(Keys[index] != "")
		{
			if(Keys[index] == key)
				return "Key already in the table";
			hashKey++;
			tries++;
			continue;
		}
		Keys[index] = key;
		Vals[index] = val;
		Coll = -1;
	}
	while(Coll == 1)
	{
		if(tries > Keys.size())
			return "Cannot insert key";
		index = hashKey % Keys.size();
		if(Keys[index] != "")
		{
			tries++;
			if(Keys[index] == key)
				return "Key already in the table";
			//double collision resolution uses the "other" hash function
			if(Fxn == 0)
			{
				if(XOR(key) % Keys.size() == 0)
					hashKey++;
				else
					hashKey += XOR(key) % Keys.size();
				continue;
			}
			else
			{	
				if(Last7(key) % Keys.size() == 0)
					hashKey++;
				else
					hashKey += Last7(key) % Keys.size();
				continue;
			}
		}
		Keys[index] = key;
		Vals[index] = val;
		Coll = -1;
	}
	Coll = origColl;
	Nkeys++;
	return "";
}

string Hash_140::Find(const string &key)
{
	//error checking
	if(Keys.size() == 0)
		return "";
	for(int i = 0; i < key.size(); i++)
		if(key[i] < 48 || (key[i] > 57 && key[i] < 65) || (key[i] > 70 && key[i] < 97) || key[i] > 102)
			return "";
	
	int hashKey;
	if(Fxn == 0)
		hashKey = Last7(key);
	else
		hashKey = XOR(key);

	//Find() is basically the same as Add() in the way it searches for a given index, but it doesn't modify the hash table
	int index;
	int tries = 0;
	while(Coll == 0)
	{
		if(tries > Keys.size())
			return "";
		index = hashKey % Keys.size();
		if (Keys[index] == "")
			return "";
		if(index == Keys.size())
			index = 0;
		if(Keys[index] != key)
		{
			Nprobes++;
			hashKey++;
			tries++;
			continue;
		}
		return Vals[index];
	}
	while(Coll == 1)
	{
		if(tries > Keys.size())
			return "";
		index = hashKey % Keys.size();
		if(Keys[index] == "")
			return "";
		if(Keys[index] != key)
		{
			Nprobes++;
			tries++;
			if(Fxn == 0)
			{
				if(XOR(key) % Keys.size() == 0)
					hashKey++;
				else
					hashKey += XOR(key) % Keys.size();
				continue;
			}
			else
			{	
				if(Last7(key) % Keys.size() == 0)
					hashKey++;
				else	
					hashKey += Last7(key) % Keys.size();
				continue;
			}
		}
		return Vals[index];
	}
}

void Hash_140::Print() const
{
	if(Keys.size() == 0)
		return;
	//prints every non-empty hash table index
	for(int i = 0; i < Keys.size(); i++)
	{
		if(Keys[i] == "")
			continue;
		printf("%5d %s %s\n", i, Keys[i].c_str(), Vals[i].c_str());
	}
}

size_t Hash_140::Total_Probes()
{
	if(Keys.size() == 0)
		return 0;
	size_t totProbes = 0;
	//uses Find() to see how many "tries" it takes to find a given index for each key
	for(int i = 0; i < Keys.size(); i++)
	{
		if(Keys[i] == "")
			continue;
		Nprobes = 0;
		Find(Keys[i]);
		totProbes += Nprobes;
	}
	return totProbes;
}
