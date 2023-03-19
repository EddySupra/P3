#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE (sizeof(list) / sizeof(*list))

int list[] = {7, 12, 19, 3, 18, 4, 2, -5, 6, 15, 8};
int result[SIZE] = {0};

typedef struct {
    int* subArray;
    unsigned int size;
} SortingThreadParameters;

typedef struct {
    SortingThreadParameters left;
    SortingThreadParameters right;
} MergingThreadParameters;

void swap(int* xp, int* yp) {
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void bubbleSort(int arr[], int n) {
    int i, j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(&arr[j], &arr[j + 1]);
            }
        }
    }
}

void* sort(void* param) {
    SortingThreadParameters* parame = (SortingThreadParameters*) param;
    bubbleSort(parame->subArray, parame->size);
    pthread_exit(0);
}

void merge(int leftArr[], int rightArr[], int sortedArr[]) {
    int sizeLeft = SIZE / 2;
    int sizeRight = SIZE - sizeLeft;
    int i = 0, j = 0, k = 0;

    while (i < sizeLeft && j < sizeRight) {
        if (leftArr[i] < rightArr[j]) {
            sortedArr[k++] = leftArr[i++];
        } else {
            sortedArr[k++] = rightArr[j++];
        }
    }

    while (i < sizeLeft) {
        sortedArr[k++] = leftArr[i++];
    }

    while (j < sizeRight) {
        sortedArr[k++] = rightArr[j++];
    }
}

void* mergeSort(void* param) {
    MergingThreadParameters* parame = (MergingThreadParameters*) param;
    merge(parame->left.subArray, parame->right.subArray, result);
    pthread_exit(0);
}

int main() {
    int middle = SIZE / 2;

    pthread_t sortingThread_1;
    pthread_t sortingThread_2;
    pthread_t mergingThread;

    // Left array sort
    SortingThreadParameters* leftArr = malloc(sizeof(SortingThreadParameters));
    leftArr->subArray = list;
    leftArr->size = middle;
    pthread_create(&sortingThread_1, NULL, sort, leftArr);

    // Right array sort
    SortingThreadParameters* rightArr = malloc(sizeof(SortingThreadParameters));
    rightArr->subArray = list + middle;
    rightArr->size = SIZE - middle;
    pthread_create(&sortingThread_2, NULL, sort, rightArr);

    // Wait till sorted
    pthread_join(sortingThread_1, NULL);
    pthread_join(sortingThread_2, NULL);

    // Merge thread
    MergingThreadParameters* mergeArr = malloc(sizeof(MergingThreadParameters));
    mergeArr->left = *leftArr;
    mergeArr->right = *rightArr;
    pthread_create(&mergingThread, NULL, mergeSort, mergeArr);
    pthread_join(mergingThread, NULL);

    // Print the sorted array
    printf("Sorted array: ");
    for (int i = 0; i < SIZE; i++)
    {
        printf("%d ",result[i]);
    }
    

    return 0;
}
