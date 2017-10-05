
 #include <pthread.h>
 #include <stdio.h>
 #include <unistd.h>
 #include <semaphore.h>
 #define NUM_THREADS 10

 long total;
 sem_t mutex;
 void *PrintHello(void *threadid)
 {
   long tid, ltotal;
  tid = (long)threadid;
  printf("Hello World! It's me, thread #%ld!\n", tid);
  sem_wait(&mutex);
  ltotal = total;
  sleep(1); // Forcing CPU losing
  ltotal++;
  total = ltotal;
  sem_post(&mutex);
  pthread_exit(NULL);
}

int main (int argc, char *argv[])
{
  pthread_t threads[NUM_THREADS];
  int rc;
  long t;

  total = 0;
  sem_init(&mutex, 1, 1);

  for(t=0; t<NUM_THREADS; t++){
    printf("In main: creating thread %ld\n", t);
    rc = pthread_create(&threads[t], NULL, PrintHello, (void *)t);
    if (rc){
      printf("ERROR; return code from pthread_create() is %d\n", rc);
      return -1;
    }
  }

  for(t=0; t<NUM_THREADS; t++){
    pthread_join(threads[t], NULL);
  }

  printf("Total value is: %ld.\n", total);
  /* Last thing that main() should do */
  pthread_exit(NULL);
}
