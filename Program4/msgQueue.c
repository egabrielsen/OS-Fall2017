#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <string.h>
#include <pthread.h>

void *consumer(int);

int N;
int msqid;
/* message structure */
struct message {
    long mtype;
    char mtext[8192];
};

int main(int argc, char *argv[ ]) {
  if (argc == 2)
    N = atoi(argv[1]);
  else
    N = 10; // default value of N

  msqid = msgget(IPC_PRIVATE, IPC_CREAT | 0600);
  if (msqid == -1) {
      perror("msgget");
      return EXIT_FAILURE;
  }


  int pid = fork();
  if (pid == 0) {
    pthread_t thread1, thread2, thread3;

    int  iret1, iret2, iret3;


    iret1 = pthread_create( &thread1, NULL, consumer, 1);
    if(iret1) {
      fprintf(stderr,"Error - pthread_create() return code: %d\n",iret1);
      exit(EXIT_FAILURE);
    }

    iret2= pthread_create( &thread2, NULL, consumer, 2);
    if(iret2) {
      fprintf(stderr,"Error - pthread_create() return code: %d\n",iret2);
      exit(EXIT_FAILURE);
    }


    iret3 = pthread_create( &thread3, NULL, consumer, 3);
    if(iret3) {
      fprintf(stderr,"Error - pthread_create() return code: %d\n",iret3);
      exit(EXIT_FAILURE);
    }


    pthread_join( thread1, NULL);
    pthread_join( thread2, NULL);
    pthread_join( thread3, NULL);


    exit(0);
  }

  int sum = 0;
  for (int i = 0; i < N; i++) {
    int R = rand() % 999;
    sum += R;

    // create message
    char rString[5];
    sprintf(rString,"%d",R);

    struct message message;
    message.mtype = 1;
    memset(&(message.mtext), 0, 8192 * sizeof(char));
    (void)strcpy(message.mtext, rString);

    /* send message to queue */
    if (msgsnd(msqid, &message, sizeof(long) + (strlen(message.mtext) * sizeof(char)) + 1, 0) == -1) {
        perror("msgsnd");

        return EXIT_FAILURE;
    }


    printf("Producer produced a R: %d\n", R);
    sleep(rand()%2);
  }

  if (msgctl(msqid, IPC_RMID, NULL) == -1) {
      perror("msgctl");

      return EXIT_FAILURE;
  }

  printf("Total Produced = %d\n", sum);

  wait(NULL);
}

void* consumer(int consumer) {
  int sum = 0;
  for (int i = 0; i < N / 3; i++) {
    struct message message;
    if (msgrcv(msqid, &message, 8192, 0, 0) == -1) {
        perror("msgrcv");

        return EXIT_FAILURE;
    } else {
      int value = atoi(message.mtext); // read value here from queue
      sum += value;
      printf("Consumer thread %d consumed a %d\n", consumer, value);
    }

    sleep((rand()%3)+1);
  }

  printf("Total consumed by consumer thread %d = %d\n", consumer, sum);
}


/* Answers
  Mutual exclusion is accomplished by having a shared queue between all of the threads.
  Each thread pulls a message off the queue so that another thread cannot get the same message.
  This is synchronized by having a sleep between when a message is given and received.

*/
