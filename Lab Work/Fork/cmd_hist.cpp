#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstring>
#include <cstdio>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

#define BS 50

using namespace std;

int main(void) {

    std::vector<string> history;
    
    cout << "Welcome to the cmd session, to quit enter \"quit()\", to view last 10 cmds, enter \"history()\"." << endl;
    
    while(true) {

        cout << "cmd >>";
        string inp;
        getline(cin, inp);

        history.push_back(inp);
        
/*        for (int i = 0; i < history.size(); i++) {
            cout << history[i] << endl;
        }*/

        if (inp == "history()")
        {
            cout << "History:" << endl;
            for (int i = 1; i <=10 && history.size() >= i ; i++)
            {
                cout << history[history.size() - i] << endl;
            }

            continue;
        }

        else if(inp == "quit()") {
            cout << endl << "Quitting..." << endl << "Bye!" << endl;
            break;
        }
        
        else {
            istringstream iss(inp);
            std::vector<string> argvs;
            string buf;
            while(iss >> buf) {
                argvs.push_back(buf);
            }
    
            std::vector<char*> argvp;
            for (int i = 0; i < argvs.size(); i++) {
                char * buf = new char[argvs[i].length()+1];
                strcpy(buf, argvs[i].c_str());
                argvp.push_back(buf);
            }
            argvp.push_back(NULL);
    
    /*        for (int i = 0; i < argvp.size()-1; i++) {
                cout << "argvp[" << i << "] = " << argvp[i] << endl;
            }*/
    
            pid_t pid = fork();
    
            if (pid < 0) {
                cerr << "Error in forking" << endl;
                exit(1);
            }
    
            else if (pid == 0) { //child
                execvp(argvp[0], &argvp[0]);
    
            }
    
            else { //parent
                wait(NULL);
            }
            
            //cout << "FOO BAR" << endl;
        }
    }

    return 0;
}