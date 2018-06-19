/*
 * main.h
 *
 *  Created on: May 8, 2018
 *      Author: Guangping 
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
const vector<bool> setB= {false, false};
bool tabu_flag;
bool seed_flag;
/*option values*/

const vector<int> setI= {INT_MAX,0,2,1,100,0};
int maxFlips;
int seed;
int fct;
int ict;
int rct1;
int rct2;
const vector<double> setD = {3.6, 1.0,0.5};
double cb;
double eps;
double lct;


/*methods*/
void parseOptions(const vector<bool>& setB, const vector<int>& setI,const vector<double>& setD);
void printOptions();
void readFile(const char* fileName);
void memAllocate(string buff);
void parseLine(string line, int index);
void testLine(string line);
void initialAssignment();
void biasAssignment();
void randomBiasAssignment();
void randomAssignment();
void setAssignment();
void printVariables();
void printClauses();
void printAssignment();
void printUnsatCs();
void printNumP();
int getFlipLiteral(int cIndex);
double (*lookUp)(int) = NULL;
double LookUpTable_exp(int literal);
double LookUpTable_poly(int literal);
void initLookUpTable_exp(void);
void initLookUpTable_poly(void);
void flip(int literal);
void debugProblem();
void debugAssign();
void test();
int computeBreakScore(int literal);
void search_prob();
int getFlipCandidate_wa(int cIndex);
int getFlipCandidate_max(int cIndex);
void printVector(vector<int>& vec);
void printUsage();

#endif /* MAIN_H_ */
