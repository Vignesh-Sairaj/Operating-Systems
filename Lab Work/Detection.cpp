#include <bits/stdc++.h>

#define NUMPROC 4
#define NUMREC 3

using namespace std;

vector<int> R {9, 3, 6}; //Total res
// /*Valid*/
// vector<int> V {1, 1, 2}; //Available

/*Invalid*/
vector<int> V {0, 1, 1}; //Available


// /*Valid*/
// vector<vector<int>> A { //Allocated
//     {1, 0, 0},
//     {5, 1, 1},
//     {2, 1, 1},
//     {0, 0, 2}
// };

/*Invalid*/
vector<vector<int>> A { //Allocated
    {2, 0, 1},
    {5, 1, 1},
    {2, 1, 1},
    {0, 0, 2}
};

// /*Valid*/
// vector<vector<int>> Q { //Request
//     {2, 2, 2},
//     {1, 0, 2},
//     {1, 0, 3},
//     {4, 2, 0}
// };

/*Invalid*/
vector<vector<int>> Q { //Request
    {1, 2, 1},
    {1, 0, 2},
    {1, 0, 3},
    {4, 2, 0}
};

vector<int> W = V;
vector<int> marked(NUMPROC, 0);

bool finishedProc(vector<int> P);
bool lessThanEq(vector<int> X, vector<int> Y);


void setZero(vector<int>& P);

void printV(vector<int> X);
void printM(vector<vector<int>> M);

int main (void) {

    cout << "A = ";
    printM(A);

    cout << "Q = ";
    printM(Q);

    cout << "V = ";
    printV(V);

    cout << "W = ";
    printV(W);

    for (int i = 0; i < NUMPROC; i++) {
        if (finishedProc(A[i])) {
            marked[i] = 1;
        }
    }


    bool procLeft = true, validLeft = true;

    while (procLeft && validLeft) {
        procLeft = false;
        validLeft = false;

        for (int i = 0; i < NUMPROC; i++) {
            if (!marked[i]) {
                procLeft = true;
                if (lessThanEq(Q[i], W)) { //can the Request for i be granted with available resources W?
                    validLeft = true;
                    cout << "Can grant request to process " << i << endl;
                    for (int j = 0; j < NUMREC; j++) {
                        W[j] += A[i][j]; // adding allocated resources back
                    }
                    cout << "Updating values...\n";
                    marked[i] = 1;

                    cout << "A = ";
                    printM(A);

                    cout << "Q = ";
                    printM(Q);

                    cout << "W = ";
                    printV(W);
                }
            }
        }
    }

    if (procLeft && !validLeft) {
        cout << "\n\n Deadlocked state!!!" << endl;
    }

    else {
        cout << "\n\n Valid state!!!" << endl;
    }

    return 0;
}

bool lessThanEq(vector<int> X, vector<int> Y) {
    if (X.size() != Y.size()) {
        throw runtime_error("lessThanEq: Sizes do not match.\n");
    }
    bool ans = true;
    for (size_t i = 0; i < X.size(); i++) {
        if (X[i] > Y[i]) {
            ans = false;
            break;
        }
    }
    return ans;
}


bool finishedProc(vector<int> L) {
    for (size_t i = 0; i < L.size(); i++) {
        if (L[i] != 0) {
            return false;
        }
    }
    return true;
}

void setZero(vector<int>& P) {
    for (size_t i = 0; i < P.size(); i++) {
        P[i] = 0;
    }
}

void printV(vector<int> X) {
    cout << "{";
    for (size_t i = 0; i < X.size(); i++) {
        cout << " " << X[i] << " ";
    } cout << "}" << endl;
}
void printM(vector<vector<int>> M) {
    cout << endl;
    for (size_t i = 0; i < M.size(); i++) {
        cout << "  ";
        for (size_t j = 0; j < M[i].size(); j++) {
            cout << " " << M[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}
