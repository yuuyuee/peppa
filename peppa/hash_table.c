/* Copyright 2023 The Peppa Authors. */

#include "peppa/hash_table.h"

#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "peppa/alloc.h"
#include "peppa/macros.h"
#include "peppa/error.h"

typedef struct Pe_HashTableNode {
  void* elt;
  uint32_t code;
  struct Pe_HashTableNode* next;
} Pe_HashTableNode;

static inline
Pe_HashTableNode* allocNode(void* elt, uint32_t code) {
  Pe_HashTableNode* node = (Pe_HashTableNode*) Pe_alloc(sizeof(*node));
  node->elt = elt;
  node->code = code;
  node->next = NULL;
  return node;
}

static inline
void freeNode(const Pe_HashTableTraits* traits, Pe_HashTableNode* node) {
  traits->free(node->elt);
  Pe_free(node);
}

static inline
int insertNode(const Pe_HashTableTraits* traits,
                Pe_HashTableNode** h, void* elt, uint32_t code) {
  const void* key = traits->extract(elt);
  for (Pe_HashTableNode* ptr = *h; ptr; ptr = ptr->next) {
    const void* x = traits->extract(ptr->elt);
    if (traits->compare(x, key) == 0) {
      PE_CHECK2(ptr->code == code);
      return 1;
    }
  }

  Pe_HashTableNode* node = allocNode(elt, code);
  node->next = *h;
  *h = node;
  return 0;
}

static inline
void removeNode(const Pe_HashTableTraits* traits,
                Pe_HashTableNode** h, const void* key) {
  Pe_HashTableNode root;
  root.next = *h;
  for (Pe_HashTableNode* ptr = &root; ptr->next;) {
    Pe_HashTableNode* node = ptr->next;
    const void* x = traits->extract(node);
    if (traits->compare(x, key) == 0) {
      ptr->next = ptr->next->next;
      freeNode(traits, node);
      break;
    } else {
      ptr = ptr->next;
    }
  }
  *h = root.next;
}

static inline
Pe_HashTableNode* findNode(const Pe_HashTableTraits* traits,
                           Pe_HashTableNode* ptr, const void* key) {
  for (; ptr; ptr = ptr->next) {
    const void* x = traits->extract(ptr->elt);
    if (traits->compare(x, key) == 0)
      return ptr;
  }
  return NULL;
}

static inline
void forEach(const Pe_HashTableNode* ptr,
             Pe_HashTableVisitor visitor, void* opaque) {
  for (; ptr; ptr = ptr->next)
    visitor(ptr->elt, opaque);
}

static inline
void clearNode(const Pe_HashTableTraits* traits, Pe_HashTableNode** h) {
  for (Pe_HashTableNode* ptr = *h; ptr;) {
    Pe_HashTableNode* node = ptr;
    ptr = ptr->next;
    freeNode(traits, node);
  }
  *h = NULL;
}

struct Pe_HashTable {
  Pe_HashTableTraits traits;
  Pe_HashTableNode** bkts;
  size_t num_bkts;
  size_t num_elts;
};

static
unsigned int lowerBound(const unsigned int* ptr, size_t len, unsigned int val) {
  while (len > 0) {
    size_t half = len / 2;
    if (*(ptr + half) < val) {
      ptr += half + 1;
      len -= half + 1;
    } else {
      len = half;
    }
  }
  return *ptr;
}

static const float kMaxLoadFactor = 1.0;
#define Pe_getBuckets(num_elts) ((size_t) ceil(num_elts * kMaxLoadFactor))
#define Pe_getLoadFactor(num_elts, num_bkts) ((long double) num_elts / num_bkts)

static const unsigned int kMaxPrime = 4294967291ul;
static const unsigned int kPrimeList[] = {
  2ul, 3ul, 5ul, 7ul, 11ul, 13ul, 17ul, 19ul, 23ul, 29ul, 31ul,
  37ul, 41ul, 43ul, 47ul, 53ul, 59ul, 61ul, 67ul, 71ul, 73ul, 79ul,
  83ul, 89ul, 97ul, 103ul, 109ul, 113ul, 127ul, 137ul, 139ul, 149ul,
  157ul, 167ul, 179ul, 193ul, 199ul, 211ul, 227ul, 241ul, 257ul,
  277ul, 293ul, 313ul, 337ul, 359ul, 383ul, 409ul, 439ul, 467ul,
  503ul, 541ul, 577ul, 619ul, 661ul, 709ul, 761ul, 823ul, 887ul,
  953ul, 1031ul, 1109ul, 1193ul, 1289ul, 1381ul, 1493ul, 1613ul,
  1741ul, 1879ul, 2029ul, 2179ul, 2357ul, 2549ul, 2753ul, 2971ul,
  3209ul, 3469ul, 3739ul, 4027ul, 4349ul, 4703ul, 5087ul, 5503ul,
  5953ul, 6427ul, 6949ul, 7517ul, 8123ul, 8783ul, 9497ul, 10273ul,
  11113ul, 12011ul, 12983ul, 14033ul, 15173ul, 16411ul, 17749ul,
  19183ul, 20753ul, 22447ul, 24281ul, 26267ul, 28411ul, 30727ul,
  33223ul, 35933ul, 38873ul, 42043ul, 45481ul, 49201ul, 53201ul,
  57557ul, 62233ul, 67307ul, 72817ul, 78779ul, 85229ul, 92203ul,
  99733ul, 107897ul, 116731ul, 126271ul, 136607ul, 147793ul,
  159871ul, 172933ul, 187091ul, 202409ul, 218971ul, 236897ul,
  256279ul, 277261ul, 299951ul, 324503ul, 351061ul, 379787ul,
  410857ul, 444487ul, 480881ul, 520241ul, 562841ul, 608903ul,
  658753ul, 712697ul, 771049ul, 834181ul, 902483ul, 976369ul,
  1056323ul, 1142821ul, 1236397ul, 1337629ul, 1447153ul, 1565659ul,
  1693859ul, 1832561ul, 1982627ul, 2144977ul, 2320627ul, 2510653ul,
  2716249ul, 2938679ul, 3179303ul, 3439651ul, 3721303ul, 4026031ul,
  4355707ul, 4712381ul, 5098259ul, 5515729ul, 5967347ul, 6456007ul,
  6984629ul, 7556579ul, 8175383ul, 8844859ul, 9569143ul, 10352717ul,
  11200489ul, 12117689ul, 13109983ul, 14183539ul, 15345007ul,
  16601593ul, 17961079ul, 19431899ul, 21023161ul, 22744717ul,
  24607243ul, 26622317ul, 28802401ul, 31160981ul, 33712729ul,
  36473443ul, 39460231ul, 42691603ul, 46187573ul, 49969847ul,
  54061849ul, 58488943ul, 63278561ul, 68460391ul, 74066549ul,
  80131819ul, 86693767ul, 93793069ul, 101473717ul, 109783337ul,
  118773397ul, 128499677ul, 139022417ul, 150406843ul, 162723577ul,
  176048909ul, 190465427ul, 206062531ul, 222936881ul, 241193053ul,
  260944219ul, 282312799ul, 305431229ul, 330442829ul, 357502601ul,
  386778277ul, 418451333ul, 452718089ul, 489790921ul, 529899637ul,
  573292817ul, 620239453ul, 671030513ul, 725980837ul, 785430967ul,
  849749479ul, 919334987ul, 994618837ul, 1076067617ul, 1164186217ul,
  1259520799ul, 1362662261ul, 1474249943ul, 1594975441ul, 1725587117ul,
  1866894511ul, 2019773507ul, 2185171673ul, 2364114217ul, 2557710269ul,
  2767159799ul, 2993761039ul, 3238918481ul, 3504151727ul, 3791104843ul,
  4101556399ul, kMaxPrime
};

static const size_t kPrimeListSize = Pe_ARRAYSIZE(kPrimeList) - 1;

static void resizeBuckets(Pe_HashTable* table, size_t num_bkts) {
  PE_CHECK2(num_bkts >= table->num_bkts);

  /* Finds the smallest prime not less than "num_bkts". */
  num_bkts = lowerBound(kPrimeList, kPrimeListSize, num_bkts);

  /* Already reach the biggest posiible bucket number. */
  if (num_bkts >= kMaxPrime)
    return;

  size_t alloc_size = sizeof(Pe_HashTableNode*) * num_bkts;
  Pe_HashTableNode** bkts = (Pe_HashTableNode**) Pe_alloc(alloc_size);
  memset(bkts, 0, alloc_size);

  /* Rehashing exists elements to new buckets. */
  if (table->num_elts > 0) {
    for (size_t i = 0; i < table->num_bkts; ++i) {
      if (table->bkts[i]) {
        size_t index = table->bkts[i]->code % num_bkts;
        if (bkts[index]) {
          Pe_HashTableNode* ptr = bkts[index];
          while (ptr->next)
            ptr = ptr->next;
          ptr->next = table->bkts[i];
        } else {
          bkts[index] = table->bkts[i];
        }
      }
    }
    Pe_free(table->bkts);
  }
  table->num_bkts = num_bkts;
  table->bkts = bkts;
}

Pe_HashTable* PeHashTable_alloc() {
  return (Pe_HashTable*) Pe_alloc(sizeof(Pe_HashTable));
}

void PeHashTable_init(Pe_HashTable* table,
                      Pe_HashTableTraits traits, size_t n) {
  table->traits = traits;
  table->bkts = NULL;
  table->num_bkts = 0;
  table->num_elts = 0;
  resizeBuckets(table, n);
}

void PeHashTable_init2(Pe_HashTable* table, Pe_HashTableTraits traits) {
  PeHashTable_init(table, traits, 10);
}

int PeHashTable_insert(Pe_HashTable* table, void* elt) {
  size_t num_elts = table->num_elts + 1;
  if (Pe_getLoadFactor(num_elts, table->num_bkts) >= kMaxLoadFactor) {
    size_t num_bkts = Pe_getBuckets(num_elts);
    if (num_bkts >= table->num_bkts) {
      num_bkts = Pe_MAX(num_bkts, table->num_bkts * 2);
      resizeBuckets(table, num_bkts);
    }
  }

  const void* key = table->traits.extract(elt);
  uint32_t code = table->traits.hash(key);
  size_t index = code % table->num_bkts;
  int ret = insertNode(&table->traits, &table->bkts[index], elt, code);
  if (ret == 0)
    ++table->num_elts;
  return ret;
}

void PeHashTable_remove(Pe_HashTable* table, const void* key) {
  uint32_t code = table->traits.hash(key);
  size_t index = code % table->num_bkts;
  removeNode(&table->traits, &table->bkts[index], key);
  --table->num_elts;
}

void PeHashTable_remove2(Pe_HashTable* table, const void* elt) {
  PeHashTable_remove(table, table->traits.extract(elt));
}

void* PeHashTable_find(Pe_HashTable* table, const void* key) {
  uint32_t code = table->traits.hash(key);
  size_t index = code % table->num_bkts;
  Pe_HashTableNode* ptr = findNode(&table->traits, table->bkts[index], key);
  return ptr ? ptr->elt : NULL;
}

void* PeHashTable_find2(Pe_HashTable* table, const void* elt) {
  return PeHashTable_find(table, table->traits.extract(elt));
}

void PeHashTable_forEach(const Pe_HashTable* table,
                         Pe_HashTableVisitor visitor,
                         void* opaque) {
  for (size_t i = 0; i < table->num_bkts; ++i) {
    if (table->bkts[i])
      forEach(table->bkts[i], visitor, opaque);
  }
}

int PeHashTable_isEmpty(const Pe_HashTable* table) {
  return table->num_elts == 0;
}

void PeHashTable_clear(Pe_HashTable* table) {
  for (size_t i = 0; i < table->num_bkts; ++i) {
    if (table->bkts[i])
      clearNode(&table->traits, &table->bkts[i]);
  }
  table->num_elts = 0;
}

size_t PeHashTable_getSize(const Pe_HashTable* table) {
  return table->num_elts;
}

size_t PeHashTable_getBucketSize(const Pe_HashTable* table) {
  return table->num_bkts;
}

float PeHashTable_getLoadFactor(const Pe_HashTable* table) {
  return Pe_getLoadFactor(table->num_elts, table->num_bkts);
}

void PeHashTable_free(Pe_HashTable* table) {
  PeHashTable_clear(table);
  Pe_free(table->bkts);
  Pe_free(table);
}

#ifdef PE_HASHTABLE_TEST

#include <stdio.h>
#include <string.h>

#include "peppa/hash.h"
#include "peppa/format.h"

typedef struct Persion {
  char* name;
  char* addr;
} Persion;

static Persion* alloc() {
  return (Persion*) Pe_alloc(sizeof(Persion));
}

static void init(Persion* p, const char* name, const char* addr) {
  p->name = (char*) Pe_alloc(strlen(name) + 1);
  memcpy(p->name, name, strlen(name) + 1);
  p->addr = (char*) Pe_alloc(strlen(addr) + 1);
  memcpy(p->addr, addr, strlen(addr) + 1);
}

static void destroy(Persion* p) {
  Pe_free(p->name);
  Pe_free(p->addr);
  Pe_free(p);
}

static const void* extract(const void* p) {
  const Persion* ptr = (const Persion*) p;
  return ptr->name;
}

static void freeWrapper(void* p) {
  destroy(p);
}

static int compare(const void* x, const void* y) {
  const char* a = (const char*) x;
  const char* b = (const char*) y;
  return strlen(a) == strlen(b) && strncmp(a, b, strlen(a)) == 0 ? 0 : 1;
}

static uint32_t hash(const void* key) {
  const char* p = (const char*) key;
  return Pe_getHashValue(p, strlen(p));
}

static void visitor(const void* ptr, void* opaque) {
  if (opaque) {
    *(int*) opaque += 1;
  } else {
    const Persion* p = (const Persion*) ptr;
    printf("name = %s, addr = %s\n", p->name, p->addr);
  }
}

static void printTable(const Pe_HashTable* table) {
  printf("size = %zu, bucket size = %zu, "
         "empty = %s, load factor = %.2f\n",
         PeHashTable_getSize(table),
         PeHashTable_getBucketSize(table),
         (PeHashTable_isEmpty(table) ? "True" : "False"),
         PeHashTable_getLoadFactor(table));
}

int main() {
  Pe_HashTableTraits traits = {
    extract, freeWrapper, compare, hash
  };
  Pe_HashTable* table = PeHashTable_alloc();
  PeHashTable_init2(table, traits);
  printTable(table);

  Persion* p = alloc();
  init(p, "Alice", "Queen street");
  int ret = PeHashTable_insert(table, p);
  PE_CHECK2(ret == 0);
  printTable(table);

  p = alloc();
  init(p, "Jinks", "Picheng");
  ret = PeHashTable_insert(table, p);
  PE_CHECK2(ret == 0);
  printTable(table);

  p = alloc();
  init(p, "Anduyin", "Stormwind");
  ret = PeHashTable_insert(table, p);
  PE_CHECK2(ret == 0);
  printTable(table);

  char name[20];
  char addr[20];
  for (int i = 0; i < 20; ++i) {
    Pe_format(name, 19, "Anduyin-%d", i);
    Pe_format(addr, 19, "Stormwind-%d", i);
    p = alloc();
    init(p, name, addr);
    ret = PeHashTable_insert(table, p);
    PE_CHECK2(ret == 0);

    int count = 0;
    PeHashTable_forEach(table, visitor, &count);
    size_t n = PeHashTable_getSize(table);
    PE_CHECK2(count == n);

  printf("****\n");
  PeHashTable_forEach(table, visitor, NULL);
  printf("****\n");

  p = (Persion*) PeHashTable_find(table, "Anduyin");
  PE_CHECK2(p != NULL);
  }
  printTable(table);

  p = (Persion*) PeHashTable_find(table, "Anduyin");
  PE_CHECK2(p != NULL);
  p = (Persion*) PeHashTable_find(table, "Varian");
  PE_CHECK2(p == NULL);

  PeHashTable_remove(table, "Alice");
  printTable(table);

  printf("****\n");
  PeHashTable_forEach(table, visitor, NULL);
  printf("****\n");

  PeHashTable_clear(table);
  printTable(table);

  printf("****\n");
  PeHashTable_forEach(table, visitor, NULL);
  printf("****\n");

  PeHashTable_free(table);
  return 0;
}

#endif
