#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <time.h>
#include <pthread.h>

/* -- Answers to Questions
  1) I expected that the pthread implementation would work faster on large data sets
  because of the ability to do the task in parrallel. I assume that spawning the threads will require a bit of overhead

  2) The results were pretty surprising. It turned out that the Sequencial mege sort
   for the two smaller arrays and then merging the two together rather without pthreads was the fastest.
   This may have something to do with how I implemented the mergeArray() function so that it would run in O(2n).
   The overhead of spawning threads must be too big for these computations for them to be of any advantage in terms of sorting.


*/

void *sort(int array[]);
void merge(int *A,int *L,int leftCount,int *R,int rightCount);
void mergeSort(int *A,int n);
void mergeArrays(int arr1[], int arr2[], int n1, int n2, int arr3[]);
void printContents(int array1[], int array2[], int n);

int n = 20;

int main(int argc, char *argv[ ]) {
  time_t t;

  /* Intializes random number generator */
  srand((unsigned) time(&t));

  if (argc == 2)
    n = atoi(argv[1]);

  //  Initialize array
  int array1[n];
  int array2[n];
  int copy_array1[n];
  int copy_array2[n];
  int double_array[2 * n];
  int mergedArray[n + n];

  for (int i = 0; i < n; i++) {
      array1[i] = rand() % n;
      array2[i] = rand() % n;
      copy_array1[i] = array1[i];
      copy_array2[i] = array2[i];
      double_array[i] = array1[i];
      double_array[n + i] = array2[i];
  }
  pthread_t thread1, thread2;

  int  iret1, iret2;

  clock_t start, end;
  double cpu_time_used;
  start = clock();

  iret1 = pthread_create( &thread1, NULL, sort, array1);
  if(iret1) {
    fprintf(stderr,"Error - pthread_create() return code: %d\n",iret1);
    exit(EXIT_FAILURE);
  }

  iret2 = pthread_create( &thread2, NULL, sort, array2);
  if(iret2) {
    fprintf(stderr,"Error - pthread_create() return code: %d\n",iret2);
    exit(EXIT_FAILURE);
  }

  pthread_join( thread1, NULL);
  pthread_join( thread2, NULL);


  mergeArrays(array1, array2, n, n, mergedArray);
  end = clock();
  cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
  printf("Merge with p threads: %f\n", cpu_time_used);

  start = clock();
  mergeSort(copy_array1, n);
  mergeSort(copy_array2, n);
  mergeArrays(copy_array1, copy_array2, n, n, mergedArray);
  end = clock();
  cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
  printf("Sequencial Merge sort then merge arrays: %f\n", cpu_time_used);


  start = clock();
  mergeSort(double_array, 2 * n);
  end = clock();
  cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
  printf("Sort 2-n array: %f\n", cpu_time_used);


  exit(EXIT_SUCCESS);
}

void *sort( int array[]) {
  mergeSort(array, n);
}

// -- adapted from https://gist.github.com/mycodeschool/9678029
void merge(int *A,int *L,int leftCount,int *R,int rightCount) {
	int i,j,k;

	// i - to mark the index of left aubarray (L)
	// j - to mark the index of right sub-raay (R)
	// k - to mark the index of merged subarray (A)
	i = 0; j = 0; k =0;

	while(i<leftCount && j< rightCount) {
		if(L[i]  < R[j]) A[k++] = L[i++];
		else A[k++] = R[j++];
	}
	while(i < leftCount) A[k++] = L[i++];
	while(j < rightCount) A[k++] = R[j++];
}

// Recursive function to sort an array of integers.
void mergeSort(int *A,int n) {
	int mid,i, *L, *R;
	if(n < 2) return; // base condition. If the array has less than two element, do nothing.

	mid = n/2;  // find the mid index.

	// create left and right subarrays
	// mid elements (from index 0 till mid-1) should be part of left sub-array
	// and (n-mid) elements (from mid to n-1) will be part of right sub-array
	L = (int*)malloc(mid*sizeof(int));
	R = (int*)malloc((n- mid)*sizeof(int));

	for(i = 0;i<mid;i++) L[i] = A[i]; // creating left subarray
	for(i = mid;i<n;i++) R[i-mid] = A[i]; // creating right subarray

	mergeSort(L,mid);  // sorting the left subarray
	mergeSort(R,n-mid);  // sorting the right subarray
	merge(A,L,mid,R,n-mid);  // Merging L and R into A as sorted list.
        free(L);
        free(R);
}

void mergeArrays(int arr1[], int arr2[], int n1, int n2, int arr3[]) {
  int i = 0, j = 0, k = 0;

  // Traverse both array
  while (i<n1 && j <n2)
  {
      // Check if current element of first
      // array is smaller than current element
      // of second array. If yes, store first
      // array element and increment first array
      // index. Otherwise do same with second array
      if (arr1[i] < arr2[j])
          arr3[k++] = arr1[i++];
      else
          arr3[k++] = arr2[j++];
  }

  // Store remaining elements of first array
  while (i < n1)
      arr3[k++] = arr1[i++];

  // Store remaining elements of second array
  while (j < n2)
      arr3[k++] = arr2[j++];
}

void printContents(int array1[], int array2[], int n) {
  for (int i = 0; i < n; i++) {
      printf("%d - ", array1[i]);
      printf("%d \n", array2[i]);
  }
}
