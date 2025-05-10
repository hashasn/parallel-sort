#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


//synchronization tools
pthread_mutex_t mutex;
pthread_cond_t condt1, condt2;


int A[200];                             // array to be sorted
int n = 0;                              // number of elements in array
int swaps = 0;                          // number of swaps made
int t1_swapped = 1, t2_swapped = 1;     // tracks swaps in each round
int done = 0;                           // used to end threads
int turn = 0;                           //whose turn it is, 0 is t1 and 1 is t2


// reads integers from file into array A
void read_input(char* filename) {
    FILE* fp = fopen(filename, "r");
    if(fp == NULL ) {
         perror("Error opening file");
         exit(1);
    }


    while(fscanf(fp, "%d", &A[n]) == 1 && n < 200) {
        n++;
    }

    fclose(fp);
}


// prints the array
void print_array() {
    for(int i = 0; i < n; i++) {
        printf("%d ", A[i]);
    }

    printf("\n");
}



// thread function for t1 and t2
void* sort(void* arg) {

    char* type = (char*) arg;   // "T1" or "T2"
 
    int id = 0;
    
     
    if(type[1] == '1') 
    {
        id = 0;     // id = 0 for T1
    } else {
        id = 1;     // id = 0 for T1
    }


    while(!done) {

        pthread_mutex_lock(&mutex);

        //wait for turn or arrays is sorted
        while(turn != id && !done) {
            if(id == 0)
            {
                pthread_cond_wait(&condt1, &mutex);     // t1 waits
            } else {
                pthread_cond_wait(&condt2, &mutex);     // t2 waits

            }
        }

        if(done) {
            pthread_mutex_unlock(&mutex);
            break;
        }

        int local_swaps = 0;

        if(id == 0) {
            for (int j = 0; j + 1 < n; j += 2) {
                if (A[j] > A[j + 1]) {
                    int temp = A[j];
                    A[j] = A[j + 1];
                    A[j + 1] = temp;
                    local_swaps++;
                }
            }
            t1_swapped = local_swaps;
        } else {
            for(int j = 1; j + 1 < n; j += 2) {
                if (A[j] > A[j + 1]) {
                    int temp = A[j];
                    A[j] = A[j + 1];
                    A[j + 1] = temp;
                    local_swaps++;
                }
            }
            t2_swapped = local_swaps;
        }

        swaps += local_swaps;


        printf("Thread %s was swapped %d times in this round\n", type, local_swaps);


        if(t1_swapped == 0 && t2_swapped == 0){
            done = 1;
            pthread_cond_signal(&condt1);
            pthread_cond_signal(&condt2);
            pthread_mutex_unlock(&mutex);
            break;
        }

        turn = 1 - id;
        if(turn == 0) {
            pthread_cond_signal(&condt1);
        } else{ 
            pthread_cond_signal(&condt2);
        }
       

        pthread_mutex_unlock(&mutex);
    }


    
    return NULL;

}


int main(int argc, char* argv[]) {
    
    if(argc != 2){
        printf("Usage: %s <ToSort>\n", argv[0]);
        return 1;
    }


    read_input(argv[1]);
    printf("Initial array %d numbers:\n", n);
    print_array();



    pthread_t t1, t2;

    pthread_create(&t1, NULL, sort, "T1");
    pthread_create(&t2, NULL, sort, "T2");


    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Sorted array:\n");
    print_array();


    printf("Total number of swaps to sort array A = %d\n", swaps);


    return 0;
}