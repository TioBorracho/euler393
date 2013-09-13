#include <string>
#include <iostream>
#include <vector>
using namespace std;
#define vvi vector<vector<int> >
#define vi vector<int> 

bool valid(vi* antePreRow, vi* prevRow, vi &nextRow, bool last) {
	bool ok = true;
	if (prevRow == NULL) {
		//first row
		int lastPos = nextRow.size() -1;
		//corner cases
		//left corner
		ok &= nextRow[0] == 1 && nextRow[1] == 2 || 
			  nextRow[0] == 3 && nextRow[1] != 2;
	   	//right corner
		
		ok &= nextRow[lastPos] == 1 && nextRow[lastPos-1] == 3 || 
			  nextRow[lastPos] == 2 && nextRow[lastPos-1] != 3;

		for (int i = 0; ok && i < lastPos; ++i) {
			//No crossing
			ok &= !(nextRow[i] == 2 && (i != 0 && nextRow[i-1] == 3)) && nextRow[i] != 0;
			/*if (!ok)
				cout << "failed on first on " << i << endl;*/
			//no 2 on same box
			ok &= !(nextRow[i] == 3 && (i+2<=lastPos && nextRow[i+2]== 2));
			/*if (!ok)
				cout << "failed on second on " << i << endl;*/

		}
		
		
		return ok;
	} else {

	}
};
bool inc(vi *vec) {
	bool stop = false;
	for (int i = vec->size()-1; !stop && i >= 0; --i){
		(*vec)[i]++;
		if ((*vec)[i] == 4 && i != 0)
			(*vec)[i] = 0;
		else 
			stop = true;
	}
	if ((*vec)[0] == 4)
		return false;
	else 
		return true;
};
string arrow(int n) {	
	switch (n) {
		case 0: return "↑"; break;
		case 1: return "↓"; break;
		case 2: return "←"; break;
		case 3: return "→"; break;
		default: return "O"; break;
	}
}
void printVector(vi vector) {
	bool val = valid(NULL, vector);
	if (val) {
	for (int i = 0; i < vector.size(); ++i)
		cout << arrow(vector[i]) << " ";
	cout << val << endl;
}
}



vvi getNextCombination(vvi *previous, bool isFirst, bool isLast, int n) {
	vvi ret ;
	if (isFirst) {
		vi actual;
		actual.resize(n,0);
		while (inc(&actual)) {
			printVector(actual);
		}

	}
}
int main (int argc, char *argv[]) {
	if (argc != 2) {
		cout << "missing size";
		return 0;
	}

	int n = atoi(argv[1]);
	cout << "solving euler 393 for " << n << endl;
	if (n%2==1 || n < 4) 
		cout << "Solution: 0\n";
	else {
		/*
		0: up
		1: down
		2: left
		3: right
		*/
		getNextCombination(NULL, true,false,n);
	}


	return 0;
};
