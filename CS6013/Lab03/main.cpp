// Author: jakob tomas west
// Monday, February 5th 2024
// Lab03


#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string>
using namespace std;
void pipe_example(string basicString);

#

int main(int argc, char *argv[]) {

//    if (argc ==2); // then we are all good

    //char* msg = argv[1]; 'this is the message'
    //int len = strlen(msg); "give me a char* and ill tell you how long it is"

    string msg = argv[1];
    int length = msg.length();
    //cout << "The length of your message is: " << length;
    errno = EFAULT;
    perror("error is:");
    pipe_example(msg);
    return 0;
}

void pipe_example(string msg)
{

    //Must "create" the fds( for the pipe) before the fork... so that both the parent and child
    //will have access to them
    int fds[2] = { -1, -1};
    int rc = pipe(fds); //paramas are file descriptors

    int readFd = fds[0];  //std in
    int writeFd = fds[1]; //std out
    if( rc!= 0){
        perror("pipe failed!");
        exit(1);
    }

    //the return code is a pid
//#if 0
    int pid = fork(); //create an exact copy
    if(pid <0){
    //fork failed
        perror("fork failed\n");
        exit(1);
    }
//#end if
    cout << "fds: " << fds[0] << ", " << fds[1] << "\n";
    //printf("fds: &d, %d\n", fds[0], fds[1);
//#if 0
//#endif
    //Parent
    if (pid >0){
        close (readFd);
        cout << "PARENT: my child has pid " << pid << "\n";
//        while(true)
//        {
            int num; // could so int*num = malloc(4) // int num[1]
//            cout << "Enter a number: ";
//            cin >> num;
            string newMessage = msg;
            cout << "The message insidfe teh function is : " << msg << "\n";
//            char* message;
//            message = ;


            int length = (newMessage.length()) +1; // +1 for null terminator
            //4 bytes is an int   ///give the address of num, make it a pointer to num?
            write( writeFd, &newMessage, length);

//        }
        wait(NULL);
        cout << "PARENT: done waiting for child, exiting now";

    }else { //pid ==0
        //readFD, WriteFd // dont have to reopen readFD because theyre different processes
        close (writeFd);
        cout << "CHILD: my pid is " << getpid() << "\n";
        cout << "CHILD: waiting for data from parent...\n";
        int* data;
        //reading a bunch of characters instead
        char message[1024];
        read(readFd, message, sizeof(message));

//        char message[size];
//        char* message = reinterpret_cast<char>(malloc(1024));

//        while(true){
//            read(readFd, data, sizeof(int)); //2* sizeof(int)
           // read(readFd, &message, size);
            cout <<"Received teh data: " << message << "\n";
            // data[0], data[1]
//        }

    }


}
    //g++ -std=c++17 main.cpp -o Myexec

//    printf("hello (pid:%d)\n", (int) getpid());
//    int rc = fork();
//    if (rc < 0) {
//        // fork failed
//        fprintf(stderr, "fork failed\n");
//        exit(1);
//    } else if (rc == 0) {
//        // child (new process)
//        printf("child (pid:%d)\n", (int) getpid());
//    } else {
//        int rc_wait = wait(nullptr);
//        // parent goes down this path (main)
//        printf("parent of %d (pid:%d)\n",
//               rc, rc_wait, (int) getpid());
//    }
