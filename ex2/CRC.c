#include <stdio.h>
#include <string.h>

int main()
{
    char data[100], divisor[30], temp[100], codeword[100], remainder[30];
    int i, j, dataLen, divLen;
    int valid = 1;

    printf("Enter Sender Binary Data: ");
    scanf("%s", data);

    printf("Enter Generator Polynomial: ");
    scanf("%s", divisor);

    divLen = strlen(divisor);

    /* Validate Generator Polynomial */

    if(divLen < 2)
        valid = 0;

    if(divisor[0] != '1' || divisor[divLen - 1] != '1')
        valid = 0;

    for(i = 0; i < divLen; i++)
    {
        if(divisor[i] != '0' && divisor[i] != '1')
        {
            valid = 0;
            break;
        }
    }

    if(!valid)
    {
        printf("\nGenerator Polynomial is Invalid.\n");
        printf("Message discarded because the generator polynomial is invalid.\n");
        return 0;
    }

    printf("\nGenerator Polynomial is Valid.\n");

    dataLen = strlen(data);

    strcpy(temp, data);

    for(i = 0; i < divLen - 1; i++)
        temp[dataLen + i] = '0';
    temp[dataLen + divLen - 1] = '\0';

    strcpy(codeword, temp);

    /* CRC Division */

    for(i = 0; i < dataLen; i++)
    {
        if(temp[i] == '1')
        {
            for(j = 0; j < divLen; j++)
            {
                if(temp[i + j] == divisor[j])
                    temp[i + j] = '0';
                else
                    temp[i + j] = '1';
            }
        }
    }

    for(i = 0; i < divLen - 1; i++)
        remainder[i] = temp[dataLen + i];

    remainder[divLen - 1] = '\0';

    strcpy(codeword, data);
    strcat(codeword, remainder);

    printf("\nCRC Remainder       : %s", remainder);
    printf("\nTransmitted Codeword: %s", codeword);

    /* Receiver Side */

    char received[100];
    strcpy(received, codeword);

    int choice;

    printf("\n\nChoose Testcase");
    printf("\n0 - Valid");
    printf("\n1 - Invalid");
    printf("\nEnter Choice: ");
    scanf("%d", &choice);

    if(choice == 1)
    {
        int pos;

        printf("Enter Bit Position to Flip (1-%ld): ", strlen(received));
        scanf("%d", &pos);

        if(pos >= 1 && pos <= strlen(received))
        {
            if(received[pos - 1] == '0')
                received[pos - 1] = '1';
            else
                received[pos - 1] = '0';
        }
    }

    printf("\nReceived Codeword   : %s\n", received);

    strcpy(temp, received);

    for(i = 0; i <= strlen(received) - divLen; i++)
    {
        if(temp[i] == '1')
        {
            for(j = 0; j < divLen; j++)
            {
                if(temp[i + j] == divisor[j])
                    temp[i + j] = '0';
                else
                    temp[i + j] = '1';
            }
        }
    }

    printf("Receiver Remainder  : ");

    int error = 0;

    for(i = strlen(received) - divLen + 1; i < strlen(received); i++)
    {
        printf("%c", temp[i]);

        if(temp[i] == '1')
            error = 1;
    }

    if(error)
        printf("\nError Detected (Invalid Testcase)\n");
    else
        printf("\nNo Error Detected (Valid Testcase)\n");

    return 0;
}
