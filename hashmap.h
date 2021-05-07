#ifndef HASHMAP_H
#define HASHMAP_H

struct llnode
{
        char *word;          //KEY
        char *document_id;   //KEY
        int num_occurrences; //VALUE
        struct llnode *next;
};

struct hashmap
{
        struct llnode **map;
        int num_buckets;
        int num_elements;
};

struct hashmap *hm_create(int num_buckets);
void hm_destroy(struct hashmap *hm);
void hm_remove(struct hashmap *hm, char *word, char *document_id);
int hash_code(struct hashmap *hm, char *w);
void hash_table_insert(struct hashmap *hm, char *w, char *i);
int SearchFile(char *docID, char *word);
struct hashmap *training(struct hashmap *hm);
void printHashMap(struct hashmap *hm);
void populate(struct hashmap *hm, char *docID);
void read(struct hashmap *hm);
int hash_deep_search(struct hashmap *hm, char *w, char *docID);
void rank(struct hashmap *hm, char *query);

#endif
