#include <bits/stdc++.h>

#define NUMPROC 4
#define NUMREC 3

using namespace std;

vector<int> R {9, 3, 6}; //Total res
/*Valid*/
vector<int> V {1, 1, 2}; //Available

// /*Invalid*/
// vector<int> V {0, 1, 1}; //Available


vector<vector<int>> C { //claim
    {3, 2, 2},
    {6, 1, 3},
    {3, 1, 4},
    {4, 2, 2}
};

/*Valid*/
vector<vector<int>> A { //Allocated
    {1, 0, 0},
    {5, 1, 1},
    {2, 1, 1},
    {0, 0, 2}
};

// /*Invalid*/
// vector<vector<int>> A { //Allocated
//     {2, 0, 1},
//     {5, 1, 1},
//     {2, 1, 1},
//     {0, 0, 2}
// };



// L = C - A, the resources claimed but yet to be allocated
vector<vector<int>> L (NUMPROC, vector<int>(NUMREC));




bool finishedProc(vector<int> P);
bool lessThanEq(vector<int> X, vector<int> Y);
bool boundsCheck(void);

void computeL(void);
void setZero(vector<int>& P);

void printV(vector<int> X);
void printM(vector<vector<int>> M);

int main (void) {

    cout << boundsCheck() << endl;
    computeL();

    cout << "C = ";
    printM(C);

    cout << "A = ";
    printM(A);

    cout << "C - A = ";
    printM(L);

    cout << "R = ";
    printV(R);

    cout << "V = ";
    printV(V);


    bool procLeft = true, validLeft = true;

    while (procLeft && validLeft) {
        procLeft = false;
        validLeft = false;

        for (int i = 0; i < NUMPROC; i++) {
            if (!finishedProc(C[i])) {
                procLeft = true;
                if (lessThanEq(L[i], V)) { //can the process i be completed with available resources V?
                    validLeft = true;
                    cout << "Can complete process " << i << endl;
                    for (int j = 0; j < NUMREC; j++) {
                        V[j] += A[i][j]; // adding allocated resources back
                    }
                    cout << "Updating values...\n";
                    setZero(C[i]);
                    setZero(A[i]);
                    computeL();

                    cout << "C = ";
                    printM(C);

                    cout << "A = ";
                    printM(A);

                    cout << "C - A = ";
                    printM(L);

                    cout << "V = ";
                    printV(V);
                }
            }
        }
    }

    if (procLeft && !validLeft) {
        cout << "\n\n Unsafe state!!!" << endl;
    }

    else {
        cout << "\n\n Safe state!!!" << endl;
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

bool boundsCheck(void){

    //Check if R[j] = V[j] + SUM_{i}(A[i][j]), for all j
    for (int j = 0; j < NUMREC; j++) {
        int sum = 0;
        for (int i = 0; i < NUMPROC; i++) {
            sum += A[i][j];
        }
        if (R[j] != V[j] + sum) {
            return false;
        }
    }

    //Check if Claim is less than Resource, Alloc is less than Claim
    for (int i = 0; i < NUMPROC; i++) {
        if (!lessThanEq(C[i], R)) {
            return false;
        }
        if (!lessThanEq(A[i], C[i])) {
            return false;
        }
    }

    return true;
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


void computeL(void) {
    for (int i = 0; i < NUMPROC; i++) {
        for (int j = 0; j < NUMREC; j++) {
            L[i][j] = C[i][j] - A[i][j];
        }
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
