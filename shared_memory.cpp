#include "methods.h"

struct sharedMemory* sharedMemoryServer(cell * array, int n )
{
    
    int shmid;
    struct sharedMemory *shm_ptr;
    
    /*Shared memory segment creation*/
    shmid = shmget(key, sizeof(struct sharedMemory), 0666 | IPC_CREAT);
    if (shmid < 0)
        return NULL;
    

    /*Accessing shared memory segment*/
    shm_ptr = (struct sharedMemory *) shmat( shmid, NULL, 0);
    if (((char*)shm_ptr) == (char*) -1)
        return NULL;
    

    return shm_ptr;
}