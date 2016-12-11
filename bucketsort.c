/* 


Distributed Memory Parallel BucketSort 

Authors: Michael Franklin michaelfranklin@sandiego.edu
	 Erin McDonald erinmcdonald@sandiego.edu

*/
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <math.h>

#define min(a,b) (((a) < (b)) ? (a) : (b))

void mergeSortSerial(int l, int r, long * arr, long * temp);
void merge(int l, int lm, int m, int r, long * arr, long * temp);
void validateSerialSort(int array_size, long * array);
void validateParallelSort(int array_size, long * arraySerial, long * arrayParallel);

int main(void) {

//  intialize variables
	int my_rank, comm_size, arr_size, s;
	long * arrSerial, * arrParallel, * temp, * samples, * firstSubArray, * finalSubArray;
	struct timeval tv1, tv2;
	double serialTime, parallelTime;
	MPI_Comm comm;



// Start MPI implementation 
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	comm = MPI_COMM_WORLD;
	setbuf(stdout, NULL);
	printf("MPI stuff initialized my rank is: %d\n", my_rank);
	if(my_rank == 0) {
		printf("Please enter an array size:\n");
		scanf("%d", &arr_size);
		// gets size of the array from standard input
		arrSerial = malloc(sizeof(long)*arr_size);
		arrParallel = malloc(sizeof(long)*arr_size);
		temp = malloc(sizeof(long)*arr_size);
		// allocates space for arrays
		int i;
		srand(2);
		for(i = 0; i < arr_size; i++) {
		        arrSerial[i] = (rand() % 20);
			arrParallel[i] = arrSerial[i];
		}
		// fills arrays with same random numbers
		printf("Unsorted:\n");
		for(i=0; i < arr_size; i++){
			printf("%ld |", arrSerial[i]);
    		}

    		// Sort with serial code
    		gettimeofday(&tv1, NULL); // start serial timing
		mergeSortSerial(0, arr_size - 1, arrSerial, temp);
    		gettimeofday(&tv2, NULL); // stop serial timing
    		serialTime = (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec);
		
		//validate sort
		//validateSerialSort(arr_size, arrSerial);
		printf("\nSerially sorted: \n");
   		for(i=0; i < arr_size; i++){
			printf("%ld |", arrSerial[i]);
    		}

		
		gettimeofday(&tv1, NULL); //start parallel timing


		s = min((10 * comm_size * (log(arr_size)/log(2))), arr_size);//computes the size of the sample array
		printf("\ns is: %d\n", s);
		samples = malloc(sizeof(long)*s);//allocates samples, now that we have the size
		if(s != arr_size) {
		        for(i = 0; i < s; i++) {
		                samples[i] = arrParallel[rand() % arr_size - 1];
		        }//assigns samples random values from arrParallel
			mergeSortSerial(0, s - 1, samples, temp);
		}
		else {
		  for(i = 0; i < s; i++) {
		    samples[i] = arrSerial[i];
		  }
	        }
		printf("Samples Sorted:\n");
		for(i = 0; i < s; i++) {
		  printf("%ld |", samples[i]);
		}
		printf("\n");
		
	}//end if statement for process 0

	MPI_Bcast(arr_size, 1, MPI_INT, 0, comm);
	printf("my rank is: %d\n and arr_size is : %d", my_rank, arr_size);

	//firstSubArray = malloc(sizeof(long)*arr_size/comm_size);
	
	//MPI_Scatter(arrParallel, arr_size, MPI_LONG, subArray, arr_size, MPI_LONG, 0, comm); 
	// block distributes arrParallel

	//TODO implement sorting algorithm

	//if(my_rank == 0) {

	//	gettimeofday(&tv2, NULL); //stop parallel timing
    	//	parallelTime = (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec);
	//
		
	//	validateParallelSort(arr_size, arrSerial, arrParallel);		
		//process 0 validates arrParallel with arrSerial

		// Print results.
    	//	printf("Number of Processes = %d\nArray Size = %d\nSerial time = %e\n", comm_size, arr_size, serialTime);
    	//	printf("Parallel time = %e\n", parallelTime);
    	//	double speedup = serialTime / parallelTime;
    	//	double efficiency = speedup / comm_size;
    	//	printf("Speedup = %e\n", speedup);
    	//	printf("Efficiency = %e\n", efficiency);
	//}//end if statement for process 0

	MPI_Finalize();

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
    int i,j;
    i = l;
    j = l;

    while(l <= lm && m <= r){
	    if(arr[l] <= arr[m]){
	        temp[i] = arr[l];
	        i++;
	        l++;
	    }
	    else{
	        temp[i]=arr[m];
	        i++;
	        m++;
	    }
    }
    while(l <= lm){
	    temp[i] = arr[l];
	    l++;
	    i++;
    }
    while(m <= r){
	    temp[i] = arr[m];
	    m++;
	    i++;
    }

    int k;
    for (k = j; k <= r; k++) {
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
	return;
}



