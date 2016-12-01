#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

using namespace std;

int main(void) {

    pid_t pid = fork();

    if (pid < 0) {
        cerr << "Error in forking" << endl;
        exit(1);
    }

    else if (pid > 0)
    {
        return 0;
    }

    else {
        execlp("bash", "bash", NULL);
    }

    return 0;
}