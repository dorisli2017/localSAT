/*
 * main.cpp
 *
 *  Created on: May 8, 2018
 *      Author: ping
 */

#include "l.h"

int main(int argc, char *argv[]){
	fileName = argv[1];
	seed = atoi(argv[2]);
	noise = atof(argv[3]);
	readFile(fileName);
	switch(mrq){
	case 0:{
		Process<minstd_rand0> process;
		process.optimal();
		break;
	}
	case 1:{
		Process<minstd_rand> process;
		process.optimal();
		break;
	}
	case 2:{
		Process<mt19937> process;
		process.optimal();
		break;
	}
	case 3:{
		Process<mt19937_64> process;
		process.optimal();
		break;
	}
	case 4:{
		Process<ranlux24_base> process;
		process.optimal();
		break;
	}
	case 5:{
		Process<ranlux48_base> process;
		process.optimal();
		break;
	}
	case 6:{
		Process<ranlux24> process;
		process.optimal();
		break;
	}
	case 7:{
		Process<ranlux48> process;
		process.optimal();
		break;
	}
	case 8:{
		Process<knuth_b> process;
		process.optimal();
		break;
	}
	default:{
		Process<default_random_engine> process;
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
Process<T>::Process(){
	//set the parameters
	   // set tabuS
	if(mrq == 10){
		srand(seed);
		randINT = &Process::randI2;
	}
	else {
		randINT = &Process::randI;
		generator.seed(seed);
	}
	tabuS = (int*) malloc(sizeof(int) * numVs);
	for(int i = 0; i < numVs; i++){
		tabuS[i] = 0;
	}
	maxLOcc = maxOcc*lct;
	numP = (int*) malloc(sizeof(int) * numCs);
	probs = (double*)malloc(sizeof(double) * numVs);
	assign = (bool*)malloc(sizeof(bool) * numVs);
	//set inititial assignment
	if(maxL <= 3){
		ict = 0;
		cb = 2.06;
		eps = 0.9;
		fct = 0;
		setAssignment =&Process::setAssignment3;
		getFlipLiteral =&Process::getFlipLiteral3;
		flip = &Process::flip3;
	}
	else if (maxL <=4){
		ict = 0;
		cb = 2.85;
		setAssignment =&Process::setAssignment3;
		getFlipLiteral =&Process::getFlipLiteral3;
		flip = &Process::flip3;
	}
	else if(maxL <=5){
		ict = 1;
		cb = 3.7;
		setAssignment =&Process::setAssignment57;
		getFlipLiteral =&Process::getFlipLiteral57;
		flip = &Process::flip57;
	}
	else if(maxL <= 6){
		ict = 1;
		cb = 5.1;
		setAssignment =&Process::setAssignment57;
		getFlipLiteral =&Process::getFlipLiteral57;
		flip = &Process::flip57;
	}
	else{
		ict = 1;
		cb = 5.4;
		setAssignment =&Process::setAssignment57;
		getFlipLiteral =&Process::getFlipLiteral57;
		flip = &Process::flip57;
	}
	//set lookuptable
	switch(ict){
	case 0:randomAssignment();break;
	case 1:biasAssignment();break;
	default: randomBiasAssignment();
	}
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
		ratio = (double)numCs/numVs;
		return;
    }// for the p line
    int lit;
    int size = 0;
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
			if(size > maxL) maxL = size;
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
	cout<<"c seed: "<<seed<<endl;
	cout<<"c fct: "<<fct<<endl;
	cout<<"c ict: "<<ict<<endl;
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
			else assign[i] = false;
	}
	(this->*setAssignment)();
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
	(this->*setAssignment)();
}
template<class T>
void Process<T>::randomAssignment(){
   	for(int j = 0; j < numVs; j++){
   		assign[j] = ((this->*randINT)()%2 ==1);
   	}
	(this->*setAssignment)();
}
template<class T>
void Process<T>::setAssignment3(){
   	for(int i = 0; i < numCs; i++){
   		numP[i] = 0;
   	}
   	for(int j = 0; j < numCs; j++){
   		for (std::vector<int>::const_iterator i = clauses[j].begin(); i != clauses[j].end(); ++i){
   			if(((*i) > 0 && (assign[*i]== true))||((*i) < 0 && assign[-*i]== false)){
   				numP[j]++;
   			}
   		}
   		if(numP[j] == 0){
   			unsatCs.push_back(j);
   		}
   	}

}
template<class T>
void Process<T>::setAssignment57(){
   	for(int i = 0; i < numCs; i++){
   		numP[i] = 0;
   	}
	breaks = (int*) malloc(sizeof(int) * numVs);
	critVar = (int*) malloc(sizeof(int) * numCs);
	for (int i = 0; i <= numVs; i++) {
		breaks[i] = 0;
	}
	int critV = 0;
   	for(int j = 0; j < numCs; j++){
   		for (std::vector<int>::const_iterator i = clauses[j].begin(); i !=clauses[j].end(); ++i){
   			if(((*i) > 0 && assign[*i]== true)||((*i) < 0 && assign[-*i]== false)){
   				numP[j]++;
   				critV = *i;
   			}
   		}
   		if(numP[j] == 0){
   			unsatCs.push_back(j);
   		}
   		else{
   			if(numP[j] == 1){
   				critV = abs(critV);
   				critVar[j] = critV;
   				breaks[critV]++;
   			}
   		}
   	}
}

template<class T>
void Process<T>::optimal(){
	while(true){
		if (unsatCs.size()== 0){
			cout<< "s SATISFIABLE"<< endl;
			printf("flips : %llu", flipCount);
			printf("greedyflips : %llu", flipGCount);
			//test();
			return;
		}
		search_prob();
	}
}
template<class T>
int Process<T>::getFlipLiteral3(int cIndex){
	clauseT.clear();
	vector<int>&  vList = clauses[cIndex];
	int j=0,bre,min = numCs+1;
	double sum=0,randD;
	int greedyLiteral = 0;
	int  randomLiteral;
	for (std::vector<int>::const_iterator i = vList.begin(); i != vList.end(); ++i){
		bre = computeBreakScore(*i);
		if(bre == 0){
			clauseT.push_back(*i);
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
	int cS = clauseT.size();
	double temp;
	if(cS > 0){
		int index = (this->*randINT)()%cS;
		if(cS < maxLOcc){
			temp = noise*lookUpTable[cS];
		}
		else{
			temp =  noise*(this->*Process::lookUp)(cS);
		}
		greedyLiteral = clauseT[index];
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
	if(cS  ==  0 || (greedyLiteral == randomLiteral)) return randomLiteral;
	int s1 = tabuS[abs(greedyLiteral)];
	int s2 = tabuS[abs(randomLiteral)]+s1;
	if(s2==0  ||(temp*((this->*randINT)()%s2)) > s1) return greedyLiteral;
	else return randomLiteral;
}
template<class T>
int Process<T>::getFlipLiteral57(int cIndex){
	clauseT.clear();
	vector<int>&  vList = clauses[cIndex];
	int j=0,bre,min = numCs+1;
	double sum=0,randD;
	int greedyLiteral = 0;
	int  randomLiteral;
	for (std::vector<int>::const_iterator i = vList.begin(); i != vList.end(); ++i){
		bre = breaks[abs(*i)];
		if(bre == 0){
			clauseT.push_back(*i);
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
	int cS = clauseT.size();
	double temp;
	if(cS > 0){
		int index = (this->*randINT)()%cS;
		if(cS < maxLOcc){
			temp = noise*lookUpTable[cS];
		}
		else{
			temp=  noise*(this->*Process::lookUp)(cS);
		}
		greedyLiteral = clauseT[index];
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
	if(cS  ==  0 || (greedyLiteral == randomLiteral)) return randomLiteral;
	int s1 = tabuS[abs(greedyLiteral)];
	int s2 = tabuS[abs(randomLiteral)]+s1;
	if(s2==0  || (temp*((this->*randINT)()%s2)) > s1) return greedyLiteral;
	else return randomLiteral;
}
template<class T>
void Process<T>::flip3(int literal){
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
void Process<T>::flip57(int literal){
	flipCount++;
	int aIndex = abs(literal);
	assign[aIndex] = (literal > 0);
    vector<int>& occList =(literal > 0)? negC[aIndex] :posC[aIndex];
    vector<int>& deList =(literal > 0)? posC[aIndex]:negC[aIndex];
	std::vector<int>::const_iterator i;
	for (i = occList.begin(); i != occList.end(); ++i){
		if(numP[*i] == 1){
			breaks[aIndex]--;
			unsatCs.push_back(*i);
		}
		else if(numP[*i] == 2){
			int aj;
			for(std::vector<int>::const_iterator j = clauses[*i].begin(); j != clauses[*i].end(); ++j){
				aj = abs(*j);
				if (((*j)==aj)== assign[aj]) {
					critVar[*i] = aj;
					breaks[aj]++;
					break;
				}
			}

		}
		numP[*i]--;
	}
	for (i = deList.begin(); i != deList.end(); ++i){
		if(numP[*i]== 0){
			critVar[*i] = aIndex;
			breaks[aIndex]++;
		}
		else if(numP[*i]== 1){
			breaks[critVar[*i]]--;
		}
		numP[*i]++;
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
	int size = unsatCs.size();
	int randC = (this->*randINT)()%size;
	int flipCindex = unsatCs[randC];
	while(numP[flipCindex] > 0){
		unsatCs[randC]=unsatCs.back();
		unsatCs.pop_back();
		size--;
		if(size == 0) return;
		randC = (this->*randINT)()%size;
		flipCindex = unsatCs[randC];
	}
	int flipLindex = (this->*Process::getFlipLiteral)(flipCindex);
	unsatCs[randC]=unsatCs.back();
	unsatCs.pop_back();
	(this->*flip)(flipLindex);
	tabuS[abs(flipLindex)]++;
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

