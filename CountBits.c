#include <stdlib.h>

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* countBits(int n, int* returnSize) {
    // 1. Set the size of the returned array
    *returnSize = n + 1;
    
    // 2. Dynamically allocate memory for the result array
    int* ret = (int*)malloc((*returnSize) * sizeof(int));
    if (ret == NULL) {
        *returnSize = 0;
        return NULL;
    }
    
    // 3. Initialize the base case (0 bits for the number 0)
    ret[0] = 0;
    
    // 4. Mirror the dynamic programming bitwise logic
    for (int i = 1; i <= n; ++i) {
        ret[i] = ret[i & (i - 1)] + 1;
    }
    
    return ret;
}
