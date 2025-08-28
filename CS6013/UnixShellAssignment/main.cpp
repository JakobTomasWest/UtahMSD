//
// Created by Student: Jakob Tomas West u0675702 on 2/7/24.
//
#include <iostream>
#include <unistd.h>
#include "shelpers.hpp"
#include <cstdlib>
#include <stdio.h>
#include <readline/readline.h>

const char* commandlist[] = {"cd","ls", "NULL"};
char* command_generator(const char* text, int state) {
    static int list_index, len;
    const char* name;

    if (!state) {
        list_index = 0;
        len = strlen(text);
    }

    while ((name = commandlist[list_index++])) {
        if (strncmp(name, text, len) == 0) {
            return strdup(name);
        }
    }

    return NULL; // No more matches
}
char ** character_name_completion(const char *text, int start, int end)
{
    if (start == 0) {
        return rl_completion_matches(text, command_generator);
    }
    return NULL;
}

using namespace std;
int main(int argc, char **argv) {
    Command command;

    vector<Command> myCommands;
//    cout << ">"; //tells terminal to give a command
    rl_attempted_completion_function = character_name_completion;
    while(true) {
        char* input = readline("> ");
        if (strlen(input) > 0) {
            add_history(input);
        }
            string inputLine(input);
            free(input);


//        getline(cin, inputLine);
        if (inputLine == "exit") { // Check for exit command
            break;
        }
        //break apart each of the strings from the input lines into tokens and put them in
        //the vector of commands
        vector<string> tokens = tokenize(inputLine);
        vector<Command> myCommands = getCommands(tokens);

        int pipefd[2];
        int inputFd = STDIN_FILENO; // Initial input for the first command
        size_t commandCount = 0;

        //if the array of tokens/string are not empty and the first string is cd we can either direct to home or change path
        if(!tokens.empty() && tokens[0] == "cd"){
            if(tokens.size() ==1){
                const char* home = getenv("HOME");
                if (home) {
                    if (chdir(home) != 0) {
                        perror("cd");
                    }
                }
                //direct to the specified path if we write more than "cd"
            } else {
                // cd with one parameter, change to the specified directory
                if (chdir(tokens[1].c_str()) != 0) {
                    perror("cd");
                }
            }
            continue; // Move to the next iteration of the loop
        }
        if (myCommands.empty()) { // Check for parsing errors or empty input
            cout << "Invalid command or no input detected.\n> ";
            continue;
        }
        for (Command &command: myCommands) {

            // Create a pipe for all but the last command
            if (commandCount < myCommands.size() - 1) {
                if (pipe(pipefd) == -1) {
                    perror("pipe");
                    exit(EXIT_FAILURE);
                }
            }

            int pid = fork(); //create an exact copy  of shell
            if (pid < 0) {
                //fork failed
                perror("fork failed\n");
                exit(1);
            }
            if (pid > 0) { //parent
                cout << "PARENT: my child has pid " << pid << "\n";
                waitpid(pid, nullptr, 0); // Wait for the child to finish
                cout << "> ";
                //p
                // if inputFd isn't [0]
                if (inputFd != STDIN_FILENO) { //this condition is true if the current command's input comes from a pipe rather than the standard input.
                    close(inputFd); //close the old inputFd if it was a pipe
                }
                //as long as we aren't on the last command, pipe it
                if (commandCount < myCommands.size() - 1) {
                    //assign the read end of the pipefd[0] for (to) the next commands input
                    inputFd = pipefd[0];
                    close(pipefd[1]); //close the write end in the parent
                }
                commandCount++;
            } else { //parent pid if pid =0
                //
                //when we get our input from another commands output... not fd[0]
                if (inputFd != STDIN_FILENO) {
                    //read the current input from the output of the previous command write end using the pipe
                    dup2(inputFd, STDIN_FILENO);
                    //close extra inputFd because we have the duped inputfd in teh table now
                    close(inputFd);
                }
                if (commandCount < myCommands.size() - 1) {
                    close(pipefd[0]); //close the read end in the child
                    //redirecting the standard output of the process to the pipe.
                    dup2(pipefd[1], STDOUT_FILENO);
                    //close childs write cause pipe read/write is established
                    close(pipefd[1]);
                }

                execvp(command.execName.c_str(), const_cast<char* const*>(command.argv.data()));
                perror("execvp failed");
                exit(1);
            }

        }
        if (inputFd != STDIN_FILENO) {
            close(inputFd); // Close the last inputFd if it was a pipe
        }
//        return 0;
    }

}


