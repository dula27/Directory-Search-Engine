#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <glob.h>
#include "hashmap.h"
/** This function searches through the File for a word
 * @param docID document you are searching through
 * @param word word we are searching for
 * @return numOccurances number of times the word has occured in document
*/
int SearchFile(char *docID, char *word)
{
    FILE *fptr;
    int lineNum = 1; // for later use
    int found = 0;   // found flag
    int num_occurances = 0;
    char temp[514];
    fptr = fopen(docID, "r");
    if (fptr == NULL)
    {
        printf("ERROR!\n");
        return -1;
    }
    while (fgets(temp, 512, fptr) != NULL)
    {
        for (int i = 0; i < 514; i++)
        {
            if (temp[i] == '\n')
            {
                temp[i + 0] = '\0';
            }
        }
        char delim[] = " ";
        //puts(temp);
        char *wordByWord = strtok(temp, delim);
        while (wordByWord != NULL)
        {
            //printf("'%s'\n", wordByWord);
            if (wordByWord != NULL)
            {
                if (strcmp(word, wordByWord) == 0)
                {
                    found++;
                    num_occurances++;
                }
            }
            wordByWord = strtok(NULL, delim);
        }
        lineNum++;
        memset(temp, 0, sizeof temp);
    }
    if (found == 0)
    {
        printf("\nSorry, match not found.\n");
    }
    //Close the file if open.
    if (fptr)
    {
        fclose(fptr);
    }
    return num_occurances;
}
/** This function populates hashmap with a certain document
 * @param hm hashmap to populate
 * @param docID document to populate with
 * @return void
*/
void populate(struct hashmap *hm, char *docID)
{
    FILE *fptr;
    int lineNum = 1; // for later use
    char temp[514];
    fptr = fopen(docID, "r");
    //IF FILE DOESNT EXIST
    if (fptr == NULL)
    {
        printf("ERROR!\n");
        return;
    }
    //READ EACH LINE
    while (fgets(temp, 512, fptr) != NULL)
    {
        for (int i = 0; i < 514; i++)
        {
            if (temp[i] == '\n')
            {
                temp[i + 0] = '\0';
            }
        }
        char delim[] = " ";
        //SPLIT LINE INTO WORDS AND INSERT INTO HASHMAP
        char *wordByWord = strtok(temp, delim);
        while (wordByWord != NULL)
        {
            char *word = strdup(wordByWord);
            if (wordByWord != NULL)
            {
                strcpy(word, wordByWord);
                //printf("%s\n", word);
                hash_table_insert(hm, word, docID);
            }
            wordByWord = strtok(NULL, delim);
        }
        lineNum++;
        memset(temp, 0, sizeof temp);
    }

    //Close the file if open.
    if (fptr)
    {
        fclose(fptr);
    }
}
/** This function uses populate function and iterates through directory to
 *  populate hashmap with all files
 * @param hm hashmap to populate
 * @return hm populate hashmap
*/
struct hashmap *training(struct hashmap *hm)
{
    //USE GLOB TO POPULATE THROUGH WHOLE DIRECTORY
    glob_t result;
    char *directory = "p5docs/*.txt";
    glob(directory, 0, NULL, &result);
    int numFiles = (int)result.gl_pathc;
    for (int i = 0; i < numFiles; i++)
    {
        populate(hm, result.gl_pathv[i]);
    }
    return hm;
}
