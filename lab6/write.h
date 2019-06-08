#include"init.h"

void *writer(int *buffer){
    sem_wait(&db);
    *buffer += 1;
    printf ("write ::%d\n", *buffer);
    sem_post(&db);
}
