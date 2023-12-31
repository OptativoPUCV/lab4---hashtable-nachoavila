#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
  long k = hash(key, map->capacity);
  long first = k;

  while(1)
  {
    Pair * current = map->buckets[k];
    if(current == NULL || current->key == NULL)
    {
      Pair * newPair = (Pair*)malloc(sizeof(Pair));
      newPair->key = strdup(key);
      newPair->value = value;
      map->size++;
      map->buckets[k] = newPair;
      map->current = k;
      return;
    }

    k = (k + 1) % map->capacity;
    if(k == first) return;
  }

}

void enlarge(HashMap * map) {
  enlarge_called = 1; //no borrar (testing purposes)
  Pair** oldBucket = map->buckets;
  long oldCapacity = map->capacity;
  map->capacity *= 2;
  map->buckets = NULL;
  map->buckets = (Pair**)calloc(map->capacity, sizeof(Pair*));
  map->size = 0;

  for(long k = 0 ; k < oldCapacity ; k++)
  {
    Pair* current = oldBucket[k];
    if(current != NULL && current->key != NULL)
    {
      insertMap(map, current->key, current->value);
      free(current->key);
      free(current);
    }  
  }
}


HashMap * createMap(long capacity) {
  HashMap* mapaHash = (HashMap*)malloc(sizeof(HashMap));
  mapaHash->buckets = (Pair**)malloc(10 * sizeof(Pair*));
  mapaHash->size = 0;
  mapaHash->capacity = capacity;
  mapaHash->current = -1;
  
  return mapaHash;
}

void eraseMap(HashMap * map,  char * key) {    
  long k = hash(key, map->capacity);
  long first = k;

  while(1)
  {
    Pair * current = map->buckets[k];
    if(current == NULL) return;
    
    if(strcmp(current->key, key) == 0)
    {
      current->key = NULL;
      map->size--;
      return;
    }

    k = (k + 1) % map->capacity;
    if(k == first) return;
  }

  return;
}

Pair * searchMap(HashMap * map,  char * key) {   
  long k = hash(key, map->capacity);
  long first = k;

  while(1)
  {
    Pair * current = map->buckets[k];  
    if(current == NULL) return NULL;

    if(strcmp(current->key, key) == 0)
    {
      map->current = k;
      return current;
    }

    k = (k + 1) % map->capacity;
    if(k == first) return NULL;
  }  
  
  return NULL;
}

Pair * firstMap(HashMap * map) {
  long k = 0;
  long first = k;
  Pair * current = map->buckets[k];
  
  while(1)
  {
    current = map->buckets[k];
    if(current != NULL && current->key != NULL) break;
    k = (k + 1) % map->capacity;
    if(k == first) return NULL;
  }

  map->current = k;
  return current;
}

Pair * nextMap(HashMap * map) {
  long k = map->current + 1;
  Pair * current = map->buckets[k];

  while(1)
  {
    current = map->buckets[k];
    if(current != NULL && current->key != NULL) break;
    k++;
    if(k == map->capacity) return NULL;
  }  

  map->current = k;
  return current;
}
