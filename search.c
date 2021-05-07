#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "hashmap.h"

int main(void)
{
    // USER INPUT FOR NUM BUCKETS OF HASHMAP
    int num_buckets;
    printf("How many buckets?\n");
    scanf("%d", &num_buckets);
    //CONST TO DECLARE IT STATIC
    const int numBuckets = num_buckets;
    struct hashmap *alpha = hm_create(numBuckets);
    //PERFORM TRAINING PHASE ON HASHMAP
    training(alpha);
    char ans = '\0';
    //DUMMY VALUE FOR EXTRA KEY INPUT
    char dummyValue[1];
    scanf("%c", dummyValue);
    //LOOP THAT ALLOWS YOU TO SEARCH WORDS UNLESS X IS PRESSED
    while (ans != 'X')
    {
        read(alpha);
        printf("\nPress <enter> to continue - Press X to exit:\n");
        scanf("%c", &ans);
    }
    return 0;
}
