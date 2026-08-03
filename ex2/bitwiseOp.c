#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

void calculate_the_maximum(int n, int k) {
    int maxAnd = 0;
    int maxOr = 0;
    int maxXor = 0;
    for (int i = 1; i <= n; i++) {
        for (int j = i + 1; j <= n; j++) {
            
            // Bitwise AND logic
            int and_res = i & j;
            if (and_res > maxAnd && and_res < k) {
                maxAnd = and_res;
            }
            
            // Bitwise OR logic
            int or_res = i | j;
            if (or_res > maxOr && or_res < k) {
                maxOr = or_res;
            }
            
            // Bitwise XOR logic
            int xor_res = i ^ j;
            if (xor_res > maxXor && xor_res < k) {
                maxXor = xor_res;
            }
        }
    }
    
    // Print the maximum values on new lines
    printf("%d\n%d\n%d\n", maxAnd, maxOr, maxXor);
}

int main() {
    int n, k;
  
    // Reads n and k from standard input
    scanf("%d %d", &n, &k);
    calculate_the_maximum(n, k);
 
    return 0;
}
