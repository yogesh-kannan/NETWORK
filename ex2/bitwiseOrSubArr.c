#include <stdio.h>
#include <stdlib.h>
int compare(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}
int subarrayBitwiseORs(int* arr, int arrSize) {
    if (arrSize == 0) return 0;
    int maxResults = arrSize * 32; 
    int* results = (int*)malloc(maxResults * sizeof(int));
    int count = 0;
    for (int i = 0; i < arrSize; i++) {
        results[count++] = arr[i];
        for (int j = i - 1; j >= 0; j--) {
            if ((arr[j] | arr[i]) == arr[j]) {
                break;
            }
            arr[j] |= arr[i];
            results[count++] = arr[j];
        }
    }
    qsort(results, count, sizeof(int), compare);
    int uniqueCount = 0;
    if (count > 0) {
        uniqueCount = 1;
        for (int i = 1; i < count; i++) {
            if (results[i] != results[i - 1]) {
                uniqueCount++;
            }
        }
    }
    free(results);
    return uniqueCount;
}
