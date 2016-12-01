#include <iostream>
#include <algorithm>
#include <functional>

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
    

        
    pid_t pid = fork();
        
    if (pid < 0) {
        cerr << "Error in forking" << endl;
        exit(1);
    }
        
    else if (pid == 0) { //sort first half [0, n/2) in ascending order
        sort(arr, arr + (n/2), less_equal<int>() );
        exit(0);
    }
        
    else {
        sort(arr + (n/2), arr + n,  greater_equal<int>() );
        wait(NULL);
    }
    
    cout << endl << "Parent, printing modified array:" << endl;
            
    cout << endl << "{";
    for (int i = 0; i < n; i++) {
        cout << " " << arr[i] << " ";
    }
    cout << "}" << endl;
    
    //UNLINK & UNMAP
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