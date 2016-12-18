#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <limits>
#include <math.h>
#include <iostream>

using namespace std;

int main(){
    
    int K = 1000; //промежутки по t
    int M = 10; //Промежутки по x
    double tau = 1.0 / K; //шаг по t
    double h = 1.0 / M; //шаг по x
    
    pid_t pid;
    double * sharedMemoryPointer;
    int sharedMemoryID;
    
    sharedMemoryID = shmget(IPC_PRIVATE, sizeof(double) * ((K + 1) * (M + 1) + 1), IPC_CREAT | 0666);
    if (sharedMemoryID == -1)
    {
	perror("shmget");
	exit(1);
    }
    pid = fork();
    if (pid == -1)
    {
	perror("fork");
	shmctl(sharedMemoryID, IPC_RMID, 0);//чистим SM 
	exit(1);
    }
    if (pid == 0) //Child
    {
	sharedMemoryPointer = (double*)shmat(sharedMemoryID, NULL, 0);
	int start = 1;
	int finish = M / 2;
	
	while (sharedMemoryPointer[K * (M + 1) + M] != sin(M_PI * K * tau)){
            //родитель присваивает дефолтные значения
        }
	
	for (int i = 0; i < K; i++)
	{
	    for (int j = start; j <= finish; j++)
	    {
		while (isnan(sharedMemoryPointer[i * (M + 1) + j - 1])){ 
                    //в ожидании вычислений соседа
                }
		while (isnan(sharedMemoryPointer[i * (M + 1) + j + 1])){
                    //в ожидании вычислений соседа
                }
		//вычисление значения в точке i+1,j
		sharedMemoryPointer[(i+1)*(M+1) + j] = sharedMemoryPointer[i*(M+1) + j] + tau*( (sharedMemoryPointer[i*(M+1) + j+1] - 2*sharedMemoryPointer[i*(M+1) + j] + sharedMemoryPointer[i*(M+1) + j-1])/(h*h) + M_PI*(M_PI*(i*tau)*(i*tau)*sin(M_PI*i*tau*j*h) + j*h*cos(M_PI*j*h*i*tau)) );
		//рассчёт ошибки
                double error = fabs(sharedMemoryPointer[(i + 1) * (M + 1) + j] - sin(M_PI * i * tau * j * h));
		if (error > sharedMemoryPointer[K * (M + 1) + M + 1])
		    sharedMemoryPointer[K * (M + 1) + M + 1] = error;
	    }
	}
	exit(0);
    }
    else
    {
	sharedMemoryPointer = (double*)shmat(sharedMemoryID, NULL, 0);
	int start = M / 2 + 1;
	int finish = M - 1;
	//запись NaN в SM не имеющих дефолтных значений (пока)
	for (int i = 1; i <= K; i++)
	    for (int j = 1; j < M; j++)
		sharedMemoryPointer[i * (M + 1) + j] = std::numeric_limits<double>::quiet_NaN();
	//запись дефолтных значений тем, кто готов
	for (int j = 0; j <= M; j++)
	    sharedMemoryPointer[j] = 0;
	for (int i = 1; i <= K; i++)
	{
	    sharedMemoryPointer[i * (M + 1)] = 0;
	    sharedMemoryPointer[i * (M + 1) + M] = sin(M_PI * i * tau);
	}
        
        //==========================================
            //maximum error
	sharedMemoryPointer[K * (M + 1) + M + 1] = 0; 
            //maximum error
        //==========================================
	for (int i = 0; i < K; i++)
	{
	    for (int j = start; j <= finish; j++)
	    {
		while (isnan(sharedMemoryPointer[i * (M + 1) + j - 1])){ 
                    //в ожидании вычислений соседа
                }
		while (isnan(sharedMemoryPointer[i * (M + 1) + j + 1])){
                    //в ожидании вычислений соседа
                }
		//рассчёт в точке i+1,j
		sharedMemoryPointer[(i + 1) * (M + 1) + j] = sharedMemoryPointer[i * (M + 1) + j] + tau * ( (sharedMemoryPointer[i * (M + 1) + j + 1] - 2 * sharedMemoryPointer[i * (M + 1) + j] + sharedMemoryPointer[i * (M + 1) + j - 1])/(h * h) + M_PI * (M_PI * (i * tau) * (i * tau) * sin(M_PI * i * tau * j * h) + j * h * cos(M_PI * j * h * i * tau)) );
		//ну ты понел
                double error = fabs( sharedMemoryPointer[(i + 1) * (M + 1) + j] - sin(M_PI * i * tau * j * h) );
		if (error > sharedMemoryPointer[K * (M + 1) + M + 1])
		    sharedMemoryPointer[K * (M + 1) + M + 1] = error;
	    }
	}
	wait(0);//в ожидании дитятки
	cout<<"Error maximum value: "<<sharedMemoryPointer[K * (M + 1) + M + 1]<<endl;
	shmctl(sharedMemoryID, IPC_RMID, 0); //чистка SM
	exit(0);
    }
    return 0;
}