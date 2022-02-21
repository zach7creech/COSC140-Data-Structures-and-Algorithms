/* Author: Zachery Creech
 * COSC140 Fall 2019
 * Lab6: fraction.cpp 
 * This program implements the member functions of the class Fraction that is declared in fraction.hpp.
 * It also contains a function for reducing the Fraction. 
 * 10/21/19 */
#include "fraction.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <set>

using namespace std;

/* This finds numbers that appear in both the numerator and the denominator, then removes them.
 * Gets called every time anything is added to the numerator or denominator. */
void reduce(multiset <int> &numerator, multiset <int> &denominator)
{
	if(numerator.empty() || denominator.empty())
		return;
	
	/* Each number in the numerator is compared to each number in the denominator. If a
	 * duplicate is found, it is erased from each multiset. The iterators of both are then reset
	 * to the beginning of the multisets, and every number in the numerator is again compared to
	 * every number in the denominator. This repeats either until every number in the numerator is
	 * checked without finding a duplicate, or if either the numerator or denominator is emptied
	 * of all values other than the default 1. */
	bool reset = false;
	for(multiset <int>::iterator i = numerator.begin(); i != numerator.end(); i++)
	{	
		//if a duplicate was found, resets the iterator for the numerator and fixes the reset flag
		if(reset)	
		{	
			i = numerator.begin();
			reset = false;
		}
		for(multiset <int>::iterator j = denominator.begin(); j != denominator.end(); j++)
		{	
			//erases the duplicate, checks for emptiness, and starts the first loop over
			if(*i == *j)
			{
				numerator.erase(i);
				denominator.erase(j);
				if(numerator.empty() || denominator.empty())
					return;
				reset = true;
				break;
			}
		}
	}
}

//clears the fraction back to 1/1 = 1
void Fraction::Clear()
{
	numerator.clear();
	denominator.clear();
}

//multiplies the fraction by n, adds a number to the numerator
bool Fraction::Multiply_Number(int n)
{
	if(n <= 0)
		return false;
	//if user tries to multiply by 1, don't fail but also don't add a 1 to the numerator
	if(n == 1)
		return true;
	numerator.insert(n);
	reduce(numerator, denominator);
	return true;
}

//divides the fraction by n, adds a number to the denominator
bool Fraction::Divide_Number(int n)
{
	if(n <= 0)
		return false;
	//if user tries to divide by 1, don't fail but also don't add a 1 to the denominator
	if(n == 1)
		return true;
	denominator.insert(n);
	reduce(numerator, denominator);
	return true;
}

//multiplies the fraction by n!, adds every number between n and 2, inclusive, to the numerator
bool Fraction::Multiply_Factorial(int n)
{
	if(n <= 0)
		return false;
	for(; n >= 2; n--)
		numerator.insert(n);
	reduce(numerator, denominator);
	return true;
}

//divides the fraction by n!, adds every number between n and 2, inclusive, to the denominator
bool Fraction::Divide_Factorial(int n)
{
	if(n <= 0)
		return false;
	for(; n >= 2; n--)
		denominator.insert(n);
	reduce(numerator, denominator);
	return true;
}

/* multiplies the fraction by a binomial coefficient, adds every number between n and 2, inclusive, to the numerator,
 * adds every number between k and 2, inclusive, to the denominator, and every number between n minus k and 2, inclusive,
 * to the denominator. fraction * (n!)/((k!)(n - k)!) */
bool Fraction::Multiply_Binom(int n, int k)
{
	if(n <= 0 || k < 0 || n - k <= 0)
		return false;
	//each for loop represents a term in the formula
	//(n!)
	for(int i = n; i >= 2; i--)
		numerator.insert(i);
	//(k!)
	for(int i = k; i >= 2; i--)
		denominator.insert(i);
	//(n - k)!
	for(int i = n - k; i >= 2; i--)
		denominator.insert(i);
	reduce(numerator, denominator);
	return true;
}

/* divides the fraction by a binomial coefficient, adds every number between n and 2, inclusive, to the denominator,
 * adds every number between k and 2, inclusive, to the numerator, and every number between n minus k and 2, inclusive,
 * to the numerator. fraction / ((n!)/((k!)(n - k)!)) */
bool Fraction::Divide_Binom(int n, int k)
{
	if(n <= 0 || k < 0 || n - k <= 0)
		return false;
	//each for loop represents a term in the formula
	//(k!)
	for(int i = k; i >= 2; i--)
		numerator.insert(i);
	//(n - k)!
	for(int i = n - k; i >= 2; i--)
		numerator.insert(i);
	//(n!)
	for(int i = n; i >= 2; i--)
		denominator.insert(i);
	reduce(numerator, denominator);
	return true;
}

//switches the numerator and the denominator
void Fraction::Invert()
{
	multiset <int> newNum;
	newNum = denominator;
	denominator = numerator;
	numerator = newNum;
}

/* prints each number in the numerator separated by * then every number in the denominator separated by /
 * n1 * n2 * n3 * ... / d1 / d2 / d3 ... */
void Fraction::Print() const
{
	ostringstream ss;
	string integer;
	string print = "";
	//if the numerator contains anything other than 1, print that/those value(s) separated by *
	if(!(numerator.empty()))
	{
		for(multiset <int>::iterator i = numerator.begin(); i != numerator.end(); i++)
		{	
			//convert the integer in the numerator to a string to be added to the string variable print
			ss.clear();
			ss.str("");
			ss << *i;
			integer = ss.str();
			//adds the current number in the numerator and the next * and space
			print += (integer + " * ");
		}
		//removes the trailing * and space from the last number added from the numerator
		print = print.substr(0, print.size() - 3);
	}
	//when the numerator only contains 1 then print 1 without any *
	else
		print += "1";
	//if the denominator contains anything other than 1, print that/those value(s) separated by /
	if(!(denominator.empty()))
	{	
		//adds first / with spaces after numbers from the numerator
		print += " / ";
		for(multiset <int>::iterator i = denominator.begin(); i != denominator.end(); i++)
		{	
			//convert the integer in the denominator to a string to be added to the string variable print
			ss.clear();
			ss.str("");
			ss << *i;
			integer = ss.str();
			//adds the current number in the denominator and the next / and space
			print += (integer + " / ");
		}
		//removes the trailing / and space from the last number added from the denominator
		print = print.substr(0, print.size() - 3);
	}
	cout << print << '\n';
}

/* calculates the double representation of the fraction, or the product of all the numbers in the
 * numerator divided by the product of all the numbers in the denominator */
double Fraction::Calculate_Product() const
{
	double product = 1;
	//finds the product of every number in the numerator and stores it in product
	for(multiset <int>::iterator i = numerator.begin(); i != numerator.end(); i++)
		product *= *i;
	//divides the product of every number in the numerator with every number in the denominator
	for(multiset <int>::iterator i = denominator.begin(); i != denominator.end(); i++)
		product /= *i;
	return product;
}
