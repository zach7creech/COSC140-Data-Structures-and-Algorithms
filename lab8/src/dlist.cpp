/* Author: Zachery Creech
 * COSC140 Fall 2019
 * Lab8: dlist.cpp
 * This program implements the methods of the Dlist class. The Dlist class
 * is a circular list with a sentinel node. This is how lists are
 * implemented in the STL.
 * 10/31/2019 */
#include "dlist.hpp"
#include <iostream>

using namespace std;

//the constructor; creates the sentinel node and initializes the list's size to 0
Dlist::Dlist()
{
	this->sentinel = new Dnode;
	sentinel->flink = sentinel;
	sentinel->blink = sentinel;
	this->size = 0;
}

//the copy constructor; just like the regular constructor except sets the new list equal to the list to be copied using the operator overload for '='
Dlist::Dlist(const Dlist &d)
{
	this->sentinel = new Dnode;
	sentinel->flink = sentinel;
	sentinel->blink = sentinel;
	this->size = 0;
	*this = d;
}

//the destructor; clears the list (deletes all non-sentinel nodes) then deletes the sentinel node
Dlist::~Dlist()
{
	this->Clear();
	delete this->sentinel;
}

/* operator overload for '='; clears the list receiving the assignment, the left hand side of the "x = y" (keyword this), then
 * copies each node from the right hand side of the "x = y" by calling Push_Back on the list receiving the assignment */
Dlist& Dlist::operator= (const Dlist &d)
{
	this->Clear();
	Dnode *newNode = new Dnode;
	for(newNode = d.Begin(); newNode != d.End(); newNode = newNode->flink)
		this->Push_Back(newNode->s);
	return *this;
}

//clears the list by deleting every node except the sentinel
void Dlist::Clear()
{
	//curNode is the one to be deleted, nextNode saves the next node to delete for the for loop to continue
	Dnode *curNode;
	Dnode *nextNode;
	for(curNode = this->Begin(); curNode != this->End(); curNode = nextNode)
	{
		//saving next node
		nextNode = curNode->flink;
		this->Erase(curNode);
	}
}

//function to check if the list is empty
bool Dlist::Empty() const
{
	if(this->size == 0)
		return true;
	return false;
}

//returns the size of the list
size_t Dlist::Size() const
{
	return this->size;
}

//calls Insert_Before() to insert the given string directly after the sentinel
void Dlist::Push_Front(const string &s)
{
	this->Insert_Before(s, this->sentinel->flink);	
}

//calls Insert_Before() to insert the given string directly before the sentinel
void Dlist::Push_Back(const string &s)
{
	this->Insert_Before(s, this->sentinel);
}

//returns the first value directly after the sentinel and deletes it from the list
string Dlist::Pop_Front()
{
	Dnode *cur = this->sentinel->flink;
	string val = cur->s;
	this->Erase(cur);
	return val;
}

//returns the first value directly before the sentinel and deletes it from the list
string Dlist::Pop_Back()
{
	Dnode *cur = this->sentinel->blink;
	string val = cur->s;
	this->Erase(cur);
	return val;
}

//returns the node directly after the sentinel
Dnode* Dlist::Begin() const
{
	return this->sentinel->flink;
}

//returns the sentinel node which signals the end of the list
Dnode* Dlist::End() const
{
	return this->sentinel;
}

//returns the node directly before the sentinel (last node of the list)
Dnode* Dlist::Rbegin() const
{
	return this->sentinel->blink;
}

//returns the sentinel node which signals the end of the list
Dnode* Dlist::Rend() const
{
	return this->sentinel;
}

//inserts the given string before the given node n
void Dlist::Insert_Before(const string &s, Dnode *n)
{
	//creates the new node
	Dnode *newNode = new Dnode;
	//saves the node before n
	Dnode *prev = n->blink;
	newNode->s = s;
	//create new links, starting from the node closest to the beginning of the list
	//prev's new flink is the newNode
	prev->flink = newNode;
	//newNode's blink is prev
	newNode->blink = prev;
	//newNode's flink is n
	newNode->flink = n;
	//n's new blink is newNode
	n->blink = newNode;
	//increment size by one
	(this->size)++;
}

//calls Insert_Before() to insert the given string AFTER the given node
void Dlist::Insert_After(const string &s, Dnode *n)
{
	//inserts before the node AFTER n, effectively adding the node after n
	this->Insert_Before(s, n->flink);
}

//erases the given node and relinks the list
void Dlist::Erase(Dnode *n)
{
	//saving previous and next node
	Dnode *prev = n->blink;
	Dnode *next = n->flink;
	//link prev forward to next, skipping node to be erased
	prev->flink = next;
	//link next back to prev, skipping node to be erased
	next->blink = prev;
	//decrements size by one and deletes n
	(this->size)--;
	delete n;
}
