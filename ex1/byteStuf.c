#include <stdio.h>
#include <string.h>

#define MAX 100

// Define special characters
#define FLAG 'F'
#define ESC  'E'

char data[MAX], stuffed[MAX], framed[MAX], destuffed[MAX];
int i,bit;

// Helper function to print a character array as 8-bit binary
void printBinary(char* label, char arr[], int n) {
   printf("%s: ", label);
   for(i = 0; i < n; i++) {
      // Extract and print bits from MSB to LSB
      for(bit = 7; bit >= 0; bit--) {
         printf("%d", (arr[i] >> bit) & 1);
      }
      printf(" "); // Space between byte blocks
   }
   printf("\n");
}

// Helper function to print a character array as standard ASCII text
void printASCII(char* label, char arr[], int n) {
   printf("%s: ", label);
   for(i = 0; i < n; i++) {
      printf("%c ", arr[i]);
   }
   printf("\n");
}

int main() {
   int n;
   printf("Enter number of bytes: ");
   if (scanf("%d", &n) != 1) return 1;

   // Clear input buffer newline
   while (getchar() != '\n');

   printf("Enter the Bytes: ");
   for(i = 0; i < n; i++) {
      if (scanf(" %c", &data[i]) != 1) return 1;
   }

   // 1. Byte Stuffing Logic
   int j = 0;
   for(i = 0; i < n; i++) {
      if (data[i] == FLAG || data[i] == ESC) {
         stuffed[j++] = ESC;
      }
      stuffed[j++] = data[i];
   }
   int stuffedlen = j;

   // 2. Framing (Prepend FLAG + Payload + Append FLAG)
   int k = 0;
   framed[k++] = FLAG;
   for(i = 0; i < stuffedlen; i++) {
      framed[k++] = stuffed[i];
   }
   framed[k++] = FLAG;
   int framedlen = k;

   // Display outputs in requested formats
   printBinary("Original Bytes : ", data, n);
   printBinary("Stuffed : ", stuffed, stuffedlen);
   printBinary("Framed : ", framed, framedlen);

   // 3. Destuffing Logic
   j = 0;
   for(i = 1; i < framedlen - 1; i++) {
      if (framed[i] == ESC) {
         i++;
      }
      destuffed[j++] = framed[i];
   }
   int destuffedlen = j;

   printASCII("Destuffed : ", destuffed, destuffedlen);
   return 0;
}
