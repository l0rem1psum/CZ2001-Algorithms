#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

intArray *generateRandomTestArray(int size);
intArray *generateAscendingTestArray(int size);
intArray *generateDescendingTestArray(int size);
intArray *readTestFileToArray(int size, const char *type);
void generateTestFile(int size);
void generateTestFile_Random(int size);
void generateTestFile_Ascending(int size);
void generateTestFile_Descending(int size);
void saveSortedArrayToFile(intArray *intArr);
int getRandomInt();
void getFilePath(char *path, const char *type, int size);
void test(int size);

intArray *generateRandomTestArray(int size){
    intArray *testArray = readTestFileToArray(size, "Random");
    return testArray;
}

intArray *generateAscendingTestArray(int size){
    intArray *testArray = readTestFileToArray(size, "Ascending");
    return testArray;
}

intArray *generateDescendingTestArray(int size){
    intArray *testArray = readTestFileToArray(size, "Descending");
    return testArray;
}


intArray *readTestFileToArray(int size, const char *type){
    char filePath[40];
    getFilePath(filePath, type, size);
    FILE * fPointer = fopen(filePath, "r");

    intArray *intArr = malloc(sizeof(intArray));
    intArr -> arr = malloc(size * sizeof(int));
    intArr -> size = size;

    for(int i = 0; i < size; i++){
        fscanf(fPointer, "%d\n", ((intArr -> arr) + i));
    }
    fclose(fPointer);
    return intArr;
}

void generateTestFile(int size){
    srand(time(NULL));
    generateTestFile_Random(size);
    generateTestFile_Ascending(size);
    generateTestFile_Descending(size);
}

void generateTestFile_Random(int size){
    char filePath[40];
    getFilePath(filePath, "Random", size);
    if (access(filePath, F_OK) != -1){
        return;
    } else {
        FILE *fPointer = fopen(filePath, "wb");
        for(int i = 0; i < size; i++){
            int x = getRandomInt();
            fprintf(fPointer, "%d\n", x);
        }
        fclose(fPointer);
    }
}

void generateTestFile_Ascending(int size){
    char filePath[40];
    getFilePath(filePath, "Ascending", size);
    if (access(filePath, F_OK) != -1){
        return;
    } else {
        FILE *fPointer = fopen(filePath, "wb");
        for(int i = 1; i <= size; i++){
            fprintf(fPointer, "%d\n", i);
        }
        fclose(fPointer);
    }
}


void generateTestFile_Descending(int size){
    char filePath[40];
    getFilePath(filePath, "Descending", size);
    if (access(filePath, F_OK) != -1){
        return;
    } else {
        FILE *fPointer = fopen(filePath, "wb");
        for(int i = size; i > 0; i--){
            fprintf(fPointer, "%d\n", i);
        }
        fclose(fPointer);
    }
}

void saveSortedArrayToFile(intArray *intArr){
    char filePath[40];
    getFilePath(filePath, "Sorted", intArr -> size);
    FILE *fPointer = fopen(filePath, "wb");
    for(int i = 0; i < intArr -> size; i++){
        fprintf(fPointer, "%d\n", *((intArr -> arr) + i));
    }
    fclose(fPointer);
}

int getRandomInt(){
    unsigned long x;
    x = rand();
    x <<= 15;
    x ^= rand();
    x %= 1000001;
    return x;
}

void getFilePath(char *path, const char *type, int size){
    snprintf(path, 40, "Test_Files/%d_%s.txt", size, type);
}

void test(int size){
    generateTestFile(size);

    FILE *fPointer = fopen("Test_Result.csv", "ab+");
    fprintf(fPointer, "%d,",size);

    printf("###########################################################################\n");


    intArray *intArr = generateRandomTestArray(size);
    printf("Sorting %d integers in a randomly generated array with InsertionSort1.\n", size);
    clock_t begin = clock();
    insertionSort1(intArr);
    clock_t end = clock();
    double exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Sort finished! Total time taken is %f seconds.\n", exec_time);
    fprintf(fPointer, "%f,", exec_time);
    free(intArr);

    intArr = generateAscendingTestArray(size);
    printf("Sorting %d integers in an ascending array with InsertionSort1.\n", size);
    begin = clock();
    insertionSort1(intArr);
    end = clock();
    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Sort finished! Total time taken is %f seconds.\n", exec_time);
    fprintf(fPointer, "%f,", exec_time);
    free(intArr);

    intArr = generateDescendingTestArray(size);
    printf("Sorting %d integers in a descending array with InsertionSort1.\n", size);
    begin = clock();
    insertionSort1(intArr);
    end = clock();
    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Sort finished! Total time taken is %f seconds.\n", exec_time);
    fprintf(fPointer, "%f,", exec_time);
    free(intArr);

    printf("---------------------------------------------------------------------------\n");

    intArr = generateRandomTestArray(size);
    printf("Sorting %d integers in a randomly generated array with InsertionSort2.\n", size);
    begin = clock();
    insertionSort2(intArr);
    end = clock();
    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Sort finished! Total time taken is %f seconds.\n", exec_time);
    fprintf(fPointer, "%f,", exec_time);
    free(intArr);

    intArr = generateAscendingTestArray(size);
    printf("Sorting %d integers in an ascending array with InsertionSort2.\n", size);
    begin = clock();
    insertionSort2(intArr);
    end = clock();
    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Sort finished! Total time taken is %f seconds.\n", exec_time);
    fprintf(fPointer, "%f,", exec_time);
    free(intArr);

    intArr = generateDescendingTestArray(size);
    printf("Sorting %d integers in a descending array with InsertionSort2.\n", size);
    begin = clock();
    insertionSort2(intArr);
    end = clock();
    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Sort finished! Total time taken is %f seconds.\n", exec_time);
    fprintf(fPointer, "%f,", exec_time);
    free(intArr);

    printf("---------------------------------------------------------------------------\n");

    intArr = generateRandomTestArray(size);
    printf("Sorting %d integers in a randomly generated array with MergeSort1.\n", size);
    begin = clock();
    mergeSort1(intArr);
    end = clock();
    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Sort finished! Total time taken is %f seconds.\n", exec_time);
    fprintf(fPointer, "%f,", exec_time);
    free(intArr);

    intArr = generateAscendingTestArray(size);
    printf("Sorting %d integers in an ascending array with MergeSort1.\n", size);
    begin = clock();
    mergeSort1(intArr);
    end = clock();
    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Sort finished! Total time taken is %f seconds.\n", exec_time);
    fprintf(fPointer, "%f,", exec_time);
    free(intArr);

    intArr = generateDescendingTestArray(size);
    printf("Sorting %d integers in a descending array with MergeSort1.\n", size);
    begin = clock();
    mergeSort1(intArr);
    end = clock();
    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Sort finished! Total time taken is %f seconds.\n", exec_time);
    fprintf(fPointer, "%f,", exec_time);
    free(intArr);

    printf("---------------------------------------------------------------------------\n");

    intArray *intArr = generateRandomTestArray(size);
    printf("Sorting %d integers in a randomly generated array with MergeSort2.\n", size);
    clock_t begin = clock();
    mergeSort2(intArr);
    clock_t end = clock();
    double exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Sort finished! Total time taken is %f seconds.\n", exec_time);
    fprintf(fPointer, "%f,", exec_time);
    free(intArr);

    intArr = generateAscendingTestArray(size);
    printf("Sorting %d integers in an ascending array with MergeSort2.\n", size);
    begin = clock();
    mergeSort2(intArr);
    end = clock();
    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Sort finished! Total time taken is %f seconds.\n", exec_time);
    fprintf(fPointer, "%f,", exec_time);
    free(intArr);

    intArr = generateDescendingTestArray(size);
    printf("Sorting %d integers in a descending array with MergeSort2.\n", size);
    begin = clock();
    mergeSort2(intArr);
    end = clock();
    exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Sort finished! Total time taken is %f seconds.\n", exec_time);
    fprintf(fPointer, "%f", exec_time);
    free(intArr);

    fprintf(fPointer, "\n");

    fclose(fPointer);
}

