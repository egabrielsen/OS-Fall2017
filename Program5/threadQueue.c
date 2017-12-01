typedef unsigned int buffer_item;
#define BUFFER_SIZE 5

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
/* The mutex lock */
pthread_mutex_t mutex;

/* the semaphores */
sem_t full, empty;

/* the buffer */
buffer_item buffer[BUFFER_SIZE];

/* buffer counter */
int counter;
int N;

pthread_t tid;       //Thread ID
pthread_attr_t attr; //Set of thread attributes

void *producer(int); /* the producer thread */
void *consumer(int); /* the consumer thread */

void initializeData() {

    srand(time(NULL));

   /* Create the mutex lock */
   pthread_mutex_init(&mutex, NULL);

   /* Create the full semaphore and initialize to 0 */
   sem_init(&full, 0, 0);

   /* Create the empty semaphore and initialize to BUFFER_SIZE */
   sem_init(&empty, 0, BUFFER_SIZE);

   /* Get the default attributes */
   pthread_attr_init(&attr);

   /* init buffer */
   counter = 0;
}

/* Producer Thread */
void *producer(int producer) {
   buffer_item item;
   int total = 0;

   for (int i = 0; i < N/2; i++) {
     item = rand() % 999;

     /* acquire the empty lock */
     sem_wait(&empty);
     /* acquire the mutex lock */
     pthread_mutex_lock(&mutex);

     if(insert_item(item)) {
        fprintf(stderr, " Producer report error condition\n");
     }
     else {
        printf("Producer thread %d produced a %d\n", producer, item);
        total += item;
     }
     /* release the mutex lock */
     pthread_mutex_unlock(&mutex);
     /* signal full */
     sem_post(&full);

     sleep(rand()%2);
   }

   printf("Total produced by producer thread %d = %d\n", producer, total);
}

/* Consumer Thread */
void *consumer(int consumer) {
  buffer_item item;
  int total = 0;

  for (int i = 0; i < N/2; i++) {
    /* acquire the empty lock */
    sem_wait(&empty);
    /* acquire the mutex lock */
    pthread_mutex_lock(&mutex);

    item = remove_item();
    if (item == -1) {
       fprintf(stderr, " Consumer report error condition\n");
    }
    else {
       printf("Consumer thread %d consumed a %d\n", consumer, item);
       total += item;
    }
    /* release the mutex lock */
    pthread_mutex_unlock(&mutex);
    /* signal full */
    sem_post(&full);

    sleep(rand()%2);
  }

  printf("Total consumed by consumer thread %d = %d\n", consumer, total);
}

/* Add an item to the buffer */
int insert_item(buffer_item item) {
   /* When the buffer is not full add the item
      and increment the counter*/
   int index = counter % BUFFER_SIZE;

   buffer[index] = item;
   counter++;
   return 0;

}

/* Remove an item from the buffer */
int remove_item() {
   /* When the buffer is not empty remove the item
      and decrement the counter */
   if(counter > 0) {
      int index = counter % BUFFER_SIZE;
      counter--;
      return buffer[index-1];
   }
   else { /* Error buffer empty */
      return -1;
   }
}

int main(int argc, char *argv[]) {
   /* Loop counter */
   int i;

   /* Verify the correct number of arguments were passed in */
   if (argc >  2) {
      fprintf(stderr, "USAGE:./a.out <INT>\n");
   } else {
     if (argc == 2)
       N = atoi(argv[1]);
     else
       N = 10;
   }

   int mainSleepTime = 1; /* Time in seconds for main to sleep */
   int numProd = 2; /* Number of producer threads */
   int numCons = 2; /* Number of consumer threads */

   pthread_t* producers;
   pthread_t* consumers;

   producers = malloc(sizeof(pthread_t)*numProd);
   consumers = malloc(sizeof(pthread_t)*numCons);

   /* Initialize the app */
   initializeData();

   /* Create the producer threads */
   for(i = 0; i < numProd; i++) {
      pthread_create(&producers[i],&attr,producer,i);
    }

   /* Create the consumer threads */
   for(i = 0; i < numCons; i++) {
      pthread_create(&consumers[i],&attr,consumer,i);
   }

   for(i = 0; i < numProd; i++) {
      pthread_join(producers[i], NULL);
   }

   for(i = 0; i < numCons; i++) {
      pthread_join(consumers[i], NULL);
   }

   wait(NULL);

   /* Exit the program */
   exit(0);
}
