/* BucketSort 

Authors: Michael Franklin michaelfranklin@sandiego.edu
	 Erin McDonald erinmcdonald@sandiego.edu

*/
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

void mergeSortSerial(int l, int r, long * arr, long * temp);
void merge(int l, int lm, int m, int r, long * arr, long * temp);
void validateSerialSort(int array_size, long * array);
void validateParallelSort(int array_size, long * arraySerial, long * arrayParallel)

int main(void) {

//  intialize variables
	int my_rank, comm_size, arr_size;
	long * arrSerial, * arrParallel, * temp;
// Start MPI implementation 
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	
	if(my_rank == 0) {
		printf("Please enter an array size:");
		arr_size = 0;
		scanf("%d", arr_size);
		// gets size of the array from standard input
		arrSerial = malloc(sizeof(long)*arr_size);
		arrParallel = malloc(sizeof(long)*arr_size);
		temp = malloc(sizeof(long)*arr_size);
		// allocates space for arrays
		int i;
		srand(0);
		for(i = 0; i < arr_size; i++) {
			arrSerial[i] = rand();
			arrParallel[i] = arrSerial[i];
		}
		// fills arrays with same random numbers
		
		// For timing
   		struct timeval  tv1, tv2;

    		// Sort with serial code
    		gettimeofday(&tv1, NULL); // start serial timing
		mergeSortSerial(0, arr_size - 1, arrSerial, temp);
    		gettimeofday(&tv2, NULL); // stop serial timing
    		double serialTime = (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec);
		
		//validate sort
		validateSerialSort(arr_size, arrSerial);
		
		gettimeofday(&tv1, NULL); //start parallel timing


		//TODO block distribute arrParallel
	}//end if statement for process 0

	//TODO implement sorting algorithm

	if(my_rank == 0) {

		gettimeofday(&tv2, NULL); //stop parallel timing
    		double parallelTime = (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec);

		
		validateParallelSort(arr_size, arrSerial, arrParallel);		
		//process 0 validates arrParallel with arrSerial

		// Print results.
    		printf("Number of Processes = %d\nArray Size = %d\nSerial time = %e\n", threadCount, arr_size, serialTime);
    		printf("Parallel time = %e\n", parallelTime);
    		double speedup = serialTime / parallelTime;
    		double efficiency = speedup / threadCount;
    		printf("Speedup = %e\n", speedup);
    		printf("Efficiency = %e\n", efficiency);
	}

	return 0;
}

void mergeSortSerial(int l, int r, long * arr, long * temp){
    int m;
    if(r>l){
        m=(r+l)/2 +1;
        mergeSortSerial(l, m-1, arr, temp);
        mergeSortSerial(m,r, arr, temp);
        merge(l,m-1,m,r, arr, temp);
    }

    return;
} /* mergeSortSerial */

void merge(int l, int lm, int m, int r, long * arr, long * temp){
    while(l <= lm && m <= r){
            if(arr[l] <= arr[m]){
                temp[m] = arr[l];
                m++;
                l++;
            }
            else{
                temp[l]=arr[m];
                l++;
                m++;
            }
    }
    while(l <= lm){
            temp[m] = arr[l];
            l++;
            m++;
    }
    while(m <= r){
            temp[l] = arr[m];
            m++;
            l++;
    }

    int k;
    for (k = 0; k <= r; k++) {
        arr[k] = temp[k];
    }
    return;

} /* merge */

void validateSerialSort(int array_size, long * array) {
	int i;
	for(i = 0; i < array_size - 2; i++) {
		if(array[i] > array[i+1]) {
			printf("Array is not sorted correctly");
			return;
		}
	}
	printf("Array is sorted");
	return;
}

void validateParallelSort(int array_size, long * arraySerial, long * arrayParallel) {
	int i;
	for(i = 0; i < array_size; i++) {
		if(arraySerial[i] != arrayParallel[i]) {
			printf("Array is not sorted correctly");
			return;
		}
	}
	printf("Sorted parallel array matches sorted serial array");
	return;
}



