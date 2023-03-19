#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define SIZE ( sizeof(list)/sizeof(*list) )
int list[] = {7, 12, 19, 3, 18, 4, 2, -5, 6, 15 , 8}; 
int result[SIZE] = {0};

typedef struct 
{
    int * subArray;
    unsigned int size;
}   SortingThreadParameters;

typedef struct 
{
    SortingThreadParameters left;
    SortingThreadParameters right;
}   MergingThreadParameters;

void swap(int* xp, int* yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void basesort(int Arr[]){
    //bubble sort (need size of array being sorted)
    int n = (&Arr)[1] - Arr;
    int i, j;
    for (i = 0; i < n - 1; i++)
        // Last i elements are already in place
        for (j = 0; j < n - i - 1; j++)
            if (Arr[j] > Arr[j + 1])
                swap(&Arr[j], &Arr[j + 1]);
}

void* sort(void *param){
    SortingThreadParameters* parame = (SortingThreadParameters*)param;
    basesort(parame->subArray);
    pthread_exit(0);
}

void mergeSort(int leftArr[], int rightArr[], int sortedArr[]){


int sizeLeft = (&leftArr)[1] - leftArr;
int sizeRight = (&rightArr)[1] - rightArr;
    
int i = 0, j = 0, k = 0;

while (i< sizeLeft && j < sizeRight)
{
    if (leftArr[i] < rightArr[j])
        sortedArr[k++] = leftArr[i++];
    else
        sortedArr[k++] = rightArr[j++];
}

while (i < sizeLeft)
    sortedArr[k++] = leftArr[i++];

while (j < sizeRight)
    sortedArr[k++] = rightArr[j++];

}

void * merge(void * param){
    MergingThreadParameters* parame = (MergingThreadParameters*)param;
    mergeSort(parame->left.subArray, parame->right.subArray, result);
    for(int i = 0; i < SIZE; i++)
    {
        printf("%d", result[i]);
    }
    pthread_exit(0);
}



int main(){
    int middle = SIZE/2;
    
    pthread_t sortingThread_1;
    pthread_t sortingThread_2;
    pthread_t mergingThread;



    //left are sort
    SortingThreadParameters * leftArr = malloc(sizeof(SortingThreadParameters));
    leftArr->subArray = list;
    leftArr->size = SIZE/2;
    //create sorting threads and call sort function 
    pthread_create(&sortingThread_1, NULL, sort, leftArr);

    
    
    //right arr sort 
    SortingThreadParameters * rightArr = malloc(sizeof(SortingThreadParameters));
    rightArr->subArray = list;
    rightArr->size = SIZE/2;
    //create sorting threads and call sort function 
    pthread_create(&sortingThread_2, NULL, sort, rightArr);
    
    //wait till sorted
    pthread_join(sortingThread_1, NULL);
    pthread_join(sortingThread_2, NULL);
    
    //merge thread
    MergingThreadParameters * mergeArr = malloc(sizeof(MergingThreadParameters));
    mergeArr->left = *leftArr;
    mergeArr->right = *rightArr;

    pthread_create(&mergingThread, NULL, merge, mergeArr);
    pthread_join(mergingThread, NULL);



    return 0;
}
