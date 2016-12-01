#include <iostream>
#include <string>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <cstdlib>
#include <cstring>


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
        
        char buf[1024];
        cout << "Child process, enter message: ";
        
        string inp, temp;
        
        do {
            getline(cin, temp);
            inp = inp + temp + "\n";
        } while(temp != "");
        
        sprintf(buf, "%s", inp.c_str());
        
        write(pipe_fd[1], buf, strlen(buf)+1);
        close(pipe_fd[1]);
        
        exit(0);
    }
    
    //Parent
    
    close(pipe_fd[1]);
    
    char msg[1024];
    read(pipe_fd[0], msg, 1024);
    
    close(pipe_fd[0]);
    
    cout << "Parent, child sent the message: " << msg << endl;

    int status;
    wait(&status);
    
    cout << "Child exited with status " << status << endl;
    
    int wc = 0, lc = 0;
    
    for (int i = 0; i < strlen(msg); i++) {
        if(msg[i] == ' ' or msg[i] == '.' or msg[i] == ',' or msg[i] == '\n') {
            wc++;
        }
        if(msg[i] == '\n') {
            lc++;
        }
    }

    cout << "\nCharacter count: " << strlen(msg) << "  Word count: " << wc << "\tLine count: " << lc << endl;

    return 0;   
}