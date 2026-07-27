#include <stdbool.h>

bool hasAlternatingBits(int n) {
    long x = 1;                     // 1 = 1
    while (x <= n) {                // 2 = 10
        if (x == n)                 // 5 = 101
            return true;            // 10 = 1010
        else if (x % 2 == 0)        // 21 = 10101
            x = 2 * x + 1;          // 42 = 101010
        else                        // 85 = 1010101
            x = 2 * x;
    }
    return false;
}
