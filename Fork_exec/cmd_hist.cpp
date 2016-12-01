#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

#define BS 50

using namespace std;

int main(void) {

    std::vector<string> history;
    
    while(true) {

        cout << "cmd >>";
        string inp;
        getline(cin, inp);

        history.push_back(inp);

        if (inp == "history")
        {
            cout << "History:" << endl;
            for (int i = 1; i <=10 && (history.size() - i) >= 0 ; ++i)
            {
                cout << history[history.size() - i] << endl;
            }

            continue;
        }


        istringstream iss(inp);
        std::vector<char*> argvp;
        while(iss) {
            char buf[BS];
            iss >> buf;
            cout << "buf = " << buf << endl;
            argvp.push_back(buf);
        }
        argvp.push_back(NULL);


        pid_t pid = fork();

        if (pid < 0) {
            cerr << "Error in forking" << endl;
            exit(1);
        }

        else if (pid == 0) { //child
            //execvp(argvp[0], &argvp[0]);

            cout << "argvp[0] = " << argvp[0] << endl;
            cout << "argvp[1] = " << argvp[1] << endl;

            char* arg[] = {"lp", NULL};
            execvp("ls", arg);
        }

        else { //parent
            wait(NULL);
        }
    }

    return 0;
}