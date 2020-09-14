#include "methods.h"

cell * array;
int n; //number of processes.
int M; //number of data in the array.

int main(int argc, char *argv[]) 
{
    for(int i=0; i<argc; i++)
    {
        if(argc>2 && string(argv[i]) == "-n")
          n = atoi(argv[i+1]); //setting number of processes.
        
        else if(argc>2 && string(argv[i]) == "-M")
          M = atoi(argv[i+1]); //setting number of data in the array.
        
         
        //input error checking.
        if (argc < 5)
        {
          perror("Error: Too few arguments");
          exit(1);
        }
        if (argc > 5)
        {
          perror("Error: Too many arguments");
          exit(1);
        }
        if ((M < 3000) && (M > 0))
        {
          perror("M must be more than 3000");
          exit(1);
        }
        
        //SAFE MODE ON
        // if (n <= 0)
        //   n = 5;
        
        // if (M <= 0)
        //   M = 5000;
        
    }



  array = new cell[M];
  arrayInit(array , M);
  
  struct sharedMemory * shm;
  shm = sharedMemoryServer(array, n); //Creating a shared memory segment.
  sharedMemoryInit(shm, array[0].data, n); //Initialized shared memory segment.
  
  //sleep(1);
  Children(n, array, M, shm); //Executing child (read) process. 
  feederProcess(n, array, M, shm); //Executing parent (write) process.
  

  destruction(array, shm); //Destroyes everything.


}