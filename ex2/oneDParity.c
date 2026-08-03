#include <stdio.h>
#include <string.h>

void corrupt_data(char *data, int position) {
    // Convert 1-based user input to 0-based array index
    int index = position - 1;

    if (data[index] == '1') {
        data[index] = '0';
    } else if (data[index] == '0') {
        data[index] = '1';
    }
}
int main()
{
    char data[100], received[100];
    int i,ch, ones = 0, parityChoice;
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
    printf("if you want to corrupt enter the position else enter 0:");
    scanf("%d",&ch);
    if (ch!=0){
       corrupt_data(data, ch);
    }
    printf("Transmitted Data: %s\n", data);

    // Receiver side
    strcpy(received,data);
    printf("\nReceived data: %s\n",received);

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
