#include <iostream>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdlib.h>


using namespace std;

static const int NUM_BYTES = 256*sizeof(int);


static int shmfd;
static const char* shmName = "shmobj";


static int *arr;
static int n;

int partion(int first, int last);


int main(void) {
    
    shmfd = shm_open(shmName, O_CREAT | O_RDWR, 0777);
    ftruncate(shmfd, NUM_BYTES);
    
    arr = (int*)mmap(NULL, NUM_BYTES,  PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
    
    
    cout << "Enter n: ";
    cin >> n;
    
    for (int i = 0; i < n; i++) {
        cout << "Enter arr[" << i << "]: ";
        cin >> arr[i];
    }
    
    int low = 0;
    int hi = n-1;
    
    cout << endl << "{";
    for (int i = 0; i < n; i++) {
        cout << " " << arr[i] << " ";
    }
    cout << "}" << endl;
    
    pid_t main_proc = getpid();
    
    
    while(true) {
        
        if(low >= hi) {
            exit(0);
        }
        
        int p = partion(low, hi);
        
        pid_t pid = fork();
        
        if (pid < 0) {
            cerr << "Error in forking" << endl;
            exit(1);
        }
        
        else if (pid == 0) {
            hi = p-1;
        }
        
        else {
            pid_t pid = fork();
            
            if (pid < 0) {
                cerr << "Error in forking" << endl;
                exit(1);
            }
            
            else if(pid == 0){
                low = p+1;
            }
            
            else {
                wait(NULL); wait(NULL);
                if(getpid() == main_proc) {
                    break;
                }
                else {
                    exit(0);
                }
            }
        }
    }
    
    cout << endl << "Parent, printing modified array:" << endl;
            
    cout << endl << "{";
    for (int i = 0; i < n; i++) {
        cout << " " << arr[i] << " ";
    }
    cout << "}" << endl;
    
    
    int err = munmap((void *)arr, NUM_BYTES);
    if(err == -1) {
        perror("MUNMAP");
        exit(2);
    }
    
    err = shm_unlink(shmName);
    
    if(err == -1) {
        perror(shmName);
        exit(2);
    }
    
    return 0;
}








/*void Sorter::quick_sort(int first, int last) {

    //
    //    Implements classical quick-sort,
    //    with pivot = last element
    //


	if(first < last) {

		int pivotInd = partion(first, last);
		quick_sort(first, pivotInd-1);
		quick_sort(pivotInd+1, last);
	}
}*/

int partion(int first, int last) {

	int pivot = arr[last];

	int i = first - 1;

	//
    //    Loop Invariant(s):
    //    >> All elements in arr[start, i] {ends included} are <= arr[pivot]
    //    >> All elements in arr[i, j] {ends excluded are > arr[pivot]}
	//

	for(int j = first ; j < last ; j++) {

		if(arr[j] <= pivot) {

			i++;
			//Swap elts at i and j
			int temp = arr[i];
			arr[i] = arr[j];
			arr[j] = temp;
		}
	}

    //Swap elts at i+1 and last (pivot)
	int temp = arr[i+1];
	arr[i+1] = pivot;
	arr[last] = temp;

	return i+1;
}