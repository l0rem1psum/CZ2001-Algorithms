#include <stdio.h>
#include <stdlib.h>

typedef struct{
    int *arr;
    int size;
} intArray;

void insertionSort1(intArray *intArr);
void insertionSort2(intArray *intArr);
void mergeSort1(intArray *intArr);
void mergeSort2(intArray *intArr);
void printSorted(intArray *intArr);
void _insertionSort1(int *arr, int size);
void _insertionSort2(int *arr, int size);
void _mergeSort1(int *arr, int size);
void _mergeSort2(int *arr, int size);
void mergeWithoutAux(int *arr, int size);
void mergeWithAux(int *arr, int size);
void _printSorted(int *arr, int size);
void swapIndex(int *arr, int i, int j);

/***************************************************************/
void insertionSort1(intArray *intArr){
    _insertionSort1(intArr -> arr, intArr -> size);
}

void insertionSort2(intArray *intArr){
    _insertionSort2(intArr -> arr, intArr -> size);
}

void _insertionSort1(int *arr, int size){
    for(int i = 0; i < size; i++){
        int j = i;
        while(j > 0 && *(arr + j) < *(arr + j - 1)){
            swapIndex(arr, j, j - 1);
            j--;
        }
    }
}

void _insertionSort2(int *arr, int size){ // Does not make use of the swapIndex() function.
    for(int i = 0; i < size; i++){
        int item = *(arr + i);
        int j = i - 1;
        while(j >= 0 && *(arr + j) > item){
            *(arr + j + 1) = *(arr + j);
            j--;
        }
        *(arr + j + 1) = item;
    }
}
/***************************************************************/
void mergeSort1(intArray *intArr){
    _mergeSort1(intArr -> arr, intArr -> size);
}

void mergeSort2(intArray *intArr){
    _mergeSort2(intArr -> arr, intArr -> size);
}

void _mergeSort1(int *arr, int size){
    if (size == 1) {return;}
    int mid = (size + 1) / 2;
    _mergeSort1(arr, mid);
    _mergeSort1(arr + mid, size - mid);
    mergeWithoutAux(arr, size);
}

void _mergeSort2(int *arr, int size){
    if (size == 1) {return;}
    int mid = (size + 1) / 2;
    _mergeSort2(arr, mid);
    _mergeSort2(arr + mid, size - mid);
    mergeWithAux(arr, size);
}

void mergeWithoutAux(int *arr, int size){
    int i = 0;
    int j = (size + 1) / 2;
    int temp;
    while (i != j){
        if (*(arr + i) < *(arr + j)){
            i++;
        } else if (*(arr + i) > *(arr + j)){
            temp = *(arr + j);
            for (int a = j; a > i; a--){
                *(arr + a) = *(arr + a - 1);
            }
            *(arr + i) = temp;
            i++;
            if(j < size - 1) {j++;}
        } else { //Actually, this third scenario can be considered the same as the second. But this slightly reduces the number of iterations.
            if (j - i == 1){return;} // If this is not included, array such as [2, 2] will cause an error.
            temp = *(arr + j);
            for (int a = j; a > i + 1; a--){
                *(arr + a) = *(arr + a - 1);
            }
            *(arr + i + 1) = temp;
            i += 2;
            if(j < size - 1) {j++;}
        }
    }
}

void mergeWithAux(int *arr, int size){
    int i = 0;
    int j = (size + 1) / 2;
    int mid = j - 1;
    int *auxArr = malloc(sizeof(int) * size);
    int a = 0;

    for (int m = 0; m < size; m++){
        *(auxArr + m) = *(arr + m);
    }

    while(i <= mid && j <= size - 1){
        if (*(auxArr + i) <= *(auxArr + j)){
            *(arr + a) = *(auxArr + i);
            i++;
        } else {
            *(arr + a) = *(auxArr + j);
            j++;
        }
        a++;
    }

    while(i <= mid){
        *(arr + a) = *(auxArr + i);
        i++;
        a++;
    }
    while(j <= size - 1){
        *(arr + a) = *(auxArr + j);
        j++;
        a++;
    }
    free(auxArr);
    //_printSorted(auxArr, size);
    //return auxArr;
}

/***************************************************************/
void printSorted(intArray *intArr){
    _printSorted(intArr -> arr, intArr -> size);
}

void _printSorted(int *arr, int size){
    for(int i = 0; i < size; i++){
        printf("%d\n", *(arr + i));
    }
}

void swapIndex(int *arr, int i, int j){
    int temp = *(arr + i);
    *(arr + i) = *(arr + j);
    *(arr + j) = temp;
}
