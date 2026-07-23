#include <stdlib.h>
#include <string.h>
typedef struct {
    int *keys;
    int size;
} HashSet;
HashSet* createSet(int size) {
    HashSet* set = (HashSet*)malloc(sizeof(HashSet));
    set->size = size * 3; // Kept large to avoid linear probing collisions
    set->keys = (int*)malloc(set->size * sizeof(int));
    memset(set->keys, -1, set->size * sizeof(int));
    return set;
}
void insertSet(HashSet* set, int key) {
    int idx = (key < 0 ? -key : key) % set->size;
    while (set->keys[idx] != -1) {
        if (set->keys[idx] == key) return; // Already exists
        idx = (idx + 1) % set->size;
    }
    set->keys[idx] = key;
}
int countSet(HashSet* set, int key) {
    int idx = (key < 0 ? -key : key) % set->size;
    while (set->keys[idx] != -1) {
        if (set->keys[idx] == key) return 1; // Found
        idx = (idx + 1) % set->size;
    }
    return 0; // Not found
}
void freeSet(HashSet* set) {
    free(set->keys);
    free(set);
}

int findMaximumXOR(int* nums, int numsSize) {
    int ans = 0, mask = 0;
    
    for (int b = 30; b >= 0; b--) {
        mask |= (1 << b);
        
        HashSet* st = createSet(numsSize);
        
        for (int i = 0; i < numsSize; i++) {
            insertSet(st, (mask & nums[i]));
        }
        
        int temp = ans | (1 << b);
        
        for (int i = 0; i < st->size; i++) {
            int current_key = st->keys[i];
            if (current_key != -1) {
                if (countSet(st, temp ^ current_key)) {
                    ans = temp;
                    break;
                }
            }
        }
        
        freeSet(st); 
    }
    return ans;
}
