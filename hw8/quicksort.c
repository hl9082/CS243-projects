/**
 * @file quicksort.c
 * @brief We implement both unthreaded and threaded quicksort algorithms, and from here, we compare their performance.
 * @author Huy Le (hl9082)
 */

#define _DEFAULT_SOURCE   // for usleep and syscall

#include<stdio.h> ///for error handling functions
#include<stdlib.h> ///standard library
#include<pthread.h> ///for threads handling
#include<time.h>///for clock(), clock_t, and CLOCKS_PER_SECOND
#include<ctype.h> ///for size_t
#include<string.h>///for string operations




int thread_count = 0;// Global counter to track the number of threads

pthread_mutex_t thread_count_mutex = PTHREAD_MUTEX_INITIALIZER;///safely increment or decrement a counter variable in a multithreaded environment where multiple threads might try to read and write to it concurrently

// Function to increment the thread count in a thread-safe manner
void increment_thread_count() {
    pthread_mutex_lock(&thread_count_mutex);  // Lock the mutex before updating
    thread_count++;  // Increment the global counter
    pthread_mutex_unlock(&thread_count_mutex);  // Unlock the mutex
}

///The struct to replicate the partition tuple of less,equal, and more arrays in Python code
typedef struct {
    int * less;//pointer to array of elements less than pivot
    size_t less_size;//number of elements of less
    int * same;//pointer to array of elements equal to pivot
    size_t same_size;//number of elements of same
    int * more;//pointer to array of elements greater than pivot
    size_t more_size;//number of elements of more
}PartitionResult;

/**
 * The struct to store the arguments for each thread.
 */
typedef struct{
    int * L;//the array to sort
    size_t size;//the size of the array
    
}ThreadArgs;

/**
 * Function for partitioning the array.
 * @param pivot the pivot for partitioning.
 * @param L the array.
 * @param size the number of elements to partition.
 * @pre make sure the array is not null.
 * @exception throw an error if allocation is improper.
 * @return the pointer to the structure representing the tuple of less, same, and more arrays.
 */
PartitionResult * partition(int pivot,const int * L,size_t size){
   
    if(L==NULL || size==0){
        fprintf(stderr,"No array to partition\n");
        return NULL;
    }

    PartitionResult * result=(PartitionResult*)malloc(sizeof(PartitionResult));
    if(result==NULL){
        fprintf(stderr,"Improper allocation\n");
        return NULL;
    }

    result->less=(int*)calloc(size,sizeof(int));
    if(result->less==NULL){
        fprintf(stderr,"Improper allocation\n");
        free(result);
        result=NULL;
        return NULL;
    }
    result->same=(int*)calloc(size,sizeof(int));
    if(result->same==NULL){
        fprintf(stderr,"Improper allocation\n");
        free(result->less);
        result->less=NULL;
        free(result);
        result=NULL;
        return NULL;
    }
    result->more=(int*)calloc(size,sizeof(int));
    if(result->more==NULL){
        fprintf(stderr,"Improper allocation\n");
        free(result->less);
        result->less=NULL;
        free(result->same);
        result->same=NULL;
        free(result);
        result=NULL;
        return NULL;
    }
    result->less_size=0;
    result->same_size=0;
    result->more_size=0;
    for(size_t i=0;i<size;i++){
        if(L[i]<pivot){
            result->less[result->less_size++]=L[i];
        }
        else if(L[i]>pivot){
            result->more[result->more_size++]=L[i];
        }
        else{
            result->same[result->same_size++]=L[i];
        }
    }
    result->less=(int*)realloc(result->less,(result->less_size)*sizeof(int));
    
    result->same=(int*)realloc(result->same,(result->same_size)*sizeof(int));
    
    result->more=(int*)realloc(result->more,(size-result->less_size-result->same_size)*sizeof(int));
   
    return result;
}

/**
 * Unthreaded quicksort - out of place.
 * @param size the number of elements to sort.
 * @param data the sample array to sort.
 * @pre make sure the data is not null or size is not 0.
 * @exception throw an error if the one of the arrays is not properly allocated.
 * @return the sorted array.
 */
int * quicksort(size_t size,  const int * data){
    if(data==NULL||size==0){
        
        return NULL;
    }
    int * res = (int*)calloc(size,sizeof(int));
    if(res==NULL){
        fprintf(stderr,"Improper allocation\n");
        return NULL;
    }
    int pivot=data[0];
    PartitionResult * partition_tuple=partition(pivot,data,size);
    if(partition_tuple==NULL){
        
        free(res);
        res=NULL;
        return NULL;
    }
    int * less_sorted=quicksort(partition_tuple->less_size,partition_tuple->less);
    
    int * more_sorted=quicksort(partition_tuple->more_size,partition_tuple->more);
    
    //combine the result : less+same+more
    int index=0;
    for(size_t i=0;i<partition_tuple->less_size;i++){
        res[index++]=less_sorted[i];
    }
    for(size_t i=0;i<partition_tuple->same_size;i++){
        res[index++]=partition_tuple->same[i];
    }
    for(size_t i=0;i<partition_tuple->more_size;i++){
        res[index++]=more_sorted[i];
    }
    free(partition_tuple->less);
    partition_tuple->less=NULL;
    free(partition_tuple->same);
    partition_tuple->same=NULL;
    free(partition_tuple->more);
    partition_tuple->more=NULL;
    free(less_sorted);
    less_sorted=NULL;
    free(more_sorted);
    more_sorted=NULL;
    free(partition_tuple);
    partition_tuple=NULL;
    return res;
}

/**
 * Threaded quicksort (out of place).
 * @param args list of arguments to create the ThreadArgs structure object above.
 * @pre args must not be null.
 * @exception throw an error if allocation is invalid.
 * @return the sorted array.
 */
void *quicksort_threaded( void *args ){
    if(args==NULL){
        fprintf(stderr,"Invalid arguments\n");
        return NULL;
    }

    ThreadArgs* threadArgs = (ThreadArgs*)args;

    
    
   const int* data = threadArgs->L;

    
    size_t size = threadArgs->size;
   
    if(size==0){
         
        return NULL;
    }

    
    
    if(size==1){
        
        int * L=(int*)malloc(sizeof(int));
        if(L==NULL){
            return NULL;
        }
        L[0]=data[0];
        
        
        return L;
    }

    
    int pivot = data[0];
    PartitionResult * partition_tuple = partition(pivot, data, size);
    
    if(partition_tuple==NULL){
        
        
        return NULL;
    }
    
    
    pthread_t thread1,thread2;
        // Decide whether to create threads based on the size of the subarrays
    
    

    
    

    ThreadArgs threadArgs1 = {partition_tuple->less,partition_tuple->less_size};

    


    increment_thread_count();

    int thread_1_res=pthread_create(&thread1, NULL, quicksort_threaded, &threadArgs1);

    if(thread_1_res!=0){

        perror("Error creating thread\n");

        free(partition_tuple->less);
        partition_tuple->less=NULL;
        free(partition_tuple->same);
        partition_tuple->same=NULL;
        free(partition_tuple->more);
        partition_tuple->more=NULL;
        free(partition_tuple);
        partition_tuple=NULL;
        
        
        
        
        return NULL;
    }
            
    
    

    ThreadArgs threadArgs2 = {partition_tuple->more,partition_tuple->more_size};

    increment_thread_count();

    int thread_res_2=pthread_create(&thread2, NULL, quicksort_threaded, &threadArgs2);

    if(thread_res_2!=0){

        perror("Error creating thread\n");


        free(partition_tuple->less);
        partition_tuple->less=NULL;
        free(partition_tuple->same);
        partition_tuple->same=NULL;
        free(partition_tuple->more);
        partition_tuple->more=NULL;
        free(partition_tuple);
        partition_tuple=NULL;
        
        
        
        
        
        return NULL;
    }        
   
    int *sorted_less;
    int *sorted_greater;

        // Wait for threads to finish and get the results
    
    pthread_join(thread1, (void**)&sorted_less);
    
    
    pthread_join(thread2, (void**)&sorted_greater);
    
    int *final_result = (int*)calloc(size , sizeof(int));

    if(final_result==NULL){
        fprintf(stderr,"Improper allocation\n");
        free(partition_tuple->less);
        partition_tuple->less=NULL;
        free(partition_tuple->same);
        partition_tuple->same=NULL;
        free(partition_tuple->more);
        partition_tuple->more=NULL;
        free(partition_tuple);
        partition_tuple=NULL;
        free(sorted_less);
        sorted_less=NULL;
        free(sorted_greater);
        sorted_greater=NULL;
        return NULL;
    }

    // Copy the results into the final array
    int index = 0;
    for (size_t i = 0; i < partition_tuple->less_size; i++) {
        final_result[index++] = sorted_less[i];
    }
    for (size_t i = 0; i < partition_tuple->same_size; i++) {
        final_result[index++] = partition_tuple->same[i];
    }
    for (size_t i = 0; i < partition_tuple->more_size; i++) {
        final_result[index++] = sorted_greater[i];
    }

    
    // Clean up
    free(partition_tuple->less);

    partition_tuple->less=NULL;

    free(partition_tuple->same);

    partition_tuple->same=NULL;

    free(partition_tuple->more);
    
    partition_tuple->more=NULL;
    
    free(partition_tuple);
    
    partition_tuple=NULL;
    
    free(sorted_less);
    sorted_less=NULL;

    free(sorted_greater);

    sorted_greater=NULL;
    

    

    return (void*)final_result;
    
}
/**
 * Print out the given number of elements in an array.
 * @param arr the array to print.
 * @param size the number of elements to print.
 */
void print_array(int * arr,size_t size){
    for (size_t i = 0; i < size; i++) {
            // Print the element
            printf("%d", arr[i]);

            // Print a comma if it's not the last element
            if (i < size - 1) {
                printf(", ");
            }
    }
}



/**
 * This is where we implement and compare unthreaded and threaded quicksorts.
 * @param argv the number of arguments.
 * @param args the list of arguments.
 * @exception throw an error if the file pointer is null
 * @return EXIT_SUCCESS if no issues happen. Else, return EXIT_FAILURE.
 */
int main(int argv, char * args[]){
    FILE*fptr;
    fptr=fopen(args[argv-1],"r");   
    if(fptr==NULL){
        fprintf(stderr,"Error opening file\n");
        return EXIT_FAILURE;
    }
    
    int num;
    fseek(fptr, 0, SEEK_END);
    size_t filesize = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);  // Reset file pointer to beginning
    

    int * L = (int *)calloc(filesize,sizeof(int));

    if(L==NULL){
        fprintf(stderr,"Improper allocation");
        fclose(fptr);
        return EXIT_FAILURE;
    }

    size_t size=0;//true size of the array

    while(fscanf(fptr,"%d",&num)!=EOF){
        L[size++]=num;
        
    }

    fclose(fptr);

    clock_t start, end;

    if(argv==3 && strcmp(args[1],"-p")==0){
        printf("Unsorted list before non-threaded quicksort: ");
        print_array(L,size);
        printf("\n");

        

        start = clock();

        int * unthreaded_sort=quicksort(size,L);

        end = clock();

        double total_time;
        total_time = (double) (end - start) / CLOCKS_PER_SEC;

        printf("Non-threaded time: %f\n",total_time);

        printf("Resulting list: ");
        print_array(unthreaded_sort,size);
        printf("\n");
        free(unthreaded_sort);
        unthreaded_sort=NULL;

        ThreadArgs threadArgs={L,size};

        

        start=clock();

        int * sorted_thread;

        pthread_t mainthread;


        increment_thread_count();

        int rc =pthread_create(&mainthread, NULL, quicksort_threaded, &threadArgs);

        if(rc){
            perror("Error creating thread in main function\n");
            free(L);
            L=NULL;
            exit(EXIT_FAILURE);
        }
        
        
        

        pthread_join(mainthread, (void **)&sorted_thread);
        if(sorted_thread==NULL){
            fprintf(stderr,"Threaded quicksort failed\n");
            free(L);
            L=NULL;
            exit(EXIT_FAILURE);
        }
        end=clock();
        total_time=(double) (end - start) / CLOCKS_PER_SEC;

        printf("Threaded time: %f\n",total_time);

        printf("Threads spawned: %d\n",thread_count);

        printf("Resulting list: ");
        print_array(sorted_thread,size);
        printf("\n");

        
        free(sorted_thread);
        sorted_thread=NULL;

        
    }
    else if(argv==2){
        

        

        start = clock();

        int * unthreaded_sort=quicksort(size,L);

        free(unthreaded_sort);
        unthreaded_sort=NULL;

        end = clock();

        double total_time;
        total_time = (double) (end - start) / CLOCKS_PER_SEC;

        printf("Non-threaded time: %f\n",total_time);

        

        ThreadArgs threadArgs={L,size};

        

        start=clock();

        int * sorted_thread;

        pthread_t mainthread;


        increment_thread_count();

        int rc =pthread_create(&mainthread, NULL, quicksort_threaded, &threadArgs);

        if(rc){
            perror("Error creating thread in main function\n");
            free(L);
            L=NULL;
            exit(EXIT_FAILURE);
        }
        
        
        

        pthread_join(mainthread, (void **)&sorted_thread);
        
        if(sorted_thread==NULL){
            fprintf(stderr,"Threaded quicksort failed\n");
            free(L);
            L=NULL;
            exit(EXIT_FAILURE);
        }

        end=clock();
        total_time=(double) (end - start) / CLOCKS_PER_SEC;

        printf("Threaded time: %f\n",total_time);

        printf("Threads spawned: %d\n",thread_count);

        

        
        free(sorted_thread);
        sorted_thread=NULL;
        
        
    }

    free(L);
    L=NULL;

    return EXIT_SUCCESS;
}