/* Author: Zachery Creech
 * COSC140 Fall 2019
 * Lab6: keno.cpp
 * This program calculates keno odds using the formula binom(80 - p, 20 - c) * binom(p, c) / binom(80, 20)
 * where p is the number of balls picked and c is the number of balls matched. It displays these odds 
 * based on user-inputted numbers read on standard input.
 * 10/21/19 */
#include "fraction.hpp"
#include <cstdio>
#include <iostream>
#include <vector>

using namespace std;

int main()
{
	double bet;
	cin >> bet;
	int numBalls;
	cin >> numBalls;
	
	int match;
	double payout;
	//a vector that holds pairings of catches and payouts in [0] and [1] of each vector
	vector < vector <double> > allPairs;
	//reads in all of the input from standard input, starting with the first number of matches
	while(cin >> match)
	{
		cin >> payout;
		vector <double> pair;
		//stores match and payout pairing in the vector
		pair.push_back(match);
		pair.push_back(payout);
		//puts the pairing in the allPairs vector
		allPairs.push_back(pair);
	}
	printf("Bet: %.2f\nBalls Picked: %d\n", bet, numBalls);
	double totExpecRet = 0;
	//does the calculating and printing for each pair
	for(unsigned int i = 0; i < allPairs.size(); i++)
	{
		double calculateProb, expecRet;
		//creation of a fraction to calculate the probability
		Fraction probability;
		//each member function call represents a term in the formula
		probability.Multiply_Binom(80 - numBalls, 20 - allPairs[i][0]);
		probability.Multiply_Binom(numBalls, allPairs[i][0]);
		probability.Divide_Binom(80,20);
		calculateProb = probability.Calculate_Product();
		//expected return = calculated probability multiplied by payout
		expecRet = calculateProb * allPairs[i][1];
		cout << "  Probability of catching " << allPairs[i][0] << " of " << numBalls << ": " << calculateProb << " -- Expected return: " << expecRet << '\n';
		totExpecRet += expecRet;
	}
	printf("Your return per bet: %.2f\nNormalized: %.2f\n", totExpecRet - bet, (totExpecRet - bet) / bet);
	return 0;
}
