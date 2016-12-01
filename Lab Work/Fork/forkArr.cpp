#include <iostream>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdlib.h>


using namespace std;




int main(void) {
    
    int *arr;
    int n;
    
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