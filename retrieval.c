#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <glob.h>
#include "hashmap.h"

/** This function reads the user input query and then calls rank 
 * @param hm hashmap to search query from
 * @return void
*/
void read(struct hashmap *hm)
{
    printf("Enter the search string:\n");
    char query[100];
    fgets(query, 100, stdin);
    printf("QUERY: %s\n", query);
    rank(hm, query);
}
/** This function ranks each word in the query. It calls hash_deep_search function
 * in hashmap.c to search for number of occurances of word in all documents. Then 
 * it calculates idf and tf-idf and writes the rank to file search_scores.txt
 * @param hm hashmap to search query from
 * @param query multiple words we are searching for
 * @return numOccurances number of times the word has occured in document
*/
void rank(struct hashmap *hm, char *query)
{
    int size;
    // DIRECTORY READ
    glob_t result;
    char *directory = "p5docs/*.txt";
    glob(directory, 0, NULL, &result);
    const int numFiles = (int)result.gl_pathc - 1;
    printf("Searching %d files...\n\n", numFiles);
    char delim[] = " ";
    for (int i = 0; i < 100; i++)
    {
        if (query[i] == '\n')
        {
            query[i + 0] = '\0';
            query[i + 1] = '\n';
        }
    }
    int searchCounts[numFiles][100];
    int notFound = 0;
    // COUNTS NUM OCCURANCES USING hash_deep_search AND STORING IT IN ARRAY FOR QUERY
    for (int i = 0; i < numFiles; i++)
    {
        int counter = 0;
        char *editable = strdup(query);
        char *wordByWord = strtok(editable, delim);
        while (wordByWord != NULL)
        {
            char *word = strdup(wordByWord);
            if (wordByWord != NULL)
            {
                strcpy(word, wordByWord);
                searchCounts[i][counter] = hash_deep_search(hm, word, result.gl_pathv[i]);
                //printf("%s %s: #%d\n", result.gl_pathv[i],word, searchCounts[i][counter]);
                if (searchCounts[i][counter] != 0)
                {
                    notFound++;
                }
                counter++;
            }
            wordByWord = strtok(NULL, delim);
        }
        searchCounts[i][counter] = -1;
        size = counter;
        //printf("------------------\n");
    }
    // IF EVERY WORD IN QUERY IS NOT FOUND IN DIRECTORY
    if (notFound == 0)
    {
        printf("No file is relevant.\n");
        return;
    }

    int docNumFound = 0;
    //CALCULATE IDF VALUE
    double idf[size];
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < numFiles; j++)
        {
            if (searchCounts[j][i] != 0)
            {
                docNumFound++;
            }
        }
        if (docNumFound != 0)
        {
            double temp = 3.0 / docNumFound;
            idf[i] = log10(temp);
        }
        else
        {
            idf[i] = 0;
        }
        //printf("idf: %.3f\n",idf[i]);
        docNumFound = 0;
    }
    //CALCULATE tf-idf VALUE AND STORE IT IN ARRAY
    double tfidf[numFiles][size];
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < numFiles; j++)
        {
            tfidf[j][i] = searchCounts[j][i] * idf[i];
            //printf("tf-idf: %.3f\n",tfidf[j][i] );
        }
    }

    double bubbleSort[numFiles];
    for (int i = 0; i < numFiles; i++)
    {
        bubbleSort[i] = 0;
    }
    // UNCOMMENT TO PRINT TF IDF SCORES
    for (int i = 0; i < numFiles; i++)
    {
        for (int j = 0; j < size; j++)
        {
            //printf("%.2f ", tfidf[i][j]);
            if (j == size - 1)
            {
                //printf("\n");
            }
        }
    }
    //CALCULATE RANK BY SUMMING TFIDF FOR EACH FILE
    int counter = 0;
    for (int k = 0; k < numFiles; k++)
    {
        while (counter != size)
        {
            bubbleSort[k] = tfidf[k][counter] + bubbleSort[k];
            counter++;
        }
        counter = 0;
    }
    //COPY DIRECTORY NAMES IN DOCS TO RANK
    char *docs[numFiles];
    for (int i = 0; i < numFiles; i++)
    {
        docs[i] = result.gl_pathv[i];
    }
    //BUBBLE SORT FOR RANKING
    for (int i = 0; i < numFiles - 1; i++)
    {
        for (int j = 0; j < numFiles - i - 1; j++)
        {
            if (bubbleSort[j] > bubbleSort[j + 1])
            {
                double temp = bubbleSort[j];
                bubbleSort[j] = bubbleSort[j + 1];
                bubbleSort[j + 1] = temp;
                char *temp1 = docs[j];
                docs[j] = docs[j + 1];
                docs[j + 1] = temp1;
            }
        }
    }
    // PRINT RANK OF RELEVANT DOCS
    for (int i = numFiles - 1; i > -1; i--)
    {
        // DOCUMENTS WITH TFIDF SCORE 0 ARE NOT PRINTED TO CONSOLE
        if (bubbleSort[i] != 0)
        {
            printf("%s\n", docs[i]);
        }
    }
    // FILE HANDLING TO ADD RANKS TO search_scores.txt FILE
    FILE *fptr;
    fptr = fopen("p5docs/search_scores.txt", "a");
    if (fptr == NULL)
    {
        printf("ERROR!\n");
        return;
    }
    for (int i = numFiles - 1; i > -1; i--)
    {
        fprintf(fptr, "%s:%.3f\n", docs[i], bubbleSort[i]);
    }
    fprintf(fptr, "-------------------\n");
    fclose(fptr);
}