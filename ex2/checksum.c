#include <stdio.h>
#include <string.h>

#define MAX 10
#define SIZE 20

// Binary addition with end-around carry
void binaryAdd(char a[], char b[], char result[], int n)
{
    int carry = 0, sum,i;

    for (i = n - 1; i >= 0; i--)
    {
        sum = (a[i] - '0') + (b[i] - '0') + carry;
        result[i] = (sum % 2) + '0';
        carry = sum / 2;
    }

    // End-around carry
    while (carry)
    {
        for (i = n - 1; i >= 0; i--)
        {
            sum = (result[i] - '0') + carry;
            result[i] = (sum % 2) + '0';
            carry = sum / 2;
        }
    }

    result[n] = '\0';
}

// One's complement
void onesComplement(char str[], int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        if (str[i] == '0')
            str[i] = '1';
        else
            str[i] = '0';
    }
}

int main()
{
    int n,i, size;

    printf("Enter number of binary data words: ");
    scanf("%d", &n);

    char data[MAX][SIZE];

    printf("Enter size of each binary word: ");
    scanf("%d", &size);

    printf("\nEnter Binary Data:\n");

    for (i = 0; i < n; i++)
    {
        printf("Data %d: ", i + 1);
        scanf("%s", data[i]);

        if (strlen(data[i]) != size)
        {
            printf("Invalid word length!\n");
            return 0;
        }
    }

    char sum[SIZE];
    strcpy(sum, data[0]);

    // Sender Side Addition
    for (i = 1; i < n; i++)
    {
        char temp[SIZE];
        binaryAdd(sum, data[i], temp, size);
        strcpy(sum, temp);
    }

    printf("\nBinary Sum      : %s", sum);

    char checksum[SIZE];
    strcpy(checksum, sum);
    onesComplement(checksum, size);

    printf("\nChecksum        : %s", checksum);

    printf("\n\n--- Transmitted Data ---\n");

    for (i = 0; i < n; i++)
        printf("Data %d : %s\n", i + 1, data[i]);

    printf("Checksum : %s\n", checksum);

    // Receiver Side
    char received[MAX][SIZE];
    char recvChecksum[SIZE];

    printf("\n--- Received Data ---\n");

    for (i = 0; i < n; i++)
    {
        strcpy(received[i],data[i]);
        printf("Received Data %d : %s\n ", i + 1,received[i]);
    }

    strcpy(recvChecksum,checksum);
    printf("Received Checksum : %s\n",recvChecksum);

    int choice;

    printf("\nIntroduce Error?\n");
    printf("0. No\n1. Yes\n");
    printf("Enter Choice: ");
    scanf("%d", &choice);

    if (choice == 1)
    {
        int flips;

        printf("Enter number of bits to flip: ");
        scanf("%d", &flips);

        while (flips--)
        {
            int word, pos;

            printf("Flip in (1-%d data words, %d for checksum): ", n + 1, n + 1);
            scanf("%d", &word);

            printf("Bit Position (1-%d): ", size);
            scanf("%d", &pos);

            if (word <= n)
            {
                if (received[word - 1][pos - 1] == '0')
                    received[word - 1][pos - 1] = '1';
                else
                    received[word - 1][pos - 1] = '0';
            }
            else
            {
                if (recvChecksum[pos - 1] == '0')
                    recvChecksum[pos - 1] = '1';
                else
                    recvChecksum[pos - 1] = '0';
            }
        }
    }

    printf("\n--- Receiver Side ---\n");

    char receiverSum[SIZE];
    strcpy(receiverSum, received[0]);

    for (i = 1; i < n; i++)
    {
        char temp[SIZE];
        binaryAdd(receiverSum, received[i], temp, size);
        strcpy(receiverSum, temp);
    }

    char temp[SIZE];
    binaryAdd(receiverSum, recvChecksum, temp, size);
    strcpy(receiverSum, temp);

    printf("Binary Sum : %s\n", receiverSum);

    char final[SIZE];
    strcpy(final, receiverSum);
    onesComplement(final, size);

    printf("Complement : %s\n", final);

    int error = 0;

    for (i = 0; i < size; i++)
    {
        if (final[i] != '0')
        {
            error = 1;
            break;
        }
    }

    if (error)
        printf("\nStatus : ERROR DETECTED\n");
    else
        printf("\nStatus : NO ERROR\n");

    return 0;
}
/**
[24bcs063@mepcolinux ex2]$./p3
Enter number of binary data words: 4
Enter size of each binary word: 8

Enter Binary Data:
Data 1: 10011011
Data 2: 11001100
Data 3: 11110000
Data 4: 00001111

Binary Sum      : 01101000
Checksum        : 10010111

--- Transmitted Data ---
Data 1 : 10011011
Data 2 : 11001100
Data 3 : 11110000
Data 4 : 00001111
Checksum : 10010111

--- Receiver Data ---
Received Data 1: 10011011
Received Data 2: 11001100
Received Data 3: 11110000
Received Data 4: 00001111
Received Checksum: 10010111

Introduce Error?
0. No
1. Yes
Enter Choice: 0

--- Receiver Side ---
Binary Sum : 11111111
Complement : 00000000

Status : NO ERROR
[24bcs063@mepcolinux ex2]$./p3
Enter number of binary data words: 4
Enter size of each binary word: 8

Enter Binary Data:
Data 1: 10011011
Data 2: 11001100
Data 3: 11110000
Data 4: 00001111

Binary Sum      : 01101000
Checksum        : 10010111

--- Transmitted Data ---
Data 1 : 10011011
Data 2 : 11001100
Data 3 : 11110000
Data 4 : 00001111
Checksum : 10010111

--- Receiver Data ---
Received Data 1: 10011011
Received Data 2: 11001100
Received Data 3: 11110000
Received Data 4: 00001111
Received Checksum: 10010111

Introduce Error?
0. No
1. Yes
Enter Choice: 1
Enter number of bits to flip: 1
Flip in (1-5 data words, 5 for checksum): 2
Bit Position (1-8): 2

--- Receiver Side ---
Binary Sum : 10111111
Complement : 01000000

Status : ERROR DETECTED
[24bcs063@mepcolinux ex2]$./p3
Enter number of binary data words: 4
Enter size of each binary word: 8

Enter Binary Data:
Data 1: 10011011
Data 2: 11001100
Data 3: 11110000
Data 4: 00001111

Binary Sum      : 01101000
Checksum        : 10010111

--- Transmitted Data ---
Data 1 : 10011011
Data 2 : 11001100
Data 3 : 11110000
Data 4 : 00001111
Checksum : 10010111

--- Receiver Data ---
Received Data 1: 10011011
Received Data 2: 11001100
Received Data 3: 11110000
Received Data 4: 00001111
Received Checksum: 10010111

Introduce Error?
0. No
1. Yes
Enter Choice: 1
Enter number of bits to flip: 3
Flip in (1-5 data words, 5 for checksum): 1
Bit Position (1-8): 2
Flip in (1-5 data words, 5 for checksum): 3
Bit Position (1-8): 4
Flip in (1-5 data words, 5 for checksum): 5
Bit Position (1-8): 4

--- Receiver Side ---
Binary Sum : 00100000
Complement : 11011111

Status : ERROR DETECTED
============================================*/
