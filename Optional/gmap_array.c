#include "gmap.h"


#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

/**
 * An implementation of a generic map as an unsorted fixed-size array.
 * This implementation will not meet the requirements of Assignment #4 and
 * not completely tested itself but should be sufficient for testing your
 * Blotto competition program.
 */

char *gmap_error = "error";

/**
 * A (key, value) pair stored in a map.
 */
typedef struct entry
{
  void *key;
  void *value;
  struct entry *next;
} entry;


/**
 * Meta-data for the map.
 *
 * @member table a pointer to the array of entries in the map
 * @member capacity the max number of entries that can be stored in the array
 * @member size the number of entries in the map
 * @member hash the hash function used by the map
 * @member compare the function used to compare keys in the map
 * @member copy the function used to create new copies of keys in the map
 * @member free the function used to destroy keys in the map
 */
struct _gmap
{
  entry **table;
  size_t capacity;
  size_t size;
  size_t (*hash)(const void *);
  int (*compare)(const void *, const void *);
  void *(*copy)(const void *);
  void (*free)(void *);
};


#define GMAP_INITIAL_CAPACITY 100


/**
 * Returns the index where key is located in the given map, or the index
 * where it would go if it is not present.
 * 
 * @param table a table with at least one free slot, non-NULL
 * @param key a string, non-NULL
 * @param hash the hash function used for the keys, non-NULL
 * @param compare a comparison function for keys, non-NULL
 * @param size the size of the table
 * @param capacity the capacity of the table
 * @return the index of key in table, or the index of the empty slot to put it in if it is not present
 */
entry *gmap_table_find_key(entry **table, const void *key, size_t (*hash)(const void *), int (*compare)(const void *, const void *), size_t capacity);


/**
 * Resizes the array holding the elements in the given map.  The current
 * implementation does nothing.
 * 
 * @param m a pointer to a map, non-NULL
 */
void gmap_embiggen(gmap *m);


gmap *gmap_create(void *(*cp)(const void *), int (*comp)(const void *, const void *), size_t (*h)(const void *s), void (*f)(void *))
{
  if (h == NULL || cp == NULL || comp == NULL || f == NULL)
    {
      // one of the required functions was missing
      return NULL;
    }
  
  gmap *result = malloc(sizeof(gmap));
  if (result != NULL)
    {
      // remember the functions used to manipulate the keys
      result->copy = cp;
      result->compare = comp;
      result->hash = h;
      result->free = f;

      // initialize the table
      result->table = calloc(GMAP_INITIAL_CAPACITY, sizeof(entry *));
      if (result->table == NULL){
        free(result);
        return NULL;
      }
      result->capacity = (result->table != NULL ? GMAP_INITIAL_CAPACITY : 0);
      result->size = 0;
    }
  return result;
}


size_t gmap_size(const gmap *m)
{
  if (m == NULL)
    {
      return 0;
    }
  
  return m->size;
}


entry *gmap_table_find_key(entry **table, const void *key, size_t (*hash)(const void *), int (*compare)(const void *, const void *), size_t capacity)
{
  size_t index = hash(key) % capacity;
  entry *find_entry = table[index];

  while(find_entry != NULL){
    if (compare(find_entry->key, key) == 0){
      return find_entry; //key found
    }
    find_entry = find_entry->next;
  }

  return NULL;
}


void *gmap_put(gmap *m, const void *key, void *value)
{
  if (m == NULL || key == NULL)
    {
      return NULL;
    }
  
  if (m->size == m->capacity)
    {
      // table is full (would that be the right condition for a hash table?); try to expand
      gmap_embiggen(m); // does nothing in this implementation
    }
  
  // find key
  size_t index = m->hash(key) % m->capacity;
  entry *existing_entry = gmap_table_find_key(m->table, key, m->hash, m->compare, m->capacity);

  if (existing_entry != NULL){
    void *old_value = existing_entry->value;
    existing_entry->value = value;
    return old_value;
  }

  //if key is not found
  entry *new_entry = malloc(sizeof(entry));
  if (new_entry == NULL){
    return gmap_error;
  }
  new_entry->key = m->copy(key);
  new_entry->value = value;
  new_entry->next = m->table[index];
  m->table[index] = new_entry;
  m->size++;

  return NULL;   
}


void *gmap_remove(gmap *m, const void *key)
{
  // TO-DO: write and test remove
size_t index = m->hash(key) % m->capacity;
entry *entry_to_remove = m->table[index];
entry *prev = NULL;

while(entry_to_remove != NULL){
  if(m->compare(entry_to_remove->key, key)== 0){
    void *value = entry_to_remove->value;

    if (prev != NULL){
      prev->next = entry_to_remove->next;
    }
    else {
      m->table[index] = entry_to_remove->next;
    }
    free(entry_to_remove->key);
    free(entry_to_remove);
    m->size--;

    return value;
  }
  prev = entry_to_remove;
  entry_to_remove = entry_to_remove->next;
}

return NULL;
}


void gmap_embiggen(gmap *m)
{
  // TO-DO: write and test embiggen
  if (m == NULL){
    return;
  }

  size_t new_capacity = m->capacity * 2;

  entry **new_table = calloc(new_capacity, sizeof(entry *));

  //Rehash and insert into new table
  for (size_t i = 0; i < m->capacity; i++){
    entry *entry_ptr = m->table[i];
    while(entry_ptr != NULL){
        entry *next = entry_ptr->next;

        size_t new_index = m->hash(entry_ptr->key) % new_capacity;

        entry_ptr->next = new_table[new_index];
        new_table[new_index] = entry_ptr;

        entry_ptr = next;
    }
  }
  //free old array
  free(m->table);
  m->table = new_table;
  m->capacity = new_capacity;
}


bool gmap_contains_key(const gmap *m, const void *key)
{
  entry *entry_ptr = gmap_table_find_key(m->table, key, m->hash, m->compare, m->capacity);
  if (entry_ptr == NULL){
    return false;
  }
  else{
    return true;
  }
}


void *gmap_get(gmap *m, const void *key)
{
  if (m == NULL || key == NULL){
    return NULL;
  }

  entry *entry_ptr = gmap_table_find_key(m->table, key, m->hash, m->compare, m->capacity);
  if (entry_ptr != NULL){
    return entry_ptr->value;
  }
  else{
    return NULL;
  }
}


/**
 * A location in an array where a key can be stored.  The location is
 * represented by a (array, index) pair.
 */
typedef struct _gmap_store_location
{
  const void **arr;
  size_t index;
} gmap_store_location;


/**
 * Stores the given key in the given location and increments the location
 * by one element.
 *
 * @param key a pointer to a key, non-NULL
 * @param value ignored
 * @param arg a pointer to a gmap_store_location
 */
void gmap_store_key_in_array(const void *key, void *value, void *arg)
{
  gmap_store_location *where = arg;
  where->arr[where->index] = key;
  where->index++;
}


const void **gmap_keys(gmap *m)
{
  const void **keys = malloc(sizeof(*keys) * m->size);

  if (keys != NULL)
    {
      gmap_store_location loc = {keys, 0};
      gmap_for_each(m, gmap_store_key_in_array, &loc);
    }

  return keys;
}


void gmap_for_each(gmap *m, void (*f)(const void *, void *, void *), void *arg)
{
  if (m == NULL || f == NULL)
    {
      return;
    }
  
  for (size_t i = 0; i < m->capacity; i++){
    entry *entry_ptr = m->table[i];

    while (entry_ptr != NULL){
        f(entry_ptr->key, entry_ptr->value, arg);
        entry_ptr = entry_ptr->next;
    }
  }
}


void gmap_destroy(gmap *m)
{
  if (m == NULL)
    {
      return;
    }
  
  // free all the copies of the keys we made + the values
  for (size_t i = 0; i < m->capacity; i++){
    entry *entry_ptr = m->table[i];

    while (entry_ptr != NULL){
      entry *next_entry = entry_ptr->next;
      m->free(entry_ptr->key);
      free(entry_ptr);
      entry_ptr = next_entry;
    }
  }
  free(m->table);
  free(m);
}
