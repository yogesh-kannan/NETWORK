#include <stdio.h>
#define MAX 200

int data[MAX], stuffed[MAX], framed[MAX], destuffed[MAX];
int flag[] = {0, 1, 1, 1, 1, 1, 1, 0};
int flaglen = 8;
int i, j, k;

void printBits(char* label, int arr[], int n) {
   printf("%s: ", label);
   for(i = 0; i < n; i++)
      printf("%d ", arr[i]);
   printf("\n");
}

int main() {
   int n, ones = 0;
   printf("Enter number of bits: ");
   if (scanf("%d", &n) != 1) return 1;

   while (getchar() != '\n'); // Clear buffer

   printf("Enter bits (0 or 1 separated by spaces): ");
   for(i = 0; i < n; i++) {
      if (scanf("%d", &data[i]) != 1) return 1;
   }


   j = 0;
   ones = 0;
   for(i = 0; i < n; i++) {
      stuffed[j++] = data[i];
      if (data[i] == 1) {
         ones++;
      } else {
         ones = 0;
      }

      if (ones == 5) {
         stuffed[j++] = 0;
         ones = 0;
      }
   }
   int stuffedlen = j;


   k = 0;
   for(i = 0; i < flaglen; i++) framed[k++] = flag[i];
   for(i = 0; i < stuffedlen; i++) framed[k++] = stuffed[i];
   for(i = 0; i < flaglen; i++) framed[k++] = flag[i];
   int framedlen = k;

   printBits("Stuffed", stuffed, stuffedlen);
   printBits("Framed (Original)", framed, framedlen);


   int errorChoice, errorPos;
   printf("\nDo you want to inject a transmission error? (1 for Yes, 0 for No): ");
   if (scanf("%d", &errorChoice) == 1 && errorChoice == 1) {
      printf("Enter bit index to flip (0 to %d): ", framedlen - 1);
      if (scanf("%d", &errorPos) == 1 && errorPos >= 0 && errorPos < framedlen) {
         framed[errorPos] = !framed[errorPos]; // Flip the bit
         printBits("Framed (Corrupted)", framed, framedlen);
      }
   }


   printf("\n--- Receiver Processing ---\n");

   if (framedlen < (flaglen * 2)) {
      printf("MESSAGE DISCARDED: Frame is too short to contain valid flags.\n");
      return 0;
   }

   for(i = 0; i < flaglen; i++) {
      if (framed[i] != flag[i]) {
         printf("MESSAGE DISCARDED: Invalid or missing opening frame flag.\n");
         return 0;
      }
   }


   for(i = 0; i < flaglen; i++) {
      if (framed[framedlen - flaglen + i] != flag[i]) {
         printf("MESSAGE DISCARDED: Missing or corrupted closing frame flag.\n");
         return 0;
      }
   }

   ones = 0;
   j = 0;


   for(i = flaglen; i < framedlen - flaglen; i++) {


      if (ones == 5 && framed[i] == 1) {

         printf("MESSAGE DISCARDED: Premature frame termination. A data bit flip created a False Closing Flag.\n");
         return 0;
      }

      if (ones == 5) {
         if (framed[i] != 0) {
            printf("MESSAGE DISCARDED: Protocol Violation. Expected stuffed 0 but found 1.\n");
            return 0;
         }
         ones = 0; // Drop the stuffed 0
         continue;
      }

      destuffed[j++] = framed[i];
      if (framed[i] == 1) {
         ones++;
      } else {
         ones = 0;
      }
   }

   printBits("Destuffed successfully", destuffed, j);
   return 0;
}
