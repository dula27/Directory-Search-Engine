# Report

All the documentation for the program

Question (a):If we have N documents, and document Diconsists of miwords, then how long does this simple solution take to search for a query consisting of K words. Give your answer in big O notation. A more efficient solution can be constructed using other data structures.
`search O(n) where n is total number of words in all documents, given that large linked list contains all words from all documents`

Question (b):If we assume that the hash function maps words evenly across the buckets (i.e., each bucket gets the same number of words), then what is the time complexity (big O notation) of this improved solution? Use the same parameters as Question (a) for the size of each document and size of the query set.
`search O(1*n) where n is number of words in a bucket`

Question (c)why does this leadto a more efficient search process?
`It does not have to iterate through words that do not affect the idf relevancy of the search engine, this saves time in terms of Big-O.`

Question (d):Which of the two approaches is more efficient in terms of removing stop words and why.
`The second approach of hashtree is more efficient because the main chain of words is not repeated, it has to only search for word once and then it can move on to the other branch to look for word frequency. Removing the word from hashtree will result in the whole branch to be removed, however in first approach same word will have to be removed for all documents.`

# Makefile

Tested on:

```java
MINGW64
bash
kali-linux
```

How to make:

```bash
make
./search
```

How to clean all residue:

```bash
make clean
```

# search.c

```java
- Programs asks for number of buckets for hashmap.
- $USER inputs integer value from console.
- creates hashmap by calling hm_create
- calls training() function from training.c to populate hashmap with words
- loops through retrieval phase
- loop stopped when `X` is input
- loop continues when <enter_key> is pressed
- read() function from retrieval calls rank() to output rank of relevant files on screen
```

# hashmap.c

```java
/** This function allocates space in the heap for hashmap to be created
 * @param num_buckets size of hashmap buckets
 * @return empty hashmap of given size
*/
```

`struct hashmap *hm_create(int num_buckets)`

```java
/** This function prints the hashmap for debugging and viewing
 * @param hm hashmap to print
 * @return void
*/
```

`void printHashMap(struct hashmap *hm)`

```java
/** This function removes a node from hashmap
 * @param hm hashmap from which word is to be removed from
 * @param word word to remove
 * @param document_id document ID to remove
 * @return void
*/
```

`void hm_remove(struct hashmap *hm, char *word, char *document_id)`

```java
/** This function frees memory of the hashmap to avoid all memory leaks
 * @param hm hashmap to free from heap
 * @return void
*/
```

`void hm_destroy(struct hashmap *hm)`

```java
/** This function provides the bucket number after hashing it.
 * @param hm hashmap to use number of buckets from
 * @param w word to use hashing function for
 * @return bucket number after hashing
*/
```

`int hash_code(struct hashmap *hm, char *w)`

```java
/** This function inserts word and documentID into the hashmap. If a word and document
 * that is already inserted before then it increments the frequency count of the node.
 * @param hm hashmap to insert node into
 * @param w word to be added
 * @param i document ID to be added
 * @return void
*/
```

`void hash_table_insert(struct hashmap *hm, char *w, char *i)`

```java
/** This function searches through the hashmap for a word and then return the 
 * number of occurances of that word in hashmap
 * @param hm hashmap to search from
 * @param w word to search for
 * @param docID document ID with the word
 * @return number of occurance of the word
*/
```

`int hash_deep_search(struct hashmap *hm, char *w, char *docID)`

# training.c

Option 1 is implemented with arbitrary number of files in directory using glob.

```java
/** This function searches through the File for a word
 * @param docID document you are searching through
 * @param word word we are searching for
 * @return numOccurances number of times the word has occured in document
*/
```

`int SearchFile(char *docID, char *word)`

```java
/** This function populates hashmap with a certain document
 * @param hm hashmap to populate
 * @param docID document to populate with
 * @return void
*/
```

`void populate(struct hashmap *hm, char *docID)`

```java
/** This function uses populate function and iterates through directory to
 *  populate hashmap with all files
 * @param hm hashmap to populate
 * @return hm populate hashmap
*/
```

`struct hashmap *training(struct hashmap *hm)`

# retrieval.c

```java
/** This function reads the user input query and then calls rank 
 * @param hm hashmap to search query from
 * @return void
*/
```

`void read(struct hashmap *hm)`

```java
/** This function ranks each word in the query. It calls hash_deep_search function
 * in hashmap.c to search for number of occurances of word in all documents. Then 
 * it calculates idf and tf-idf and writes the rank to file search_scores.txt
 * @param hm hashmap to search query from
 * @param query multiple words we are searching for
 * @return numOccurances number of times the word has occured in document
*/
```

`void rank(struct hashmap *hm, char *query)`

# Assumptions

- Case sensitive searching
- Stopwords function is implemented using if statement such that if the word is not found in all documents in directory it wont show on console for the relevant file.
- There are no stopwords
