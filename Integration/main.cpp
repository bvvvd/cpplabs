#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#define NUMBER_OF_THREADS 4

using namespace std;

double leftBorder = 1;
double rightBorder = 2;
double step = (rightBorder - leftBorder) / NUMBER_OF_THREADS;
double integral = 0;

double function(double x){
    return sin(29 * x) * cos(-7.153 * x) / sin (-7.153 * x);
    //return x * x;
}

double simpson(long tid){
    double leftBorderOfSegment = leftBorder + step * tid;
    double rightBorderOfSegment = leftBorderOfSegment + step;
    return (rightBorderOfSegment - leftBorderOfSegment) / 6 * (function(leftBorderOfSegment) +
            4 * function((leftBorderOfSegment + rightBorderOfSegment) / 2) + function(rightBorderOfSegment));
}

void *BusyWork(void *t) {
    int i;
    long tid;
    tid = (long) t;
    integral+=simpson(tid);
    pthread_exit((void*) t);
}

int main(int argc, char** argv) {
  
    pthread_t thread[NUMBER_OF_THREADS];
    pthread_attr_t attr;
    int rc;
    long t;
    long taskids[NUMBER_OF_THREADS];
    void *status;
    
    for (t = 0; t < NUMBER_OF_THREADS; t++) {
        taskids[t]=t;
        rc = pthread_create(&thread[t], NULL, BusyWork, (void *) taskids[t]);
        if (rc) {
            exit(-1);
        }
    }
    
    pthread_attr_destroy(&attr);
    for (t = 0; t < NUMBER_OF_THREADS   ; t++) {
        rc = pthread_join(thread[t], &status);
        if (rc) {
            exit(-1);
        }
    }

    cout<<"The result if integration = "<<integral<<endl;
    
    return 0;
}

