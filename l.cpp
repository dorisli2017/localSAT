/*
 * main.cpp
 *
 *  Created on: May 8, 2018
 *      Author: ping
 */

#include "l.h"

int main(int argc, char *argv[]){
	fileName = argv[1];
	parseOptions(setB, setI,setD);
	readFile(fileName);
	switch(ict){
	case 0:randomAssignment();break;
	case 1:biasAssignment();break;
	default: randomBiasAssignment();
	}
	initLookUpTable();
	int size;
	//debugProblem();
	while(true){
		for(unsigned int j = 0; j < maxFlips; j++){
			size =  unsatCs.size();
			if (size == 0){
				//debugAssign();
				test();
				cout<< "s SATISFIABLE"<< endl;
				//printAssignment();
				return 0;
			}
			search_prob();
		}
		switch(rct){
		case 0:randomAssignment();break;
		case 1:biasAssignment();break;
		default: randomBiasAssignment();
		}
	}
	//test();
	debugAssign();
    return 0;
}
void debugProblem(){
	printOptions();
	printVariables();
	printClauses();
	cout<< "Occurences:"<< endl;
	for(int i = 1; i < numVs; i++){
		cout<< i<< ":"<<posOc[i]<< " "<<negOc[i]<< endl;
	}
}
void debugAssign(){
	/* Testing code**********************************/
	   	printAssignment();
	   	printUnsatCs();
	   	printNumP();


	/*Testing code**********************************/

}
/*parse the argument (including options and filename)
 *using getopt_long to allow GNU-style long options as well as single-character options
 */
void parseOptions(const vector<bool>& setB, const vector<int>& setI,const vector<double>& setD){
	tabu_flag = setB[0];
	seed_flag = setB[1];

	maxFlips =setI[0];
	seed = setI[1];
	fct= setI[2];
	ict = setI[3];
	rct = setI[4];

	cb=setD[0];
	eps= setD[1];
//set the parameters
	// set seed
	if(seed_flag)srand(seed);
	else srand (0);
	//else srand (time(NULL));
	switch (fct){
	case 0:initLookUpTable = initLookUpTable_poly;break;
	default:initLookUpTable = initLookUpTable_exp;break;
	}
}
// construct the Problem with fill information of the input file
void readFile(const char* fileName){
	//cout<< "in readFile"<<endl;
	ifstream fp;
	//fp.open("Debug/instance.cnf",std::ios::in);
	fp.open(fileName,std::ios::in);
	if(!fp.is_open()){
		perror("read file fails");
		exit(EXIT_FAILURE);
	}
	string buff;
	char head;
   	getline(fp,buff);
   	// Get the p line
   	while(!fp.eof()){
		//cout<<buff<<endl;
		//todo:parseLine
   		if(buff.empty()) break;
		head =buff.at(0);
		if(head == 'p'){
			memAllocate(buff);
			break;
		}
	  getline(fp,buff);
	}
   	// Get the clauses
   	int index = -1;
   	int line = 0;
   	while(!fp.eof() && line < numCs){
   		index++;
		getline(fp,buff);
		if(buff.empty()) break;
		parseLine(buff, index);
		line++;
   	}
   	initialAssignment();
};
void memAllocate(string buff){
	parseLine(buff,-1);
	clauses = new vector<int>[numCs];
	posC= new vector<int>[numVs];
	negC= new vector<int>[numVs];
	numP = (int*) malloc(sizeof(int) * numCs);
	probs = (double*)malloc(sizeof(double) * numVs);
	assign = (bool*)malloc(sizeof(bool) * numVs);
	posOc = (int*) malloc(sizeof(int) * numVs);
	negOc = (int*) malloc(sizeof(int) * numVs);
	for(int i = 0; i < numVs; i++){
		posOc[i] = 0;
	}
	for(int i = 0; i < numVs; i++){
		negOc[i] = 0;
	}
	clauseT.reserve(numVs);
}
void parseLine(string line,int indexC){
	char* str = strdup(line.c_str());
    const char s[2] = " ";
    if( indexC == -1){
    	strtok(str, s);
		strtok(NULL, s);
		numVs = atoi(strtok(NULL, s))+1;
		numCs = atoi(strtok(NULL, s));
		return;
    }// for the p line
    int lit;
    int size;
    char* token = strtok(str, s);
    while(token != NULL){
		if(*token== '-'){
			lit = atoi(token);
			clauseT.push_back(lit);
		    negOc[-lit]++;
			token = strtok(NULL, s);
			size++;
			continue;
		}
		if(*token == '0'){
			clauses[indexC] = clauseT;
			clauseT.clear();
		    return;
		}
		lit = atoi(token);
		clauseT.push_back(lit);
	    posOc[lit]++;
	    size++;
		token = strtok(NULL, s);
    }
	perror("a clause line does not terminates");
	exit(EXIT_FAILURE);
}
void printOptions(){
	printf("localSAT options: \n");
	cout<<"c tabu_flag: "<<tabu_flag<<endl;
	cout<<"c seed_flag: "<<seed_flag<<endl;
	cout<<"c maxFlips: "<<maxFlips<<endl;
	cout<<"c seed: "<<seed<<endl;
	cout<<"c fct: "<<fct<<endl;
	cout<<"c ict: "<<ict<<endl;
	cout<<"c rct: "<<rct<<endl;
	cout<<"c cb: "<<cb<<endl;
	cout<<"c eps: "<<eps<<endl;
	switch(fct){
	case 0:{
		cout<<"c polynomial function"<<endl;
		cout<<"c eps: "<<eps<<endl;
		cout<<"c cb: "<<cb<<endl;
		cout<< "pow((eps+break),-cb)" << endl;
		break;
		  }
	default:{
		cout<<"c exponential function"<<endl;
		cout<<"c cb: "<<cb<<endl;
		cout<< "pow(cb,-break)"<< endl;
		break;
		   }
	}
	cout<<"c seed: "<<seed<<endl;
}
void printVariables(){
	cout<< "Variables "<< ": " <<endl ;
   	for(int i = 1; i < numVs; i++){
   		cout<< "Variable "<<i << ": " <<endl ;
   		printVector(posC[i]);
   		printVector(negC[i]);
   	}
}
void printClauses(){
	cout<< "Clauses "<< ": " << endl ;
   	for(int i = 0; i < numCs; i++){
   		cout<< "Clause "<< i<< ": " ;
   		printVector(clauses[i]);
   	}
}
void printAssignment(){
	cout<< "v ";
	for(int i = 1; i < numVs; i++){
		if(assign[i]) cout <<i<<" ";
		else cout << -i<<" ";
	}
	cout <<endl ;
}
void printUnsatCs(){
	cout<< "Unsatisfied clauses ";
	printVector(unsatCs);
	cout <<endl ;
}
void printNumP(){
	cout<< "numP: ";
	for(int i = 0; i < numCs; i++){
		cout << numP[i]<< " ";
	}
	cout<<endl;
}




void initialAssignment(){
	for(int i = 0; i < numVs; i++){
			if(posOc[i] > negOc[i]){
				if(posOc[i]> maxOcc) maxOcc = posOc[i];
			}
			else{
				if(negOc[i]> maxOcc) maxOcc = negOc[i];
			}
			posC[i].reserve(posOc[i]);
			negC[i].reserve(negOc[i]);
		}
	for(int j = 0; j < numCs; j++){
		for (std::vector<int>::const_iterator i = clauses[j].begin(); i != clauses[j].end(); ++i){
			if(*i < 0) negC[-(*i)].push_back(j);
			else  posC[(*i)].push_back(j);
		}
	}
}

void biasAssignment(){
	for(int i = 0; i < numVs; i++){
			if(posOc[i] > negOc[i]){
				assign[i] = true;
			}
			else{
				assign[i] = false;
			}
	}
	setAssignment();
}

void randomBiasAssignment(){
	int sum;
	for(int i = 0; i < numVs; i++){
		sum = posOc[i] +negOc[i];
		if(sum == 0){
			assign[i] = true;
		}
		else{
			assign[i] = (rand()%sum)<posOc[i];
		}
	}
	setAssignment();
}
void randomAssignment(){
   	for(int j = 0; j < numVs; j++){
   		assign[j] = (rand()%2 ==1);
   	}
    setAssignment();
}


void setAssignment(){
   	for(int i = 0; i < numCs; i++){
   		numP[i] = 0;
   	}
   	for(int j = 0; j < numVs; j++){
		if(assign[j] == false){
	   		for (std::vector<int>::const_iterator i = negC[j].begin(); i != negC[j].end(); ++i){
	   			numP[*i]++;
	   		}
		}
		else{
			for (std::vector<int>::const_iterator i = posC[j].begin(); i != posC[j].end(); ++i){
	   			numP[*i]++;
			}
   		}
   	}
   	for(int i = 0; i < numCs; i++){
   		if(numP[i] == 0){
   			unsatCs.push_back(i);
   		}
   	}
}
int getFlipCandidate(int cIndex){
	vector<int>&  vList = clauses[cIndex];
	int j=0,bre;
	double sum=0,randD;
	for (std::vector<int>::const_iterator i = vList.begin(); i != vList.end(); ++i){
		bre = computeBreakScore(*i);
		//if(bre == 0) return *i;
		sum+= lookUpTable[bre];
		probs[j]= sum;
		j++;
	}
	randD = ((double)rand()/RAND_MAX)*sum;
	assert(randD >= 0);
	for(int i = 0; i < j;i++){
		if(probs[i]> randD){
			return vList[i];
		}
	}
	assert(false);
	return 0;
}
void flip(int j){
	std::vector<int>::const_iterator i;
	if(j > 0){
   		for (i = negC[j].begin(); i != negC[j].end(); ++i){
   			numP[*i]--;
   			if(numP[*i] == 0) unsatCs.push_back(*i);
   		}
		for (i = posC[j].begin(); i != posC[j].end(); ++i){
   			numP[*i]++;
		}

		assign[j] = true;
	}
	else{
   		for (i = negC[-j].begin(); i != negC[-j].end(); ++i){
   			numP[*i]++;
   		}
		for (i = posC[-j].begin(); i != posC[-j].end(); ++i){
   			numP[*i]--;
   			if(numP[*i] == 0) unsatCs.push_back(*i);
		}
		assign[-j]= false;
	}
}
void test(){
	ifstream fp;
	fp.open(fileName,std::ios::in);
	if(!fp.is_open()){
		perror("read file fails");
		exit(EXIT_FAILURE);
	}
	string buff;
	char head;
   	getline(fp,buff);
   	while(!fp.eof()){
   		if(buff.empty()) break;
		head =buff.at(0);
		if(head == 'p'){
			break;
		}
	  getline(fp,buff);
	}
   	while(!fp.eof()){
		getline(fp,buff);
		if(buff.empty()) break;
		testLine(buff);
   	}
   	cout<< "tested" << endl;
}

void testLine(string line){
	char* str = strdup(line.c_str());
    const char s[2] = " ";
    int lit;
    int numT=0;
    char* token = strtok(str, s);
    while(token != NULL){
		if(*token== '-'){
			lit = atoi(token);
			if(assign[-lit] == false) numT++;
			token = strtok(NULL, s);
			continue;
		}
		if(*token == '0'){
			if(numT == 0){
				perror("TEST FAILURE");
				exit(EXIT_FAILURE);
			}
		    return;
		}
		lit = atoi(token);
		if(assign[lit] == true) numT++;
		token = strtok(NULL, s);
    }
	perror("a clause line does not terminates");
	exit(EXIT_FAILURE);

}
int computeBreakScore(int index){
    int score = 0;
    int aIndex = abs(index);
    vector<int>& occList =(index < 0)? posC[aIndex] :negC[aIndex];
    for(std::vector<int>::const_iterator i = occList.begin(); i != occList.end(); ++i) {
        if (numP[*i]== 1) {
            score++;
        }
    }
	//cout<< "out make "<<endl;
    return score;
}

double func_exp(int index){
	return pow(cb,-computeBreakScore(index));
}
double func_poly(int index){
	return pow((eps+computeBreakScore(index)),-cb);
}

void search_prob(){
	int randC = rand()%unsatCs.size();
	int flipCindex = unsatCs[randC];
	if(numP[flipCindex] > 0){
		unsatCs[randC]=unsatCs.back();
		unsatCs.pop_back();
		return;
	}
	int flipVindex = getFlipCandidate(flipCindex);
	unsatCs[randC]=unsatCs.back();
	unsatCs.pop_back();
	flip(flipVindex);
}


void printVector(vector<int>& vec){
	for (std::vector<int>::const_iterator i = vec.begin(); i != vec.end(); ++i){
		cout << *i << ' ';
	}
	cout << endl;
}

/*print the manual of the program*/
void printUsage(){
	printf("\n");
	printf("\n");
	printf("\n");
	printf("\n--------localSAT, a stochastic local search solving SAT problem-----------------\n");
	printf("\nautor: Guangping Li\n");
	printf("references: Engineering a lightweight and efficient local search sat solver \n");

	printf("\n-------------------Usage--------------------------------------------------------\n");
	printf("\n");
	printf("./locolSAT  <DIMACS CNF instance> [options]\n");
	printf("\n--------------------------------------------------------------------------------\n");

	printf("\n");
	printf("\n-------------------Options------------------------------------------------------\n");
	printf("\n");
	printf("**for the potential flipping function:\n");
	printf("   --function, -f: 0 =  polynomial; 1 = exponential with make; 2 = exponential only with break: 3 = equal  [default = 0]\n");
	printf("   --eps, -e <double value> [0.0,1.0] :  [default = 1.0]\n");
	printf("   --cb, -b <double value>  [2.0,10.0] : constant for break\n");
	printf("   --cm, -m <double value>  [-2.0,2.0] : constant for make\n");
	printf("   --w, -w <double value>  [0.0,1.0] : [default = 0.3] the probability used only in the WALKSAT\n");
	printf("\n");
	printf("**for the process:\n");
	printf("   --caching, -c  : 0 =  no caching ; 1 = caching  for scores\n");
	printf("   --rounds, -r <long long int_value>,   : maximum number of tries \n");
	printf("   --flips, -p  <long long int_value>,   : maximum number of flips per try \n");
	printf("   --seed, -s : seed used for Randomness\n");
	printf("\n");
	printf("**further options:\n");
	printf("   --output, -o : output the final assignment\n");
	printf("   --help, -h : output this help\n");
	printf("\n");
	printf("---------------------------------------------------------------------------------\n");
}


void initLookUpTable_exp(){
	lookUpTable = (double*)malloc(sizeof(double) * maxOcc);
	for(int i = 0; i < maxOcc;i++){
		lookUpTable[i] = pow(cb,-i);
	}
}

void initLookUpTable_poly(){
	lookUpTable = (double*)malloc(sizeof(double) * maxOcc);
	for(int i = 0; i < maxOcc;i++){
		lookUpTable[i] = pow((eps+i),-cb);
	}
}

