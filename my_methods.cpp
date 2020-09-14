#include "methods.h"

key_t key = 5023;

int sharedMemoryInit(struct sharedMemory *p, int fisrtNum, int n)
{
    clock_t t = clock();
    int check; //temporary, just for chacking if inits were succesfull.

    pthread_mutexattr_t mutex_attr;
    pthread_condattr_t cond_attr;

    //Initialized shared memory for the first time.
    p->readersCounter = n;
    p->data = fisrtNum;
    p->current_cell = 0;
    p->smTimeStamp = t;
    
    
    //Making mutex executable throught processes.
    check = pthread_mutexattr_init(&mutex_attr);
    if (check != 0)
        return -1;
    
    check = pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_SHARED);
    if (check != 0)
        return -1;

    pthread_mutex_init(&(p->SHM_MUTEX), &mutex_attr);
    
    
    //Making cond vars executable throught processes.
    check = pthread_condattr_init(&cond_attr);
    if (check != 0)
        return -1;
  
    check = pthread_condattr_setpshared(&cond_attr, PTHREAD_PROCESS_SHARED);
    if (check != 0)
        return -1;

    check = pthread_cond_init(&(p->SHM_COND_CAN_READ), &cond_attr);
    if (check != 0)
        return -1;
  
    check = pthread_cond_init(&(p->SHM_COND_CAN_WRITE), &cond_attr);
    if (check != 0)
        return -1;
    
    //Destroy conditional variable attributes.
    check = pthread_condattr_destroy(&cond_attr);
    if (check != 0)
        return -1;

    check = pthread_mutexattr_destroy(&mutex_attr);
    if (check != 0)
        return -1;
    
    return 0;
}

double calculateTimestamp(clock_t t)
{
    double tmp;
    tmp = (double)(t)/CLOCKS_PER_SEC; //Converts clock() to seconds.
    return tmp;
}

void arrayInit(cell * ar , int lenght)
{
  //Initialized array numbers.
  for (int i = 0; i < lenght; i++)
  {
    ar[i].data = rand()%1000;
  }
  
}

//Forking
void Child(int n , cell * array , int arrayLength, struct sharedMemory* sc )
{
    int fid = fork();
        
    if(fid < 0)
        perror("Error: process failed");      
        
    if(fid == 0)
    {
        childProcess(n, array, arrayLength, sc);
        exit(0);
    }
} 

//Creating multiple processes.
void Children(int n , cell * array , int arrayLength , struct sharedMemory* sc)
{
  for (int i = 0; i < n; i++)
  {
    Child(n, array, arrayLength, sc);
  }
  
}

//Writer. Writes to shared memory once every cicle.
void feederProcess(int n , cell * array , int arrayLength, struct sharedMemory* sc)
{
  clock_t _CPU_FEEDER ; 
  for (int i = 1; i < arrayLength; i++)
  {
    
    pthread_mutex_lock(&(sc->SHM_MUTEX));
    while (sc->readersCounter != 0)
    {
      //cout << "here DEBUG" << endl;
      pthread_cond_wait(&(sc->SHM_COND_CAN_WRITE), &(sc->SHM_MUTEX));
    }

    sc->readersCounter = n;
    sc->data = array[i].data;    

    _CPU_FEEDER = clock();
    sc->smTimeStamp = _CPU_FEEDER;
    sc->current_cell ++;

    //wake up readers..
    pthread_cond_broadcast(&(sc->SHM_COND_CAN_READ));

    pthread_mutex_unlock(&(sc->SHM_MUTEX));

    
  }
  
  //Wait for children to finish execution.
  for (int i = 0; i < n; i++) 
  {
    wait(NULL);
  }

}

void destruction(cell * array, struct sharedMemory * shm)
{
  /*deallocate array*/
  delete[] array;

  /*destroying cond_vars and mutexes from the shared memory*/
  pthread_cond_destroy(&(shm->SHM_COND_CAN_READ));
  pthread_cond_destroy(&(shm->SHM_COND_CAN_WRITE));
  pthread_mutex_destroy(&(shm->SHM_MUTEX));
}
