/*
 * main.cpp
 *
 *  Created on: May 8, 2018
 *      Author: ping
 */

#include "l.h"

int main(int argc, char *argv[]){
	fileName = argv[1];
	readFile(fileName);
	const vector<bool> setB= {false};
// maxFlips, maxSteps,fct,ict,rct1,rct2,cct,gen,seed
	const vector<int> setI= {INT_MAX,2,1,100,0,50,10,0};
	const vector<double> setD = {3.6, 1.0,0.5};
	switch(setI[6]){
	case 0:{
		Process<minstd_rand0> process (setB, setI,setD);
		process.optimal();
		break;
	}
	case 1:{
		Process<minstd_rand> process (setB, setI,setD);
		process.optimal();
		break;
	}
	case 2:{
		Process<mt19937> process (setB, setI,setD);
		process.optimal();
		break;
	}
	case 3:{
		Process<mt19937_64> process (setB, setI,setD);
		process.optimal();
		break;
	}
	case 4:{
		Process<ranlux24_base> process (setB, setI,setD);
		process.optimal();
		break;
	}
	case 5:{
		Process<ranlux48_base> process (setB, setI,setD);
		process.optimal();
		break;
	}
	case 6:{
		Process<ranlux24> process (setB, setI,setD);
		process.optimal();
		break;
	}
	case 7:{
		Process<ranlux48> process (setB, setI,setD);
		process.optimal();
		break;
	}
	case 8:{
		Process<knuth_b> process (setB, setI,setD);
		process.optimal();
		break;
	}
	default:{
		Process<default_random_engine> process (setB, setI,setD);
		process.optimal();
		break;
	}
	}
	//randTest();
	//debugProblem();
	//process.printOptions();
	//process.debugAssign();
}
void debugProblem(){
	printVariables();
	printClauses();
	cout<< "Occurences:"<< endl;
	for(int i = 1; i < numVs; i++){
		cout<< i<< ":"<<posOc[i]<< " "<<negOc[i]<< endl;
	}
}
template<class T>
void Process<T>::debugAssign(){
	/* Testing code**********************************/
		printOptions();
	   	printAssignment();
	   	printUnsatCs();
	   	printNumP();


	/*Testing code**********************************/

}
template<class T>
Process<T>::Process(const vector<bool>& setB, const vector<int>& setI,const vector<double>& setD){
	parseOptions(setB, setI,setD);
	//set the parameters
	   // set tabuS
	if(setI[6] == 10){
		srand(seed);
		randINT = &Process::randI2;
	}
	else {
		randINT = &Process::randI;
		generator.seed(seed);
	}
	if(tabu_flag){
		tabuS = (int*) malloc(sizeof(int) * numVs);
		for(int i = 0; i < numVs; i++){
			tabuS[i] = 0;
		}
	}
	maxLOcc = maxOcc*lct;
	numP = (int*) malloc(sizeof(int) * numCs);
	probs = (double*)malloc(sizeof(double) * numVs);
	assign = (bool*)malloc(sizeof(bool) * numVs);
	//set inititial assignment
	switch(ict){
	case 0:randomAssignment();break;
	case 1:biasAssignment();break;
	default: randomBiasAssignment();
	}
	//set lookuptable
	switch (fct){
	case 0:initLookUpTable_poly();
			lookUp =&Process::LookUpTable_poly;
			break;
	default:initLookUpTable_exp();
			lookUp =&Process::LookUpTable_exp;
			break;
	}
}

/*parse the argument (including options and filename)
 *using getopt_long to allow GNU-style long options as well as single-character options
 */
template<class T>
void Process<T>::parseOptions(const vector<bool>& setB, const vector<int>& setI,const vector<double>& setD){
	tabu_flag = setB[0];

	maxFlips =setI[0];
	fct= setI[1];
	ict = setI[2];
	rct1 = setI[3];
	rct2 = setI[4];
	cct= setI[5];
	seed = setI[7];
	cb=setD[0];
	eps= setD[1];
	lct = setD[2];
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
template<class T>
void Process<T>::printOptions(){
	printf("localSAT options: \n");
	cout<<"c tabu_flag: "<<tabu_flag<<endl;
	cout<<"c maxFlips: "<<maxFlips<<endl;
	cout<<"c seed: "<<seed<<endl;
	cout<<"c fct: "<<fct<<endl;
	cout<<"c ict: "<<ict<<endl;
	cout<<"c rct1: "<<rct1<<endl;
	cout<<"c rct2: "<<rct2<<endl;
	cout<<"c cct: "<<cct<<endl;
	cout<<"c cb: "<<cb<<endl;
	cout<<"c eps: "<<eps<<endl;
	cout<<"c lct: "<<lct<<endl;
	switch(generator){
	case 0:{
		cout<<"c rand() generator with seed " <<seed <<endl;
		break;
		}
	case 1:{
		cout<<"c minstd_rand() generator with seed " <<seed <<endl;
		break;
		}
	default:{
		cout<<"c mersenne_twister_rand() generator with seed " <<seed <<endl;
	}
	}
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
template<class T>
void Process<T>::printAssignment(){
	cout<< "v ";
	for(int i = 1; i < numVs; i++){
		if(assign[i]) cout <<i<<" ";
		else cout << -i<<" ";
	}
	cout <<endl ;
}
template<class T>
void Process<T>::printUnsatCs(){
	cout<< "Unsatisfied clauses ";
	printVector(unsatCs);
	cout <<endl ;
}
template<class T>
void Process<T>::printNumP(){
	cout<< "numP: ";
	for(int i = 0; i < numCs; i++){
		cout << numP[i]<< " ";
	}
	cout<<endl;
}




void initialAssignment(){
	for(int i = 0; i < numVs; i++){
		if(posOc[i]> maxOcc) maxOcc = posOc[i];
		if(negOc[i]> maxOcc) maxOcc = negOc[i];
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
template<class T>
void Process<T>::biasAssignment(){
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
template<class T>
void Process<T>::randomBiasAssignment(){
	int sum;
	for(int i = 0; i < numVs; i++){
		sum = posOc[i] +negOc[i];
		if(sum == 0){
			assign[i] = true;
		}
		else{
			assign[i] = ((this->*randINT)()%sum)<posOc[i];
		}
	}
	setAssignment();
}
template<class T>
void Process<T>::randomAssignment(){
   	for(int j = 0; j < numVs; j++){
   		assign[j] = ((this->*randINT)()%2 ==1);
   	}
    setAssignment();
}

template<class T>
void Process<T>::setAssignment(){
   	for(int i = 0; i < numCs; i++){
   		numP[i] = 0;
   	}
	if( tabu_flag && (this->*randINT)()%100<cct){
		for(int i = 0; i < numVs; i++){
			tabuS[i] =0;
		}
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

template<class T>
void Process<T>::optimal(){
	int rct;
	while(true){
		for(unsigned int i = 0; i < maxFlips; i++){
			if (unsatCs.size()== 0){
				cout<< "s SATISFIABLE"<< endl;
				printf("flips : %llu", flipCount);
				return;
			}
			search_prob();
		}
		rct = (this->*randINT)()%100;
		if(rct < rct1) randomAssignment();
		else{
			if(rct< rct2) biasAssignment();
			else randomBiasAssignment();
		}
	}
}
template<class T>
int Process<T>::getFlipLiteral(int cIndex){
	vector<int>&  vList = clauses[cIndex];
	int j=0,bre,min= numCs+1;
	double sum=0,randD;
	int greedyLiteral = 0, randomLiteral;
	for (std::vector<int>::const_iterator i = vList.begin(); i != vList.end(); ++i){
		bre = computeBreakScore(*i);
		if(tabu_flag &&bre == 0 && tabuS[abs(*i)] == 0) return *i;
		if(bre < min){
			min = bre;
			greedyLiteral = *i;
		}
		if(bre < maxLOcc){
		sum+= lookUpTable[bre];
		}
		else{
		sum+=(this->*Process::lookUp)(bre);
		}
		probs[j]= sum;
		j++;
	}
	randD = ((double)(this->*randINT)()/RAND_MAX)*sum;
	assert(randD >= 0);
	for(int i = 0; i < j;i++){
		if(probs[i]< randD){
			continue;
		}
		randomLiteral= vList[i];
		break;
	}
	if(tabu_flag &&tabuS[abs(greedyLiteral)] < tabuS[abs(randomLiteral)]){
		return greedyLiteral;
	}
	return randomLiteral;
}
template<class T>
void Process<T>::flip(int literal){
	flipCount++;
	std::vector<int>::const_iterator i;
	if(literal > 0){
   		for (i = negC[literal].begin(); i != negC[literal].end(); ++i){
   			numP[*i]--;
   			if(numP[*i] == 0) unsatCs.push_back(*i);
   		}
		for (i = posC[literal].begin(); i != posC[literal].end(); ++i){
   			numP[*i]++;
		}

		assign[literal] = true;
	}
	else{
   		for (i = negC[-literal].begin(); i != negC[-literal].end(); ++i){
   			numP[*i]++;
   		}
		for (i = posC[-literal].begin(); i != posC[-literal].end(); ++i){
   			numP[*i]--;
   			if(numP[*i] == 0) unsatCs.push_back(*i);
		}
		assign[-literal]= false;
	}
}
template<class T>
void Process<T>::test(){
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
template<class T>
void Process<T>::testLine(string line){
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
template<class T>
int Process<T>::computeBreakScore(int literal){
    int score = 0;
    int aIndex = abs(literal);
    vector<int>& occList =(literal < 0)? posC[aIndex] :negC[aIndex];
    for(std::vector<int>::const_iterator i = occList.begin(); i != occList.end(); ++i) {
        if (numP[*i]== 1) {
            score++;
        }
    }
	//cout<< "out make "<<endl;
    return score;
}
template<class T>
double Process<T>::func_exp(int literal){
	return pow(cb,-computeBreakScore(literal));
}
template<class T>
double Process<T>::func_poly(int literal){
	return pow((eps+computeBreakScore(literal)),-cb);
}
template<class T>
void Process<T>::search_prob(){
	int randC = (this->*randINT)()%unsatCs.size();
	int flipCindex = unsatCs[randC];
	if(numP[flipCindex] > 0){
		unsatCs[randC]=unsatCs.back();
		unsatCs.pop_back();
		return;
	}
	int flipLindex = getFlipLiteral(flipCindex);
	unsatCs[randC]=unsatCs.back();
	unsatCs.pop_back();
	flip(flipLindex);
	if(tabu_flag) tabuS[abs(flipLindex)]++;
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

template<class T>
void Process<T>::initLookUpTable_exp(){

	lookUpTable = (double*)malloc(sizeof(double) * maxLOcc);
	for(int i = 0; i < maxLOcc;i++){
		lookUpTable[i] = pow(cb,-i);
	}
}
template<class T>
void Process<T>::initLookUpTable_poly(){
	lookUpTable = (double*)malloc(sizeof(double) * maxLOcc);
	for(int i = 0; i < maxLOcc;i++){
		lookUpTable[i] = pow((eps+i),-cb);
	}
}
template<class T>
double Process<T>::LookUpTable_exp(int bre){
	return pow(cb,-bre);
};
template<class T>
double Process<T>::LookUpTable_poly(int bre){
	return pow((eps+bre),-cb);
};
template<class T>
int Process<T>::randI(){
	return distribution(generator);
};

template<class T>
int Process<T>::randI2(){
	return rand();
};

