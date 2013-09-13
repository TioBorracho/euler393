#include <string>
#include <iostream>
#include <vector>
using namespace std;
#define vvvi vector<vector<vector<int> > >
#define vvi vector<vector<int> >
#define vi vector<int> 
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
void printVector(vi);
bool valid(vi* antePreRow, vi* prevRow, vi &nextRow, bool last) {
	bool ok = true;
	int lastPos = nextRow.size() -1;
	bool debug = false;
	if (prevRow != NULL && antePreRow != NULL) {
		/*cout << "validating:" << endl;
		printVector(*prevRow);
		printVector(nextRow);
		debug = false;*/
	}
	if (prevRow == NULL) {
		//first row
		//corner cases
		//left corner
		ok &= nextRow[0] == 1 && nextRow[1] == 2 || 
			  nextRow[0] == 3 && nextRow[1] != 2;
	   	//right corner
		
		ok &= nextRow[lastPos] == 1 && nextRow[lastPos-1] == 3 || 
			  nextRow[lastPos] == 2 && nextRow[lastPos-1] != 3;

		for (int i = 0; ok && i <= lastPos; ++i) {
			//No crossing
			ok &= !(nextRow[i] == 2 && (i != 0 && nextRow[i-1] == 3)) && nextRow[i] != 0;
			//no 2 on same box
			ok &= !(nextRow[i] == 3 && (i+2<=lastPos && nextRow[i+2]== 2));
		}
		
		
		return ok;
	} else {
		//middle row, compare with previous
		//sides cases
		//left side
		ok &= (nextRow[0] != LEFT);
	   	//right corner
		ok &= nextRow[lastPos] != RIGHT; 


	    if (debug && !ok) cout << "stage 1 " << ok;

		for (int i = 0; ok && i <= lastPos; ++i) {

			//No crossing
			ok &= !(nextRow[i] == LEFT && (i != 0 && nextRow[i-1] == RIGHT)) && 
				  !(last && nextRow[i] == DOWN) &&
				  !(nextRow[i] == UP && (*prevRow)[i] == DOWN)	 ;

			//no 2 on same box
			ok &= !(nextRow[i] == RIGHT && (i+2<=lastPos && nextRow[i+2]== LEFT)) ;
			if (antePreRow != NULL)
				ok &= !(nextRow[i] == UP && ((*antePreRow)[i]== DOWN));

			if (debug && !ok) cout << "stage 2 " << i  << " " << ok << endl;

			//every box has its in && out
			//prev
			switch ((*prevRow)[i]) {
				case UP:
					ok &= (i != 0 && (*prevRow)[i-1] == RIGHT) ||
						  (nextRow[i] == UP) || 
						  (i != lastPos && (*prevRow)[i+1] == LEFT);
				break;

				case DOWN:
					ok &= (i != 0 && (*prevRow)[i-1] == RIGHT) ||
						  (i != lastPos && (*prevRow)[i+1] == LEFT) ||
						  (antePreRow != NULL && (*antePreRow)[i] == DOWN);
				break;
				case LEFT:
					ok &= (i != lastPos && (*prevRow)[i+1] == LEFT) ||
						  (antePreRow != NULL && (*antePreRow)[i] == DOWN) ||
						  (nextRow[i] == UP);
				break;
				case RIGHT:
					ok &= (i != 0 && (*prevRow)[i-1] == RIGHT) ||
						  (antePreRow != NULL && (*antePreRow)[i] == DOWN) ||
						  (nextRow[i] == UP);
				break;

			}
			if (debug && !ok) cout << "stage 3 " << i << " " << ok << endl;
			/*somebody come here*/
			if (last) {
				ok &= (i != 0 && nextRow[i-1] == RIGHT) ||
						  ((*prevRow)[i] == DOWN) || 
						  (i != lastPos && nextRow[i+1] == LEFT);

			}
			if (debug && !ok) cout << "stage 4 " << i << " " << ok << endl;

		}
		
		
		return ok;

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
	for (int i = 0; i < vector.size(); ++i)
		cout << arrow(vector[i]) << " ";
	cout << endl;
}
void printSolution(vvi vectors) {
	for (int i = 0; i < vectors.size(); ++i) {
		printVector(vectors[i]);
	}
}


vvvi getConfigurations(vvi chain, bool isFirst, bool isLast, int n, int level) {
	vvvi ret ;
	//cout << "recursing level " << level << endl;
	vi actual;
	actual.resize(n,0);
	while (inc(&actual)) {
		
		vi * previous = chain.size() == 0 ? NULL : &(chain.back());
		vi * antePre = chain.size() <= 1 ? NULL : &(chain[chain.size()-2]);
		bool val = valid(antePre,previous, actual,isLast);
		if (val) {
			if (level == 0) {
				cout << "NEW TOP LEVEL VECTOR: ";
				printVector(actual);
			}
			chain.push_back(actual);
			/*for (int i = 0; i < chain.size(); ++i)
				printVector(chain[i]);*/
			if (!isLast) {

				vvvi rret = getConfigurations(chain,false,n == level+2, n, level +1 );
				for (int i = 0; i < rret.size(); ++i)
					ret.push_back(rret[i]);
			}
			else
				ret.push_back(chain);

			chain.pop_back();
		}
	}

	return ret;
}
int main (int argc, char *argv[]) {
	if (argc != 2) {
		cout << "missing size";
		return 0;
	}

	int n = atoi(argv[1]);
	cout << "solving euler 393 for " << n << endl;
	if (n%2==1 || n < 2) 
		cout << "Solution: 0\n";
	else {
		/*
		0: up
		1: down
		2: left
		3: right
		*/
		vvvi sol =  getConfigurations(vvi(), true,false,n,0);
		cout << endl << endl << "Solutions (" << sol.size() << "):" << endl;
		for (int i = 0; i < sol.size(); ++i) {
			printSolution(sol[i]);
			cout << "--------------"<< endl;
		}
	}


	return 0;
};
