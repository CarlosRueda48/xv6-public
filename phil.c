#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdlib.h>
#include <time.h>

#define NUM_PHIL 5
#define NUM_FORK 5
#define NUM_CHAIR NUM_FORK/2
sem_t forks[NUM_FORK];
sem_t chairs;

int getRand(double max) {
  return 1+max*rand()/RAND_MAX;
}

void eating(void) {
  sleep(getRand(5.0));
}

void thinking(void) {
  sleep(getRand(5.0));
}

void *phil_life(void *threadid) {
  while(1) {
    long tid;
    tid = (long)threadid;
    printf("I am philosopher #%ld and I am thinking.\n", tid);
    thinking();
    sem_wait(&chairs);
    sem_wait(&forks[tid]);
    sem_wait(&forks[(tid+1)%NUM_PHIL]);
    printf("I am philosopher #%ld and I am eating.\n", tid);
    eating();
    sem_post(&forks[tid]);
    sem_post(&forks[(tid+1)%NUM_PHIL]);
  }
}

int main (int argc, char *argv[])
{
  pthread_t philosophers[NUM_PHIL];
  int rc;
  long t;
  srand(time(NULL));

  sem_init(&chairs, 0, NUM_CHAIR);
  for(t=0; t<NUM_FORK; t++)
    sem_init(&forks[t], 0, 1);

  for(t=0; t<NUM_PHIL; t++){
    rc = pthread_create(&philosophers[t], NULL, phil_life, (void *)t);
    if (rc){
      printf("ERROR; return code from pthread_create() is %d\n", rc);
      return -1;
    }
  }

  /* Last thing that main() should do */
  pthread_exit(NULL);
}
