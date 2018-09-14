/*
 * main.h
 *
 *  Created on: May 8, 2018
 *      Author: Guang 
 */

#ifndef MAIN_H_
#define MAIN_H_

/* includes*/

#include <iostream>
#include <climits>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string>
#include <fstream>
#include <string.h>
#include <vector>
#include <assert.h>
#include <math.h>
#include <vector>
#include<iostream>
#include <stdio.h>
#include <stdlib.h>
#include <random>
#include <functional>
using namespace std;

/*problem and assignment*/
long long int flipCount = 0;
long long int flipGCount = 0;
char* fileName;
int numCs;
int numVs;
vector<int>* clauses;
struct V *variables;
vector<int>* posC;
vector<int>* negC;
int* posOc;
int* negOc;
int maxOcc;
int seed;
int mrq = 10;
int fct = 1;
int ict = 1;
double cb;
double eps;
double lct = 0.5;
double noise = 1;
int maxL = 0;
double ratio;
vector<int> clauseT;
uniform_int_distribution<int> distribution(0,INT_MAX);
/*methods*/

void readFile(const char* fileName);
void memAllocate(string buff);
void parseLine(string line, int index);
void initialAssignment();
void printVariables();
void printClauses();
void debugProblem();
int getFlipCandidate_wa(int cIndex);
int getFlipCandidate_max(int cIndex);
void printVector(vector<int>& vec);
void printUsage();

template<class T>
class Process{
	int* numP;
	vector<int> unsatCs;
	double* probs;
	bool* assign;
	double* lookUpTable;
	int* tabuS;
	int maxLOcc;
	T generator;

public:
	Process();
	void optimal();
	void printOptions();
	double LookUpTable_exp(int literal);
	double LookUpTable_poly(int literal);
	double func_exp(int literal);
	double func_poly(int literal);
	double (Process::*lookUp)(int)  = NULL;
	void initLookUpTable_exp(void);
	void initLookUpTable_poly(void);
	void search_prob();
	int getFlipLiteral(int cIndex);
	void flip(int literal);
	void biasAssignment();
	void randomBiasAssignment();
	void randomAssignment();
	void setAssignment();
	void printAssignment();
	void printUnsatCs();
	void printNumP();
	void test();
	void testLine(string line);
	void debugAssign();
	int computeBreakScore(int literal);
	int (Process::*randINT)(void) = NULL;
	int randI();
	int randI2();
};
#endif /* MAIN_H_ */
