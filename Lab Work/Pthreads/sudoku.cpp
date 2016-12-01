#include <iostream>
#include <vector>
#include <cstdlib>

#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>

using namespace std;


bool checkRow(int r);
bool checkCol(int c);
bool checkBox(int r, int c);

void* checkSudo(void* opt);

vector<vector<int>> sudVec {
	{5, 1, 3,   6, 8, 7,   2, 4, 9},
	{8, 4, 9,   5, 2, 1,   6, 3, 7},
	{2, 6, 7,   3, 4, 9,   5, 8, 1},

	{1, 5, 8,   4, 6, 3,   9, 7, 2},
	{9, 7, 4,   2, 1, 8,   3, 6, 5},
	{3, 2, 6,   7, 9, 5,   4, 1, 8},

	{7, 8, 2,   9, 3, 4,   1, 5, 6},
	{6, 3, 5,   1, 7, 2,   8, 9, 4},
	{4, 9, 1,   8, 5, 6,   7, 2, 3}
};

bool rowsCorrect, colsCorrect, boxesCorrect;



int main(void) {


	for (int i = 0; i < sudVec.size(); ++i) {
		for (int j = 0; j < sudVec[i].size(); ++j) {
			cout << " " << sudVec[i][j] << " ";
			if (j%3 == 2)
				cout << "  ";

		}
		if (i%3 == 2)
			cout << endl;
		cout << endl;
	}


	char rowOpt[] = "r";
	char colOpt[] = "c";
	char boxOpt[] = "b";

	pthread_t t1, t2, t3;

	pthread_create(&t1, NULL, &checkSudo, (void*)(&rowOpt));
	pthread_create(&t2, NULL, &checkSudo, (void*)(&colOpt));
	pthread_create(&t3, NULL, &checkSudo, (void*)(&boxOpt));

	printf("abc\n");


	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_join(t3, NULL);

	printf("123\n");

	cout << "\n\n\n" << "(rowsCorrect, colsCorrect, boxesCorrect) = (" << rowsCorrect << ", " << colsCorrect << ", " << boxesCorrect << ")" << endl;
	return 0;
}

bool checkRow(int r) {

	vector<bool> mask(9, false);
	for (int j = 0; j < sudVec[r].size(); ++j) {
		if (sudVec[r][j]-1 < 0 || sudVec[r][j]-1 > 9) {
			return false;
		}
		mask[sudVec[r][j]-1] = true;
	}



	for (int i = 0; i < mask.size(); ++i) {
		if(!mask[i])
			return false;
	}
	return true;
}

bool checkCol(int c) {

	vector<bool> mask(9, false);
	for (int i = 0; i < sudVec.size(); ++i) {
		if (sudVec[i][c]-1 < 0 || sudVec[i][c]-1 > 9) {
			return false;
		}
		mask[sudVec[i][c]-1] = true;
	}


	for (int i = 0; i < mask.size(); ++i) {
		if(!mask[i])
			return false;
	}
	return true;
}

bool checkBox(int r, int c) {

	vector<bool> mask(9, false);

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			if (sudVec[r+i][c+j]-1 < 0 || sudVec[r+i][c+j]-1 > 9) {
				return false;
			}
			mask[sudVec[r+i][c+j]-1] = true;
		}
	}

	for (int i = 0; i < mask.size(); ++i) {
		if(!mask[i])
			return false;
	}
	return true;
}


void* checkSudo(void* opt) {

	char option = ((char*)opt)[0];

	cout << "Enter " << option << " \n";

	if(option == 'r') {

		rowsCorrect = true;

		for (int i = 0; i < sudVec.size(); ++i) {
			cout << " " << "row:" << i << ":" << checkRow(i) << " ";
			rowsCorrect = rowsCorrect && checkRow(i);
		}
		cout << endl;
	}

	else if (option == 'c') {

		colsCorrect = true;

		for (int i = 0; i < sudVec.size(); ++i) {
			cout << " " << "col:" << i << ":" << checkCol(i) << " ";
			colsCorrect = colsCorrect && checkCol(i);
		}
		cout << endl;

	}

	else if (option == 'b') {

		boxesCorrect = true;

		for (int i = 0; i < sudVec.size(); i += 3) {
			for (int j = 0; j < sudVec[i].size(); j += 3) {
				cout << " " << i << "," << j << ":" << checkBox(i, j) << " ";
				boxesCorrect = boxesCorrect && checkBox(i, j);
			}
		}
		cout << endl;
	}

	else {
		cerr << "Incorrect Call" << endl;
		printf("Incorrect call!\n");
		exit(1);
	}
	pthread_exit(NULL);
}
