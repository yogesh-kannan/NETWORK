#include <stdlib.h>

int* countBits(int n, int* returnSize) {

    *returnSize = n + 1;
    
    int* ret = (int*)malloc((*returnSize) * sizeof(int));
    if (ret == NULL) {
        *returnSize = 0;
        return NULL;
    }

    ret[0] = 0;

    for (int i = 1; i <= n; ++i) {
        ret[i] = ret[i & (i - 1)] + 1;
    }
    
    return ret;
}
