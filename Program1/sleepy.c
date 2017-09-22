// Erik Gabrielsen - 40354578
// Compiled using GCC on genuse44. Compile with `gcc sleepy.c` then run with `./a.out <arg1>`

#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[ ]) {
  int count;
  if (argc == 2)
    count = atoi(argv[1]);
  else
    count = 5;

  for (int i = 0; i < count; i++) {
    if (i == 0)
      printf("%d - Start\n", getpid());
    else
      printf("%d - Tick %d\n", getpid(), i);
    sleep(1);
  }
}

/* Answers to Questions:
1.  The Process Id differs after each run, but each tick has the same process Id.
2.  The Process reverts to the sleep state (S)
*/
