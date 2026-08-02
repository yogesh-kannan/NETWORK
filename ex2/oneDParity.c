#include <stdio.h>
#include <string.h>

int main()
{
    char data[100], received[100];
    int i, ones = 0, parityChoice;
    char parityBit;

    printf("Enter Binary Data: ");
    scanf("%s", data);

    printf("Choose Parity:\n");
    printf("1. Even Parity\n");
    printf("2. Odd Parity\n");
    printf("Enter Choice: ");
    scanf("%d", &parityChoice);

    // Count number of 1's
    for(i = 0; data[i] != '\0'; i++)
    {
        if(data[i] == '1')
            ones++;
    }

    // Generate parity bit
    if(parityChoice == 1) // Even Parity
    {
        parityBit = (ones % 2 == 0) ? '0' : '1';
    }
    else // Odd Parity
    {
        parityBit = (ones % 2 == 0) ? '1' : '0';
    }

    printf("\nGenerated Parity Bit: %c\n", parityBit);

    // Transmitted data
    strcat(data, (char[]){parityBit, '\0'});
    printf("Transmitted Data: %s\n", data);

    // Receiver side
    printf("\nEnter Received Data: ");
    scanf("%s", received);

    ones = 0;
    for(i = 0; received[i] != '\0'; i++)
    {
        if(received[i] == '1')
            ones++;
    }

    if(parityChoice == 1) // Even parity check
    {
        if(ones % 2 == 0)
            printf("Status: No Error Detected\n");
        else
            printf("Status: Error Detected\n");
    }
    else // Odd parity check
    {
        if(ones % 2 != 0)
            printf("Status: No Error Detected\n");
        else
            printf("Status: Error Detected\n");
    }

    return 0;
}

/**
Sample Output (Even Parity)

Input

Enter Binary Data: 1011001
Choose Parity:
1. Even Parity
2. Odd Parity
Enter Choice: 1

Output

Generated Parity Bit: 0
Transmitted Data: 10110010

Enter Received Data: 10110010
Status: No Error Detected
Sample Output (Error Case)

Received Data

10110011

Output

Status: Error Detected

*/
