// Erik Gabrielsen - 40354578
// Compiled using GCC on genuse44. Compile with `gcc sleepy.c` then run with `./a.out <arg1>`

#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char *argv[ ]) {
  int count;
  if (argc == 2)
    count = atoi(argv[1]);
  else
    count = 5;

  int pid = fork();
  if (pid == 0) {
    for (int i = 0; i < count; i++) {
      printf("Child Process with PID: %d and PPID: %d tick %d\n", getpid(), getppid(), i);
      sleep(1);
    }
    printf("Child process terminating\n");
    exit(0);
  }

  for (int i = 0; i < count; i++) {
    printf("Original Process with PID: %d and PPID: %d tick %d\n", getpid(), getppid(), i);
    sleep(1);
  }
  printf("Parent process terminating\n");
  wait(NULL);
}

/* Answers to Questions:
Part I
1. Using a simple five-state process model, what states do you think each process goes through during its lifetime?
      Each process hits all five states. For example, the parent process will start at new then move to ready and then running. When it hits a sleep call the process
      will be moved to the blocked state which allows for the child process to move from ready to running. Then it will also be moved to blocked. Then once the 1 second has ticked,
      the parent will move back to ready then running and the cycle will continue until the program terminates and each process will move to the exit state
2. Might this explain why the output is so evenly interleaved between parent and child?
      Yes, this explains why the parent and child seem to complete the same tick evenly because while the parent is sleeping, the child executes,
      then goes into sleep and once the parent wakes up it will be executed and so the cycle continues in that fashion.

Part II
1. Describe what you see using comments in your source code.
      In this part, the parent completes all ticks before the child starts. Then the command line prompt displays before the child process has completed, so it continues to
      display output after the command line prompt.
2. Is there anything unusual about the PPID in the child? Do some research and explain what is happening.
      The PPID in the child process is 1. THe PPID of 1 occurs when any process with children is stopped for some reason before the children complete,
      those children processes get assigned a 1 for PPID

*/
