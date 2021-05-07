#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "hashmap.h"

/** This function allocates space in the heap for hashmap to be created
 * @param num_buckets size of hashmap buckets
 * @return empty hashmap of given size
*/
struct hashmap *hm_create(int num_buckets)
{
	struct hashmap *alpha;									  //'alpha' is our hashmap being created
	alpha = (struct hashmap *)malloc(sizeof(struct hashmap)); //allocate memory size of hashmap in heap

	alpha->num_buckets = num_buckets;

	alpha->map = (struct llnode **)malloc((num_buckets) * sizeof(struct llnode *));

	//ALLOCATE EACH BUCKET SIZE OF A LLIST
	for (int i = 0; i < num_buckets; i++)
	{
		alpha->map[i] = (struct llnode *)malloc(sizeof(struct llnode));
		alpha->map[i] = NULL;
	}
	return alpha;
}
/** This function prints the hashmap for debugging and viewing
 * @param hm hashmap to print
 * @return void
*/
void printHashMap(struct hashmap *hm)
{

	printf("Number of Buckets: %d\n", hm->num_buckets);

	//EDGE CASE CHECK RETURN IF HASHMAP IS EMPTY
	if (hm == NULL)
	{
		printf("hm is null");
		return;
	}

	int trackCounter = 1; //count for track number

	//LOOP WITHIN EACH BUCKET
	for (int i = 0; i < hm->num_buckets; i++)
	{
		if (hm->map[i] == NULL)
		{
			continue;
		}
		printf("Bucket Number: %d\n", i);
		struct llnode *curNode = hm->map[i]; //current node declared to increment as a counter
		while (curNode != NULL)
		{ //loop till the last track in a bucket
			printf("Track #%d\n", trackCounter);
			printf("Word: %s\n", curNode->word);
			printf("Document: %s\n", curNode->document_id);
			printf("Frequency: %d\n\n", curNode->num_occurrences);

			trackCounter++;
			curNode = curNode->next; //move to next track
		}
	}
}
/** This function removes a node from hashmap
 * @param hm hashmap from which word is to be removed from
 * @param word word to remove
 * @param document_id document ID to remove
 * @return void
*/
void hm_remove(struct hashmap *hm, char *word, char *document_id)
{

	struct llnode *nodeCheck = (struct llnode *)malloc(sizeof(struct llnode));
	nodeCheck->word = word;
	nodeCheck->document_id = document_id;

	for (int i = 0; i < hm->num_buckets; i++)
	{

		//Base Case: Linked list is not empty
		if (hm->map[i] != NULL)
		{
			//Case 1: Only one element in linked list (head) is to be removed
			if ((strcmp(nodeCheck->word, hm->map[i]->word) == 0) && (strcmp(nodeCheck->document_id, hm->map[i]->document_id) == 0) && (hm->map[i]->next == NULL))
			{
				hm->map[i] = NULL;
				printf("Removed.\n");
				return;
			}
		}
		//Case 2: Head is to be removed
		if (hm->map[i] != NULL)
		{
			if ((strcmp(nodeCheck->word, hm->map[i]->word) == 0) && (strcmp(nodeCheck->document_id, hm->map[i]->document_id) == 0) && (hm->map[i]->next != NULL))
			{

				struct llnode *temp = hm->map[i]->next;
				free(hm->map[i]->next);
				hm->map[i] = temp;
				free(temp);
				printf("Removed.\n");
				return;
			}
		}

		//Case 3: Already has elements
		struct llnode *curCounter = hm->map[i];
		while (curCounter != NULL)
		{
			if ((hm->map[i] != NULL) && (hm->map[i]->next != NULL))
			{
				if ((strcmp(nodeCheck->word, curCounter->next->word) == 0) // hm->buckets[i]->head->next->artist
					&& (strcmp(nodeCheck->document_id, curCounter->next->document_id) == 0))
				{
					struct llnode *temp = curCounter;
					free(curCounter->next);
					temp->next = curCounter->next->next;
					printf("Removed.\n");
					return;
				}
			}
			curCounter = curCounter->next;
		}
	}
	printf("Unable to Remove.\n");
	return;
}
/** This function frees memory of the hashmap to avoid all memory leaks
 * @param hm hashmap to free from heap
 * @return void
*/
void hm_destroy(struct hashmap *hm)
{
	for (int i = 0; i < hm->num_buckets; i++)
	{
		struct llnode *curCounter = hm->map[i];
		struct llnode *curNext;
		while (curCounter != NULL)
		{
			curNext = curCounter->next;
			free(curCounter);
			curCounter = curNext;
		}
		free(hm->map[i]);
		free(curCounter);
		free(curNext);
	}
	free(hm->map);
	free(hm);
}
/** This function provides the bucket number after hashing it.
 * @param hm hashmap to use number of buckets from
 * @param w word to use hashing function for
 * @return bucket number after hashing
*/
int hash_code(struct hashmap *hm, char *w)
{
	int S = 0;
	int modulo;
	int i = 0;

	//loops while it sums ASCII values of the string characters
	while (*(w + i) != '\0')
	{
		S = S + (int)*(w + i);
		i++;
	}
	modulo = S % hm->num_buckets;

	return modulo;
}
/** This function inserts word and documentID into the hashmap. If a word and document
 * that is already inserted before then it increments the frequency count of the node.
 * @param hm hashmap to insert node into
 * @param w word to be added
 * @param i document ID to be added
 * @return void
*/
void hash_table_insert(struct hashmap *hm, char *w, char *i)
{
	int bucketNumber; //variable to store the bucket number to add to
	bucketNumber = hash_code(hm, w);
	int num_occurrences = 1;

	//CREATE A NEW NODE AND STORE PASSED VALUES INTO THE NODE
	struct llnode *nodeCheck = (struct llnode *)malloc(sizeof(struct llnode)); //allocate space for nodeCheck in heap memory
	nodeCheck->word = w;
	nodeCheck->document_id = i;

	//LOOP THROUGH ALL NODES IN EACH BUCKET
	for (int i = 0; i < hm->num_buckets; i++)
	{
		struct llnode *curCounter = hm->map[i]; //current node declared to increment as a counter
		while (curCounter != NULL)
		{ //loop till it reaches last track
			//IF FOUND RETURN NODE
			if ((strcmp(nodeCheck->word, curCounter->word) == 0) && ((strcmp(nodeCheck->document_id, curCounter->document_id) == 0)))
			{
				curCounter->num_occurrences = curCounter->num_occurrences + 1;
				//printf("Added1.\n");
				return;
			}
			curCounter = curCounter->next;
		}
	}
	//Case 1: Bucket is empty
	if (hm->map[bucketNumber] == NULL)
	{
		hm->map[bucketNumber] = (struct llnode *)malloc(sizeof(struct llnode));
		hm->map[bucketNumber]->word = w;
		hm->map[bucketNumber]->document_id = i;
		hm->map[bucketNumber]->num_occurrences = num_occurrences;
		hm->map[bucketNumber]->next = NULL;
		//printf("%s\n", hm->map[bucketNumber]->word);
		//printf("Added2.\n");
		return;
	}

	struct llnode *nodeCounter = hm->map[bucketNumber];
	//Case 2: Bucket already has nodes
	while (nodeCounter != NULL)
	{
		if (nodeCounter->next == NULL)
		{
			nodeCounter->next = (struct llnode *)malloc(sizeof(struct llnode));
			nodeCounter->next->word = w;
			nodeCounter->next->document_id = i;
			nodeCounter->next->num_occurrences = num_occurrences;
			nodeCounter->next->next = NULL;
			//printf("%s\n", nodeCounter->next->word);
			//printf("Added3.\n");
			return;
		}
		nodeCounter = nodeCounter->next;
	}
	printf("Not added.\n");
	return;
}
/** This function searches through the hashmap for a word and then return the 
 * number of occurances of that word in hashmap
 * @param hm hashmap to search from
 * @param w word to search for
 * @param docID document ID with the word
 * @return number of occurance of the word
*/
int hash_deep_search(struct hashmap *hm, char *w, char *docID)
{

	//CREATE A NEW NODE AND STORE PASSED VALUES INTO THE NODE
	struct llnode *nodeCheck = (struct llnode *)malloc(sizeof(struct llnode)); //allocate space for nodeCheck in heap memory
	nodeCheck->word = w;
	nodeCheck->document_id = docID;

	//LOOP THROUGH ALL NODES IN EACH BUCKET
	for (int i = 0; i < hm->num_buckets; i++)
	{
		struct llnode *curCounter = hm->map[i]; //current node declared to increment as a counter
		while (curCounter != NULL)
		{ //loop till it reaches last track
			//IF FOUND RETURN NODE
			//IF FOUND RETURN NODE
			if ((strcmp(nodeCheck->word, curCounter->word) == 0) && ((strcmp(nodeCheck->document_id, curCounter->document_id) == 0)))
			{
				//printf("Counted.\n");
				return curCounter->num_occurrences;
			}
			curCounter = curCounter->next;
		}
	}
	return 0;
}
