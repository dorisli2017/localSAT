/*
 * main.h
 *
 *  Created on: May 8, 2018
 *      Author: Guangping Li
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
using namespace std;

/*problem and assignment*/
char* fileName;
int numCs;
int numVs;
vector<int>* clauses;
struct V *variables;
int* numP;
vector<int> unsatCs;
double* probs;
bool* assign;
vector<int>* posC;
vector<int>* negC;
int* posOc;
int* negOc;
int maxOcc;
double* lookUpTable;
vector<int> clauseT;


/*settings*/
bool output_flag;
bool tabu_flag;
bool seed_flag;
/*option values*/

unsigned long long int maxTries = ULLONG_MAX;
unsigned long long int maxFlips = ULLONG_MAX;
int seed;
double cb=3.6;
double cm=0.5;
double w = 0.3;// The probability used in the WALKSAT algorithm
float eps= 1.0;
int fct= 2;


/*methods*/
void parseOptions(int argc, char *argv[]);
void printOptions();
void readFile(const char* fileName);
void memAllocate(string buff);
void parseLine(string line, int index);
void testLine(string line);
void initialAssignment();
void randomAssignment();
void setAssignment();
void printVariables();
void printClauses();
void printAssignment();
void printUnsatCs();
void printNumP();
int getFlipCandidate(int cIndex);
void (*initLookUpTable)(void) = NULL;
void initLookUpTable_exp(void);
void initLookUpTable_poly(void);
void flip(int j);
void debugProblem();
void debugAssign();
void test();
int computeMakeScore(int index);
int computeBreakScore(int index);
void search_prob();
int getFlipCandidate_wa(int cIndex);
int getFlipCandidate_max(int cIndex);
void printVector(vector<int>& vec);
void printUsage();

#endif /* MAIN_H_ */
