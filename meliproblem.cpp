#include <string>
#include <iostream>
#include <vector>
using namespace std;
#define vvr vector<vr>
#define vr vector<row>
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
//solutions http://oeis.org/A216678
struct row {
	bool valid;
	long ocuppied;
	long move;
	long fixed;
	int size;
};
void printVector(row);
bool checkCover(long* ocuppied, int i) {
	if (((*ocuppied) >> i )%2 == 1) return false;
	(*ocuppied) += (1 << i);
	return true;
}

bool valid(row* prevRow, row *nextRow, bool isLast, int size, bool debug = false) {
	//any move for everything
	long map = 1 << size -1;
	bool ok = true ;
	//only one

		long move = nextRow->move;
		nextRow->ocuppied = 0;

	if (prevRow == NULL) {
		//first row
		//right corner
		long firstMove = nextRow->move % 4;
		ok &= firstMove != UP && firstMove != RIGHT;
		//left corner
		ok &= ((nextRow->move >> (2*(size-1))) % 2 == 1);
		long current = move % 4, next = (move >> 2) % 4, last = -1;
		for (int i = 0; ok && i < size; ++i) {
			if (debug)	cout << "iteration " << i << endl;
			//no up
			ok &= current != UP;
			//check crossings and fill ocuppieds
			//No crossing (not left and right on next)
			if (debug)	cout << ok << endl;
			ok &= !(current == LEFT && next == RIGHT);
			if (debug)	cout << ok << endl;
			if (current==RIGHT) {
				ok &= (nextRow->ocuppied >> (i-1)) % 2 == 0;
				nextRow->ocuppied += 1 << (i-1);
			}
			if (debug)	cout << ok << endl;

			if (current==LEFT) {
				ok &= (nextRow->ocuppied >> (i+1)) % 2 == 0;
				nextRow->ocuppied += (1 << (i+1));
			}
			if (debug)	cout << ok << endl;
			if (current == DOWN) {
				ok &= next == RIGHT || last == LEFT;
			}
			if (debug)	cout << ok << endl;
			move = move >> 2;
			last = current % 4;
			current = move % 4;
			next = (move >> 2 )% 4;
		}


		return ok;
	} else {
				long pmove = prevRow->move;

		long ocuppied = prevRow->ocuppied;
		//middle row, compare with previous
		//sides cases
		//right side
		ok &= (nextRow->move % 4) != RIGHT;
	   	//left corner
		ok &= ((nextRow->move >> (2*(size-1))) != LEFT);
		long current = move % 4, next = (move >> 2) % 4, last = -1,
			 pcurrent = pmove % 4, pnext = (pmove >> 2 )% 4, plast = -1;

		for (int i = 0; ok && i < size; ++i) {

			//No crossing
			ok &= !(current == LEFT && next == RIGHT);
			ok &= !(current == UP && pcurrent == DOWN) ;

			//check coverage
			if (pcurrent == DOWN)
				ok &= checkCover(&nextRow->ocuppied, i);
			if (i != 0 && last == LEFT)
				ok &= checkCover(&nextRow->ocuppied, i);
			if (i != size-1 && next == RIGHT)
				ok &= checkCover(&nextRow->ocuppied, i);


			//every box has its in && out
			//prev
			switch (current) {
				case UP:
					ok &= (prevRow->ocuppied >> (i)) % 2 == 0 &&
						   (!isLast || next == RIGHT || last == LEFT)	;
				break;

				case DOWN:
					ok &= !isLast && //no es la ultima
							(prevRow->ocuppied >> (i)) % 2 == 1 && //la de arriba ya fue cubierta
							((nextRow->ocuppied >> i) % 2 == 1) ; //me cubre alguna
				break;
				case LEFT: //remember check from R to L, L is next
					ok &= ! (i == size - 1) && //no es el extremo izquierdo
						(prevRow->ocuppied >> (i)) % 2 == 1;//la de arriba esta cubierta
				break;
				case RIGHT:
					ok &= (i != 0 )&&
						(prevRow->ocuppied >> (i)) % 2 == 1 ;
				break;

			}
			if (isLast) {
				ok &= (nextRow->ocuppied >> (i)) % 2 == 1;
			}
			move = move >> 2;
			pmove = pmove >> 2;
			last = current % 4;
			plast = pcurrent % 4;
			current = move % 4;
			pcurrent = pmove % 4;

			next = (move >> 2 )% 4;
			pnext = (pmove >> 2) % 4;

		}
				prevRow->ocuppied = ocuppied;

		return ok;

	}
};
bool inc(row *vec) {
	bool stop = false;

	vec->move++;
	if (vec->move == 1<<(2*vec->size))
		return false;
	return true;
};
string arrow(int n) {
	switch (n) {
		case UP: return "↑"; break;
		case DOWN: return "↓"; break;
		case LEFT: return "←"; break;
		case RIGHT: return "→"; break;
		default: return "O"; break;
	}
}
void printVector(row vector) {
	string moves = "";
	long theMoves = vector.move;
	cout << vector.move << ":\t";
	for (int i = 0; i < vector.size; ++i, theMoves = theMoves >> 2) {
		moves = arrow(theMoves % 4) + moves;
	}
	cout << moves << endl;
}
void printSolution(vr rows) {
	for (int i = 0; i < rows.size(); ++i) {
		printVector(rows[i]);
	}
}


vvr getConfigurations(vr chain, bool isFirst, bool isLast, int n, int level) {
	vvr ret ;
	//cout << "recursing level " << level << endl;
	row actual;
	actual.move = -1;
	actual.size = n;
	while (inc(&actual)) {
		row * previous = chain.size() == 0 ? NULL : &(chain.back());
		bool ok = valid(previous, &actual,isLast, n);
		if (ok) {
			if (level == 0) {
				cout << "NEW TOP LEVEL VECTOR: ";
				printVector(actual);
			}
			chain.push_back(actual);
			/*for (int i = 0; i < chain.size(); ++i)
				printVector(chain[i]);*/
			if (!isLast) {
				vvr rret = getConfigurations(chain,false,n == level+2, n, level +1 );
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
void getCountConfigurations(vr chain, bool isFirst, bool isLast, int n, int level, long &sols) {
	//cout << "recursing level " << level << endl;
	row actual;
	actual.move = -1;
	actual.size = n;
	while (inc(&actual)) {
		row * previous = chain.size() == 0 ? NULL : &(chain.back());
		bool ok = valid(previous, &actual,isLast, n);
		if (ok) {
			if (level == 0) {
				cout << "NEW TOP LEVEL VECTOR: ";
				printVector(actual);
			}
			chain.push_back(actual);
			/*for (int i = 0; i < chain.size(); ++i)
				printVector(chain[i]);*/
			if (!isLast) {
				getCountConfigurations(chain,false,n == level+2, n, level +1 , sols);
			}
			else {
				printSolution(chain);//ret.push_back(chain);
				sols++;
				cout << sols << endl;
			}

			chain.pop_back();
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
	if (n%2==1 || n < 2)
		cout << "Solution: 0\n";
	else {
		row r;
		r.move = -1;
		r.ocuppied = 0;
		r.size = n;
		//vvr sol =  getConfigurations(vr(), true,false,n,0);
		long s = 0;
		getCountConfigurations(vr(), true,false,n,0,s);
		//cout << endl << endl << "Solutions (" << sol.size() << "):" << endl;
		cout << endl << endl << "Solutions (" << s << "):" << endl;
		/*for (int i = 0; i < sol.size(); ++i) {
			printSolution(sol[i]);
			cout << "--------------"<< endl;
		}
		r.move=13;
		cout << valid(NULL, &r, false, n, true);*/
	}


	return 0;
};
