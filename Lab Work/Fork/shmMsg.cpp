#include <iostream>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <cstdlib>


using namespace std;

static const int NUM_BYTES = 1024*sizeof(char);


static int shmfd;
static const char* shmName = "shmobj";


static char *shmPtr;



int main(void) {
    
    shmfd = shm_open(shmName, O_CREAT | O_RDWR, 0777);
    ftruncate(shmfd, NUM_BYTES);
    
    shmPtr = (char*)mmap(NULL, NUM_BYTES,  PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
    
    
    
    
    pid_t pid = fork();
    
    if(pid == -1) {
        perror("Error in forking");
        exit(1);
    }
    
    
    else if(pid == 0) { //Child, write
        char buf[256];
        cout << "Child process, enter message: ";
        cin.getline(buf, 256);
        
        sprintf(shmPtr, "%s", buf);
        
        exit(0);
    }
    
    //Parent
    
    int status;
    wait(&status);
    
    cout << "Child exited with status " << status << " and printed the message: ";
    printf("%s\n\n", shmPtr);
    
    int err = munmap((void *)shmPtr, NUM_BYTES);
    if(err == -1) {
        perror("MUNMAP");
        exit(2);
    }
    
    err = shm_unlink(shmName);
    
    if(err == -1) {
        perror(shmName);
        exit(3);
    }
    
    
    return 0;
}