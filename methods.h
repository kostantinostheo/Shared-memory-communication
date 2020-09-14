#ifndef METHODS
#define METHODS

#include <sys/types.h>
#include <stdlib.h> 
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h> 
#include <pthread.h>
#include <sys/wait.h>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>


using namespace std;

#define SHMSZ sizeof(struct sharedMemory)


extern key_t key;//Shared memory access key.


typedef struct cell
{
  int data;
}cell;

 struct sharedMemory
 {
   pthread_mutex_t SHM_MUTEX;
   pthread_cond_t SHM_COND_CAN_WRITE; //Readers alert Writer that a cicle finished.
   pthread_cond_t SHM_COND_CAN_READ;  //Whriter alerts Readers that he wrote something new in the S_M.
   int current_cell;
   int data;
   double smTimeStamp;
   int readersCounter;
  
 };



struct sharedMemory * sharedMemoryServer(cell * array, int n);
struct sharedMemory * sharedMemoryClient();
void smDetach(struct sharedMemory * sm_ptr);
int sharedMemoryInit(struct sharedMemory *p, int fisrtNum, int n);

void arrayInit(cell * ar , int lengtht );
void feederProcess(int n , cell * array , int arrayLength, struct sharedMemory* sc);
double calculateTimestamp(clock_t t);
void destruction(cell * array, struct sharedMemory * shm);

void Children(int n , cell * array , int arrayLengtht, struct sharedMemory* sc);

void childProcess(int n , cell * array , int arrayLengtht, struct sharedMemory* sc);

#endif
