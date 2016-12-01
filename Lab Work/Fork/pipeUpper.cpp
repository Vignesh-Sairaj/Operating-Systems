#include <iostream>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <cstdlib>
#include <cstring>
#include <cctype>


using namespace std;



int main(void) {
    
    int pipe_fd[2];
    
    int err = pipe(pipe_fd);
    if( err == -1 ) {
        perror("Pipe error!");
        exit(1);
    }
    
    pid_t pid = fork();
    
    if(pid == -1) {
        perror("Error in forking");
        exit(2);
    }
    
    
    if(pid == 0) { //Child
        close(pipe_fd[0]);
        char buf[256];
        cout << "Child process, enter message: ";
        cin.getline(buf, 256);
        
        write(pipe_fd[1], buf, strlen(buf)+1);
        close(pipe_fd[1]);
        
        exit(0);
    }
    
    //Parent
    
    close(pipe_fd[1]);
    
    char msg[256];
    read(pipe_fd[0], msg, 256);
    
    close(pipe_fd[0]);
    
    cout << "Parent, child sent the message: " << msg << endl;
    
    cout << "Uppercase: " << endl;
    
    for (int i = 0; i < strlen(msg); i++) {
        putchar(toupper(msg[i]));
    }
    

    int status;
    wait(&status);
    
    cout << "\n\nChild exited with status " << status << endl;

    return 0;   
}