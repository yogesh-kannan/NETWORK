#include <stdio.h>
#include <math.h>

int main() {
    int m,r=0,i,j,k=0,n,pos;
    printf("Enter number of data bits: ");
    scanf("%d",&m);
    while((1<<r) < m+r+1) r++;
    n=m+r;
    int data[m+1], ham[n+1];
    printf("Enter %d data bits:\n",m);
    for(i=m;i>=1;i--) scanf("%d",&data[i]);
    k=1;
    for(i=1;i<=n;i++){
        if((i&(i-1))==0) ham[i]=0;
        else ham[i]=data[k++];
    }
    for(i=0;i<r;i++){
        int p=1<<i,par=0;
        for(j=1;j<=n;j++) if(j&p) par^=ham[j];
        ham[p]=par;
    }
    printf("\nHamming Code: ");
    for(i=n;i>=1;i--) printf("%d",ham[i]);
    printf("\nEnter error position (0 for no error): ");
    scanf("%d",&pos);
    if(pos>0&&pos<=n) ham[pos]^=1;
    printf("Received Code: ");
    for(i=n;i>=1;i--) printf("%d",ham[i]);
    int syn=0;
    for(i=0;i<r;i++){
        int p=1<<i,par=0;
        for(j=1;j<=n;j++) if(j&p) par^=ham[j];
        if(par) syn+=p;
    }
    if(syn){
        printf("\nError detected at position %d",syn);
        ham[syn]^=1;
        printf("\nCorrected Code: ");
        for(i=n;i>=1;i--) printf("%d",ham[i]);
    } else printf("\nNo Error Detected");
    printf("\nRecovered Data: ");
    for(i=n;i>=1;i--) if((i&(i-1))!=0) printf("%d",ham[i]);
    printf("\n");
    return 0;
}

/**
 Enter number of data bits: 7
Enter 7 data bits:
1 0 0 1 1 0 1

Enter error position (0 for no error): 0
Hamming Code: 10011100101
Received Code: 10011100101
No Error Detected
Recovered Data: 1001101

Enter number of data bits: 7
Enter 7 data bits:
1 0 0 1 1 0 1

Enter error position (0 for no error): 5
Hamming Code: 10011100101
Received Code: 10011110101
Error detected at position 5
Corrected Code: 10011100101
Recovered Data: 1001101


Enter number of data bits: 4
Enter 4 data bits:
1 0 1 1

Enter error position (0 for no error): 0
Hamming Code: 1010101
Received Code: 1010101
No Error Detected
Recovered Data: 1011


Enter number of data bits: 4
Enter 4 data bits:
1 0 1 1

Enter error position (0 for no error): 3
Hamming Code: 1010101
Received Code: 1010001
Error detected at position 3
Corrected Code: 1010101
Recovered Data: 1011

*/
