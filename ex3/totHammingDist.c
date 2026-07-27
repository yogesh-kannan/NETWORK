int totalHammingDistance(int* nums, int numsSize) {
    int count = 0;
    for (int i = 0; i < 32; i++) {
        int countOnes = 0;
        for (int j = 0; j < numsSize; j++)
            if ((nums[j] >> i) & 1) countOnes++;
        int countZeros = numsSize - countOnes;
        count += countOnes * countZeros;
    }
    return count;
}
