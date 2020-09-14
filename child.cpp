#include "methods.h"

void write_text_to_file(cell * array , int size , double avg )
{   

    ofstream out("FinalProcessStats.txt");
    if (out)
    {
        out << "Final process stats." << endl;
        out << "Process ID: " << getpid() << endl;
        out << "Average time: " << avg << " ms" << endl;

        out << " " << endl;

        out << "Printing " << size << " integers from the array." << endl;
        out << "------------------------------------------------" << endl;

        for (int k = 0; k < size; k++)
        {
            out << array[k].data << endl;
        }

        out.close();
    }

    

}

//Reader
void childProcess(int n , cell * array , int arrayLength, struct sharedMemory * sc)
{
  
  //struct sharedMemory * sc = sharedMemoryClient();
  double timeWait = 0.0;
  double sum = 0.0;
  double processTimeStamp = 0.0;
  double avg = 0.0;
  clock_t _CPU_TIME;
  bool isLastProcess;
  
  
  for (int i = 0; i < arrayLength; i++)
  {
    
    pthread_mutex_lock(&(sc->SHM_MUTEX));
    isLastProcess = false;
    while (sc->current_cell != i || sc->readersCounter == 0) //
    {
      pthread_cond_wait(&(sc->SHM_COND_CAN_READ), &(sc->SHM_MUTEX));
      //cout << "stuck" << endl;
    }
    pthread_mutex_unlock(&(sc->SHM_MUTEX));


    //math from here.
    pthread_mutex_lock(&(sc->SHM_MUTEX));
    _CPU_TIME = clock(); //current proccess time.
    processTimeStamp = _CPU_TIME; 
    timeWait = sc->smTimeStamp - processTimeStamp;
    double processDelay = calculateTimestamp(timeWait);
    pthread_mutex_unlock(&(sc->SHM_MUTEX));

    sum = sum + processDelay;
    if (i > 0)
    {
      avg = sum/i;
    }
    //to here.

    pthread_mutex_lock(&(sc->SHM_MUTEX));
    sc->readersCounter--;  
    if(sc->readersCounter == 0)
    {
      pthread_cond_signal(&(sc->SHM_COND_CAN_WRITE)); 
      isLastProcess = true;
    }
    pthread_mutex_unlock(&(sc->SHM_MUTEX));
    
  }
  
  pthread_mutex_lock(&(sc->SHM_MUTEX));

  double milli = avg * pow(10,3);
  cout << fixed << "Process with ID: " << getpid() << " has average time:  " << milli << " ms" << endl;
  if (isLastProcess)
  {
    write_text_to_file(array,arrayLength, milli);
    shmdt(sc); //Detaching shared memory segment after last process finish execution.
  }
  pthread_mutex_unlock(&(sc->SHM_MUTEX));
  
}
