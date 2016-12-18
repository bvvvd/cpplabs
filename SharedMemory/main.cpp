#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <wait.h>
#include <sys/stat.h>

#define AMOUNT_OF_COLLUMNS 326
#define shmsize 30

using namespace std;

int main(){
    
    double etalon = 0;
    
    fstream etalonIn;
    etalonIn.open("/home/valeriy/find_max.dat");
    for(int i = 0; i < AMOUNT_OF_COLLUMNS * 2; i++){
        double buf;
        etalonIn>>buf;
        etalon+=buf;
    }
    
    cout<<"etalon is "<<etalon<<endl;
    
    int sharedMemoryID;
    char * sharedMemoryPointer;
    sharedMemoryID = shmget(IPC_PRIVATE,shmsize, 
        IPC_CREAT |IPC_EXCL |S_IRUSR|S_IWUSR);
    sharedMemoryPointer = (char *) shmat(sharedMemoryID,0,0);
    sprintf(sharedMemoryPointer,"%g",0.0);
    shmdt(sharedMemoryPointer);
    
    int numberOfProcesses = 20;
    int workload = AMOUNT_OF_COLLUMNS / numberOfProcesses;
    
    int status;
    
    pid_t pid[numberOfProcesses];
    
    
    for (int numberOfCurrentProcess = 0; numberOfCurrentProcess < numberOfProcesses; numberOfCurrentProcess++) {
        if ((pid[numberOfCurrentProcess] = fork()) < 0){                        //in case fork error 
                cout << "Error fork" << endl;
                abort();
            }
        else if (pid[numberOfCurrentProcess] == 0) {
            
            ifstream in;
            in.open("/home/valeriy/find_max.dat");
            double number;
            double sum = 0;
            for (int i = 0; i < numberOfCurrentProcess * workload; i++) {
                in>>number;
                in>>number;
            }
            for (int j = 0; j < workload * 2; j++){
                in>>number;
                sum+=number;
            }
            if (numberOfCurrentProcess == numberOfProcesses - 1)
                for (int i = 0; i < AMOUNT_OF_COLLUMNS % numberOfProcesses; i++) {
                    in>>number;
                    sum += number;
                    in>>number;
                    sum += number;
                }
                    
            sharedMemoryPointer = (char *) shmat(sharedMemoryID,0,0);
            sprintf(sharedMemoryPointer,"%g",atof(sharedMemoryPointer) + sum);
            shmdt(sharedMemoryPointer);
            cout<<"temporary sum is: "<<sum<<endl;
            _exit(0);
            
        } else if (pid[numberOfCurrentProcess] > 0) {
           
            wait(&status);
        }
    }

    sharedMemoryPointer = (char *) shmat(sharedMemoryID,0,0);
    double total = atof(sharedMemoryPointer);
    shmdt(sharedMemoryPointer);
    shmctl(sharedMemoryID, IPC_RMID, 0);
    
    cout<<"The sum of numbers is: "<<total<<endl;
    
    return 0;
}
