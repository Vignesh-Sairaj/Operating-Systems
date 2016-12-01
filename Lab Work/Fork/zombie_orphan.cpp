#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

#define orphan 0

using namespace std;

int main(void) {
    
    pid_t pid = fork();

    if (pid < 0) {
        cerr << "Error in forking" << endl;
        exit(1);
    }

    else if (pid == 0) { //child
        
        if(orphan) {
            sleep(10);
            cout << "Orphan: child pid = " << getpid() << "; parent pid = " << getppid() << endl;
        }

        else { //zombie
            return 0;
        }
    }

    else { //parent
        if (orphan) {
            return 0;
        }

        else { // Zombie
            sleep(5);
            int status;
            pid_t chPid = wait(&status);

            cout << "Zombie: parent pid = " << getpid() << " waited for child, pid = " << chPid << " with status: " << status << endl;
        }

    }

    return 0;
}