#include <stdbool.h>

bool hasAlternatingBits(int n) {
    long x = 1;                    
    while (x <= n) {              
        if (x == n)               
            return true;            
        else if (x % 2 == 0)       
            x = 2 * x + 1;          
        else                       
            x = 2 * x;
    }
    return false;
}
