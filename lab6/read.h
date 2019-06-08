#include"init.h"

void *reader(int *buffer){
    sem_wait(&rc);
    if(readcount == 0) {
        sem_wait(&db);
        readcount++;
    }
    printf("\nReader Inside..%d\n", *buffer);
    sem_post(&rc);
    sem_wait(&rc);
    readcount--;
    if(readcount == 0) {
        sem_post(&db);
    } 
    sem_post(&rc);
}
