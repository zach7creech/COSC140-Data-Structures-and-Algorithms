/* Author: Zachery Creech
 * COSC140 Fall 2019
 * Lab7: code_processor.cpp
 * This program implements the member functions for the class Code_Processor.
 * 10/29/2019 */
#include "code_processor.hpp"
#include <fstream>
#include <string>
#include <set>
#include <map>

using namespace std;

//hash function
unsigned int djb_hash(const string &s)
{
	size_t i;
	unsigned int h;

	h = 5381;

	for(i = 0; i < s.size(); i++)
		h = (h << 5) + h + s[i];
	return h;
}

//creates a new prize and puts it in Prizes
bool Code_Processor::New_Prize(const string &id, const string &description, int points, int quantity)
{
	//error checking
	if(points <= 0)
		return false;
	if(quantity <= 0)
		return false;
	if(Prizes.find(id) != Prizes.end())
		return false;

	//dynamically allocates memory for the created prize and assigns the member variables
	Prize *prize = new Prize;
	prize->id = id;
	prize->description = description;
	prize->points = points;
	prize->quantity = quantity;
	Prizes[id] = prize;
	return true;
}

//creates a new user and puts it in Names
bool Code_Processor::New_User(const string &username, const string &realname, int starting_points)
{
	//error checking
	if(starting_points < 0)
		return false;
	if(Names.find(username) != Names.end())
		return false;

	//dynamically allocates memory for the created user and assigns the member variables
	User *user = new User;
	user->username = username;
	user->realname = realname;
	user->points = starting_points;
	Names[username] = user;
	return true;
}

//deletes the given user and frees the memory
bool Code_Processor::Delete_User(const string &username)
{
	//error checking
	if(Names.find(username) == Names.end())
		return false;
	
	User *user = Names.find(username)->second;
	
	//iterates through the given user's list of phone numbers and erases them from Phones one by one
	for(set <string>::const_iterator sp = user->phone_numbers.begin(); sp != user->phone_numbers.end(); sp++)
		Phones.erase(*sp);
	Names.erase(username);
	delete user;
	return true;
}

//adds a phone number to the user's list of numbers and to Phones
bool Code_Processor::Add_Phone(const string &username, const string &phone)
{
	//error checking
	if(Names.find(username) == Names.end())
		return false;
	if(Phones.find(phone) != Phones.end())
		return false;
	Names.find(username)->second->phone_numbers.insert(phone);
	Phones[phone] = Names.find(username)->second;
	return true;
}

//removes phone number from the user's list of numbers and from Phones
bool Code_Processor::Remove_Phone(const string &username, const string &phone)
{
	User *curUser = Names.find(username)->second;
	//error checking
	if(Names.find(username) == Names.end())
		return false;
	if(curUser->phone_numbers.find(phone) == curUser->phone_numbers.end())
		return false;
	if(Phones.find(phone)->second->username != username)
		return false;
	
	curUser->phone_numbers.erase(phone);
	Phones.erase(phone);
	return true;
}

//prints all phones registered to given user
string Code_Processor::Show_Phones(const string &username) const
{
	//error checking
	if(Names.find(username) == Names.end())
		return "BAD USER";
	User *user = Names.find(username)->second;
	if(user->phone_numbers.empty())
		return "";

	//iterates through all numbers in user's list of numbers and adds them to a string of all numbers for printing
	string allNumbers = "";
	for(set <string>::const_iterator sp = user->phone_numbers.begin(); sp != user->phone_numbers.end(); sp++)
		allNumbers += (*sp + '\n');
	return allNumbers;
}

//allows a user to enter a code with their username
int Code_Processor::Enter_Code(const string &username, const string &code)
{
	//error checking
	if(Codes.find(code) != Codes.end())
		return -1;
	if(Names.find(username) == Names.end())
		return -1;
	
	User *user = Names.find(username)->second;
	unsigned int hash;
	//computes the hash of the code
	hash = djb_hash(code);
	//checks for code validity and adds appropriate number of points to user account, marks code as used
	if(hash % 17 == 0)
	{
		Codes.insert(code);
		user->points += 10;
		return 10;
	}
	else if(hash % 13 == 0)
	{
		Codes.insert(code);
		user->points += 3;
		return 3;
	}
	return 0;
}

//allows a user to enter a code with their phone number
int Code_Processor::Text_Code(const string &phone, const string &code)
{
	//error checking
	if(Codes.find(code) != Codes.end())
		return -1;
	if(Phones.find(phone) == Phones.end())
		return -1;

	User *user = Phones.find(phone)->second;
	unsigned int hash;
	//computes the hash of the code
	hash = djb_hash(code);
	//checks for code validity and adds appropriate number of points to user account, marks code as used
	if(hash % 17 == 0)
	{
		Codes.insert(code);
		user->points += 10;
		return 10;
	}
	else if(hash % 13 == 0)
	{
		Codes.insert(code);
		user->points += 3;
		return 3;
	}
	return 0;
}

//used by system to mark codes as used from saved state
bool Code_Processor::Mark_Code_Used(const string &code)
{
	//error checking
	if(Codes.find(code) != Codes.end())
		return false;
	if(djb_hash(code) % 17 != 0 && djb_hash(code) % 13 != 0)
		return false;

	Codes.insert(code);
	return true;
}

//returns given user's points balance
int Code_Processor::Balance(const string &username) const
{
	//error checking
	if(Names.find(username) == Names.end())
		return -1;

	return Names.find(username)->second->points;
}

//allows a user to redeem a prize in exchange for their points
bool Code_Processor::Redeem_Prize(const string &username, const string &prize)
{
	//error checking
	if(Names.find(username) == Names.end())
		return false;
	if(Prizes.find(prize) == Prizes.end())
		return false;
	
	Prize *curPrize = Prizes.find(prize)->second;
	int *userPoints = &(Names.find(username)->second->points);
	int cost = curPrize->points;
	//user can't buy the prize if they don't have enough points
	if(cost > *userPoints)
		return false;
	//decrements user's account the cost of the prize
	*userPoints -= cost;
	//decrements quantity of the prize left
	(curPrize->quantity)--;
	//deletes the prize if quantity equals 0 after redemption
	if(curPrize->quantity == 0)
	{
		Prizes.erase(prize);
		delete curPrize;
	}
	return true;
}

//destructor for the class, frees all memory from user pointers and prize pointers
Code_Processor::~Code_Processor()
{
	//iterates through Names and deletes each user
	for(map <string, User*>::iterator  mp = Names.begin(); mp != Names.end(); mp++)
		delete mp->second;
	//iterates through Prizes and deletes each prize
	for(map <string, Prize*>::iterator mp = Prizes.begin(); mp != Prizes.end(); mp++)
		delete mp->second;
}

//saves the server state in a file by printing commands with relevant values
bool Code_Processor::Write(const string &filename) const
{
	ofstream fout;
	fout.open(filename.c_str());
	if(fout.fail())
		return false;

	//iterates through Prizes and prints the command PRIZE followed by the prize's id, cost, quantity, and description
	for(map <string, Prize*>::const_iterator mp = Prizes.begin(); mp != Prizes.end(); mp++)
		fout << "PRIZE     "  << mp->second->id << "     " << mp->second->points << "     " << mp->second->quantity << "     " << mp->second->description << '\n';
	//iterates through Names and prints the command ADD_USER followed by the user's username, points, and real name
	for(map <string, User*>::const_iterator mp = Names.begin(); mp != Names.end(); mp++)
	{	
		fout << "ADD_USER " << mp->second->username << "     " << mp->second->points << "     " << mp->second->realname << '\n';
		//after printing an ADD_USER, iterate through that user's registered phone list and print each of those
		//prints command ADD_PHONE followed by owner's username, then the phone number string *sp
		for(set <string>::const_iterator sp = mp->second->phone_numbers.begin(); sp != mp->second->phone_numbers.end(); sp++)
			fout << "ADD_PHONE " << mp->second->username << "     " << *sp << '\n';
	}
	//if there are codes to print, MARK_USED must be printed first
	if(!(Codes.empty()))
		fout << "MARK_USED ";
	//only 8 codes are allowed per line, numMarked keeps track of how many have been printed
	int numMarked = 0;
	//iterates through Codes and prints each code separated by a space
	for(set <string>::const_iterator sp = Codes.begin(); sp != Codes.end(); sp++)
	{	
		fout << *sp << " ";
		numMarked++;
		//every 8 codes, print MARK_USED on a new line and continue
		if(numMarked == 8)
		{
			fout <<"\nMARK_USED ";
			numMarked = 0;
		}
	}
	fout << '\n';
	fout.close();
	return true;
}
