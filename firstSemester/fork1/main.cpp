#include <cstdlib>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <wait.h>

using namespace std;

int main(){
    
    pid_t pid1 = fork();
    
    if(pid1 == -1){
        cout<<"fork() failed"<<endl;
        exit(1);
    }else if(pid1 == 0){
        cout<<"Child process, my pid = "<<getpid()<<", my parent process' pid ="<<getppid()<<endl; 
        exit(0);
    }else{
        wait(0);
        cout<<"Parent process, my pid = "<<getpid()<<", my child process' pid ="<<pid1<<endl;
        exit(0);
    }    
    
    return 0;
}