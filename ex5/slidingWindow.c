#include <stdio.h>

void goBackN(int n, int window, int lost)
{
    int i = 1, j;
    int transmissions = 0;

    printf("\n===== GO-BACK-N =====\n");

    while (i <= n)
    {
        int end = i + window - 1;

        if (end > n)
            end = n;

        printf("\nSending window: ");

        for (j = i; j <= end; j++)
        {
            printf("%d ", j);
            transmissions++;
        }

        printf("\n");

        if (lost >= i && lost <= end)
        {
            printf("Frame %d lost/corrupted.\n", lost);

            for (j = i; j < lost; j++)
                printf("ACK %d received.\n", j);

            printf("Timeout for frame %d.\n", lost);

            printf("Retransmitting frames: ");

            for (j = lost; j <= end; j++)
            {
                printf("%d ", j);
                transmissions++;
            }

            printf("\n");

            for (j = lost; j <= end; j++)
                printf("ACK %d received.\n", j);

            lost = 0;
        }
        else
        {
            for (j = i; j <= end; j++)
                printf("ACK %d received.\n", j);
        }

        i = end + 1;
    }

    printf("\nAll frames delivered successfully.\n");
    printf("Total transmissions = %d\n", transmissions);
}

void selectiveRepeat(int n, int window, int lost)
{
    int i = 1, j;
    int transmissions = 0;

    printf("\n===== SELECTIVE REPEAT =====\n");

    while (i <= n)
    {
        int end = i + window - 1;

        if (end > n)
            end = n;

        printf("\nSending window: ");

        for (j = i; j <= end; j++)
        {
            printf("%d ", j);
            transmissions++;
        }

        printf("\n");

        if (lost >= i && lost <= end)
        {
            printf("Frame %d lost/corrupted.\n", lost);

            for (j = i; j <= end; j++)
            {
                if (j == lost)
                    printf("Frame %d: No ACK\n", j);
                else
                    printf("Frame %d: ACK received\n", j);
            }

            printf("Timeout for frame %d.\n", lost);

            printf("Retransmitting only frame %d\n", lost);

            transmissions++;

            printf("ACK %d received.\n", lost);

            lost = 0;
        }
        else
        {
            for (j = i; j <= end; j++)
                printf("Frame %d: ACK received\n", j);
        }

        i = end + 1;
    }

    printf("\nAll frames delivered successfully.\n");
    printf("Total transmissions = %d\n", transmissions);
}

int main()
{
    int choice;
    int n, window, lost;

    printf("====================================\n");
    printf("     SLIDING WINDOW PROTOCOL\n");
    printf("====================================\n");

    printf("\n1. Go-Back-N\n");
    printf("2. Selective Repeat\n");

    printf("\nEnter your choice: ");
    scanf("%d", &choice);

    printf("Enter number of frames: ");
    scanf("%d", &n);

    printf("Enter window size: ");
    scanf("%d", &window);

    printf("Enter frame number to be lost/corrupted (0 for none): ");
    scanf("%d", &lost);

    if (window <= 0 || window > n)
    {
        printf("Invalid window size.\n");
        return 0;
    }

    if (lost < 0 || lost > n)
    {
        printf("Invalid frame number.\n");
        return 0;
    }

    if (choice == 1)
    {
        goBackN(n, window, lost);
    }
    else if (choice == 2)
    {
        selectiveRepeat(n, window, lost);
    }
    else
    {
        printf("Invalid choice.\n");
    }

    return 0;
}
