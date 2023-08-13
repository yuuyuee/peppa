/* Copyright 2023 The Peppa Authors. */

#include "peppa/hash_table.h"

#include <math.h>

#include "peppa/alloc.h"
#include "peppa/macros.h"

typedef struct Pe_HashTableNode {
  void* elt;
  uint32_t hash;
  Pe_HashTableNode* next;
} Pe_HashTableNode;

struct Pe_HashTable {
  Pe_HashTableTraits traits;
  Pe_HashTableNode** nodes;
  size_t num_bkts;
  size_t num_elts;
};



static const uint32_t kPrimeList[] = {
  2u, 3u, 5u, 7u, 11u, 13u, 17u, 19u, 23u, 29u, 31u,
  37u, 41u, 43u, 47u, 53u, 59u, 61u, 67u, 71u, 73u, 79u,
  83u, 89u, 97u, 103u, 109u, 113u, 127u, 137u, 139u, 149u,
  157u, 167u, 179u, 193u, 199u, 211u, 227u, 241u, 257u,
  277u, 293u, 313u, 337u, 359u, 383u, 409u, 439u, 467u,
  503u, 541u, 577u, 619u, 661u, 709u, 761u, 823u, 887u,
  953u, 1031u, 1109u, 1193u, 1289u, 1381u, 1493u, 1613u,
  1741u, 1879u, 2029u, 2179u, 2357u, 2549u, 2753u, 2971u,
  3209u, 3469u, 3739u, 4027u, 4349u, 4703u, 5087u, 5503u,
  5953u, 6427u, 6949u, 7517u, 8123u, 8783u, 9497u, 10273u,
  11113u, 12011u, 12983u, 14033u, 15173u, 16411u, 17749u,
  19183u, 20753u, 22447u, 24281u, 26267u, 28411u, 30727u,
  33223u, 35933u, 38873u, 42043u, 45481u, 49201u, 53201u,
  57557u, 62233u, 67307u, 72817u, 78779u, 85229u, 92203u,
  99733u, 107897u, 116731u, 126271u, 136607u, 147793u,
  159871u, 172933u, 187091u, 202409u, 218971u, 236897u,
  256279u, 277261u, 299951u, 324503u, 351061u, 379787u,
  410857u, 444487u, 480881u, 520241u, 562841u, 608903u,
  658753u, 712697u, 771049u, 834181u, 902483u, 976369u,
  1056323u, 1142821u, 1236397u, 1337629u, 1447153u, 1565659u,
  1693859u, 1832561u, 1982627u, 2144977u, 2320627u, 2510653u,
  2716249u, 2938679u, 3179303u, 3439651u, 3721303u, 4026031u,
  4355707u, 4712381u, 5098259u, 5515729u, 5967347u, 6456007u,
  6984629u, 7556579u, 8175383u, 8844859u, 9569143u, 10352717u,
  11200489u, 12117689u, 13109983u, 14183539u, 15345007u,
  16601593u, 17961079u, 19431899u, 21023161u, 22744717u,
  24607243u, 26622317u, 28802401u, 31160981u, 33712729u,
  36473443u, 39460231u, 42691603u, 46187573u, 49969847u,
  54061849u, 58488943u, 63278561u, 68460391u, 74066549u,
  80131819u, 86693767u, 93793069u, 101473717u, 109783337u,
  118773397u, 128499677u, 139022417u, 150406843u, 162723577u,
  176048909u, 190465427u, 206062531u, 222936881u, 241193053u,
  260944219u, 282312799u, 305431229u, 330442829u, 357502601u,
  386778277u, 418451333u, 452718089u, 489790921u, 529899637u,
  573292817u, 620239453u, 671030513u, 725980837u, 785430967u,
  849749479u, 919334987u, 994618837u, 1076067617u, 1164186217u,
  1259520799u, 1362662261u, 1474249943u, 1594975441u, 1725587117u,
  1866894511u, 2019773507u, 2185171673u, 2364114217u, 2557710269u,
  2767159799u, 2993761039u, 3238918481u, 3504151727u, 3791104843u,
  4101556399u, 4294967291u, 4294967291u
};

static const size_t kPrimeListSize = Pe_ARRAYSIZE(kPrimeList);

static const float kLoadFactor = 1.0;

static uint32_t binarySearch(const uint32_t* first, size_t len, uint32_t val) {
  while (len > 0) {
    size_t half = len >> 1;
    const uint32_t* middle = first;
    if (*middle < val) {
      first = middle;
      ++first;
    } else {
      len = half;
    }
  }
  return first;
}

static inline
size_t Pe_getBucket(size_t num_elts, long double load_factor) {
  return ceil(num_elts / load_factor);
}

static inline
size_t Pe_hashing(size_t num, size_t den) {
#ifdef PE_DIVISION_METHOD
  /* For modulo range hashing, use division to fold a large */

#else /* Multiplication method */

#endif
}


/* Return a bucket size no smaller than n. */
static inline
size_t Pe_getNextBucket(size_t n) {
#ifdef PE_DIVISION_METHOD

#else /* Multiplication method */

#endif
}

Pe_HashTable* PeHashTable_alloc() {
  return Pe_CAST(Pe_HashTable*, Pe_alloc(sizeof(Pe_HashTable)));
}

void PeHashTable_init(Pe_HashTable* table, Pe_HashTableTraits traits) {
  table->traits = traits;
  table->load_factor = 1.0;
  table->num_bkts = 10;
  table->num_elts = 0;
}

void PeHashTable_add(Pe_HashTable* table, void* elt) {

}

void PeHashTable_remove(Pe_HashTable* table, const void* key) {

}

void PeHashTable_remove2(Pe_HashTable* table, const void* elt) {
  PeHashTable_remove(table, table->traits.extract(elt));
}

void* PeHashTable_find(Pe_HashTable* table, const void* key) {

}

void* PeHashTable_find2(Pe_HashTable* table, const void* elt) {

}

void PeHashTable_foreach(Pe_HashTable* table,
                         Pe_HashTableVisitor visitor,
                         void* opaque) {

}

int PeHashTable_isEmpty(Pe_HashTable* table) {
  return table->num_elts == 0;
}

void PeHashTable_clear(Pe_HashTable* table) {

}

void PeHashTable_free(Pe_HashTable* table) {

}

#define PE_HASHTABLE_
#ifdef PE_HASHTABLE_

#include <stdio.h>
#include <string.h>

#include "peppa/hash.h"

typedef struct Persion {
  const char* name;
  const char* addr;
} Persion;

static Persion* alloc() {
  return Pe_alloc(sizeof(Persion));
}

static void init(Persion* p, const char* name, const char* addr) {
  p->name = name;
  p->addr = addr;
}

static void free(Persion* p) {
  /* destroy object */
  Pe_free(p);
}

static const void* extract(const void* p) {
  const Persion* ptr = Pe_CAST(const Persion*, p);
  return ptr->name;
}

static void freeWrapper(void* p) {
  free(Pe_CAST(Persion*, p));
}

int compare(const void* x, const void* y) {
  const Persion* a = Pe_CAST(const Persion*, x);
  const Persion* b = Pe_CAST(const Persion*, y);
  return strlen(a->name) == strlen(b->name) &&
      strcmp(a->name, b->name) == 0;
}

uint32_t hash(const void* key) {
  const char* p = Pe_CAST(const char*, key);
  return Pe_getHashValue(p, strlen(p));
}

static Pe_HashTableTraits traits = {
  extract, freeWrapper, compare, hash
};

int main() {
  Pe_HashTableTraits traits;
  Pe_HashTable* table = PeHashTable_alloc();
  PeHashTable_init(table, traits);

  Persion* p = alloc();
  init(p, "Alice", "Queen street");
  PeHashTable_add(table, p);

  p = alloc();
  init(p, "Jinks", "Picheng");
  PeHashTable_add(table, p);


  PeHashTable_free(table);
  return 0;
}

#endif
