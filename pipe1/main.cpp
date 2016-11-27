#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>

using namespace std;

int sizeOfNumbers(){
    int numberCount = -1;
    ifstream input;
    input.open("/home/valeriy/find_max.dat");
    while (!input.eof()){
        double count;
        input>>count;
        input>>count;
        numberCount++;
    }   
    return numberCount;
}

int main(){
    
    
    int numbers = sizeOfNumbers();
    int numberOfProcesses = 10;
    int workload = numbers / numberOfProcesses;
    int pipedes[numberOfProcesses][2];
    double final[numberOfProcesses];
    
    for (int i = 0; i < numberOfProcesses; i++){
        pipe(pipedes[i]);
    }   
    
    pid_t pid[numberOfProcesses];
    for (int i = 0; i < numberOfProcesses; i++){
        pid[i]=fork();
    }
    
    
    for (int numberOfCurrentProcess = 0; numberOfCurrentProcess < numberOfProcesses; numberOfCurrentProcess++) {
        if (pid[numberOfCurrentProcess] == 0) {
            
            ifstream in;
            in.open("/home/valeriy/find_max.dat");
            double number;
            double maxNumber;
            for (int i = 0; i < numberOfCurrentProcess * workload; i++) {
                in>>number;
                in>>number;
            }
            in>>number;
            in>>maxNumber;
            for (int j = 1; j < workload; j++){
                in>>number;
                in>>number;
                maxNumber = maxNumber < number ? number : maxNumber;
            }
            if (numberOfCurrentProcess == 0){
                in>>number;
                in>>number;
                maxNumber = maxNumber < number ? number : maxNumber;
            }
            
            close(pipedes[numberOfCurrentProcess][0]);
            write(pipedes[numberOfCurrentProcess][1], &maxNumber, sizeof(double));
            close(pipedes[numberOfCurrentProcess][0]);
            exit(0);
            
        } else if (pid[numberOfCurrentProcess] > 0) {
            
            double buff[1];
            close(pipedes[numberOfCurrentProcess][1]);
            read(pipedes[numberOfCurrentProcess][0], buff, sizeof(double));
            final[numberOfCurrentProcess]=buff[0];
            close(pipedes[numberOfCurrentProcess][0]);
        }
    }
    
    double maxNumber = final[0];
    for (int i = 0; i < numberOfProcesses; i++){
        maxNumber = maxNumber < final[i] ? final[i] : maxNumber;
        //cout<<final[i]<<endl;
    }
    
    //cout<<sizeof(final)/sizeof(double);
    
    cout<<"Max number is "<<maxNumber<<endl;
    
    return 0;
}