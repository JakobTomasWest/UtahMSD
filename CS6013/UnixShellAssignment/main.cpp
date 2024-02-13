//
// Created by Jakob West on 2/7/24.
#include <iostream>
#include <unistd.h>
#include "shelpers.hpp"


using namespace std;
int main(int argc, char **argv) {
    Command command;
    string inputLine;
    vector<Command> myCommands;
    cout << ">"; //tells terminal to give a command

    while(true) {

        getline(cin, inputLine);
        if (inputLine == "exit") { // Check for exit command
            break;
        }
        //break apart each of the strings from the input lines into tokens and put them in
        //the vector of commands
        vector<string> tokens = tokenize(inputLine);
        vector<Command> myCommands = getCommands(tokens);

        if (myCommands.empty()) { // Check for parsing errors or empty input
            cout << "Invalid command or no input detected.\n> ";
            continue;
        }
        for (Command &command: myCommands) {
            int pid = fork(); //create an exact copy  of shell
            if (pid < 0) {
                //fork failed
                perror("fork failed\n");
                exit(1);
            }
            if (pid > 0) { //parent
                cout << "PARENT: my child has pid " << pid << "\n";
                wait(nullptr);

                cout << "> ";
                break;
            } else { //parent pid if pid =0

                if (command.inputFd != STDIN_FILENO) {

                    if (dup2(command.inputFd, STDIN_FILENO) == -1) {
                        perror("dup2 failed for inputFd");
                        exit(1);

                    }
                    close(command.inputFd);
                }
                if (command.outputFd != STDOUT_FILENO) {

                    if (dup2(command.inputFd, STDOUT_FILENO) == -1) {
                        perror("dup2 failed for inputFd");
                        exit(1);
                    }
                    close(command.outputFd);

                }
                cout << "Child" << getpid();
                execvp(command.execName.c_str(), const_cast<char *const *>(myCommands[0].argv.data()));

            }
        }

//        return 0;
    }

}
