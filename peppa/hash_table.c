/* Copyright 2023 The Peppa Authors. */

#include "peppa/hash_table.h"

#include <math.h>

#include "peppa/alloc.h"
#include "peppa/macros.h"

#define PE_DIVISION_METHOD 1

typedef struct Pe_HashTableNode {
  void* elt;
  uint32_t hash_code;
  Pe_HashTableNode* next;
} Pe_HashTableNode;

struct Pe_HashTable {
  Pe_HashTableTraits traits;
  Pe_HashTableNode** bkts;
  size_t num_bkts;
  size_t num_elts;
};

static uint64_t lowerBound(const uint64_t* ptr, size_t len, uint64_t val) {
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

static Pe_HashTableNode* allocNode(void* elt) {
  Pe_HashTableNode* node = Pe_CAST(Pe_HashTableNode*, Pe_alloc(sizeof(node)));
  node->elt = elt;
  node->hash_code = 0;
  node->next = NULL;
}

static const double kMaxLoadFactor = 1.0;
#define Pe_getBuckets(num_elts) ceil(num_elts * kMaxLoadFactor)
#define Pe_getLoadFactor(num_elts, num_bkts) (num_elts / num_bkts)

static const uint64_t kMaxPrime = 18446744073709551557ul;
static const uint64_t kPrimeList[] = {
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
  4101556399ul, 4294967291ul, 442450933ul, 8589934583ul, 12884901857ul,
  17179869143ul, 25769803693ul, 34359738337ul, 51539607367ul, 68719476731ul,
  103079215087ul, 137438953447ul, 206158430123ul, 274877906899ul,
  412316860387ul, 549755813881ul, 824633720731ul, 1099511627689ul,
  1649267441579ul, 2199023255531ul, 3298534883309ul, 4398046511093ul,
  6597069766607ul, 8796093022151ul, 13194139533241ul, 17592186044399ul,
  26388279066581ul, 35184372088777ul, 52776558133177ul, 70368744177643ul,
  105553116266399ul, 140737488355213ul, 211106232532861ul, 281474976710597ul,
  562949953421231ul, 1125899906842597ul, 2251799813685119ul,
  4503599627370449ul, 9007199254740881ul, 18014398509481951ul,
  36028797018963913ul, 72057594037927931ul, 144115188075855859ul,
  288230376151711717ul, 576460752303423433ul, 1152921504606846883ul,
  2305843009213693951ul, 4611686018427387847ul, 9223372036854775783ul,
  kMaxPrime, kMaxPrime
};

static const size_t kPrimeListSize = Pe_ARRAYSIZE(kPrimeList) - 1;

Pe_HashTable* PeHashTable_alloc() {
  return Pe_CAST(Pe_HashTable*, Pe_alloc(sizeof(Pe_HashTable)));
}

void PeHashTable_init(Pe_HashTable* table, Pe_HashTableTraits traits, size_t n) {
  table->traits = traits;
  table->bkts = NULL;
  table->num_bkts = 0;
  table->num_elts = 0;
  if (n > 0) {
    size_t num_bkts = lowerBound(kPrimeList, kPrimeListSize, n);
    size_t alloc_size = sizeof(Pe_HashTableNode*) * num_bkts;
    Pe_HashTableNode** bkts = Pe_CAST(Pe_HashTableNode**, Pe_alloc(alloc_size));
    memset(bkts, 0, alloc_size);
    table->num_bkts = num_bkts;
    table->bkts = bkts;
  }
}

void PeHashTable_init2(Pe_HashTable* table, Pe_HashTableTraits traits) {
  PeHashTable_init(table, traits, 10);
}

void PeHashTable_add(Pe_HashTable* table, void* elt) {
  size_t num_elts = table->num_elts + 1;
  if (Pe_getLoadFactor(num_elts, table->num_bkts) > kMaxLoadFactor) {
    size_t num_bkts = Pe_getBuckets(num_elts);
    if (num_bkts >= table->num_bkts) {
      num_bkts = Pe_MAX(num_bkts + 1, table->num_bkts * 2);
      num_bkts = lowerBound(kPrimeList, kPrimeListSize, num_bkts);
      size_t alloc_size = sizeof(Pe_HashTableNode*) * num_bkts;
      Pe_HashTableNode** bkts = Pe_CAST(Pe_HashTableNode**, Pe_alloc(alloc_size));
      memset(bkts, 0, alloc_size);
      for (size_t i = 0; i < table->num_bkts; ++i) {
        Pe_HashTableNode* node = table->bkts[i];
        if (node)
          bkts[node->hash_code % num_bkts] = node;
      }
      Pe_free(table->bkts);
      table->bkts = bkts;
      table->num_bkts = num_bkts;
    }
  }

  void* key = table->traits.extract(elt);
  uint32_t hash_code = table->traits.hash(key);
  uint32_t index = hash_code % table->num_bkts;
  Pe_HashTableNode* ptr = table->bkts[index];
  while (ptr) {
    const void* x = table->traits.extract(ptr->elt);
    const void* y = table->traits.extract(elt);
    if (table->traits.compare(x, y) == 0) {
      table->traits.free(ptr->elt);
      ptr->elt = elt;
      return;
    }
  }
  Pe_HashTableNode* node = allocNode(elt);
  node->hash_code = index;
  node->next = table->bkts[index];
  table->bkts[index] = node;
}

void PeHashTable_remove(Pe_HashTable* table, const void* key) {
  uint32_t hash_code = table->traits.hash(key);
  uint32_t index = hash_code % table->num_bkts;
  Pe_HashTableNode* ptr = table->bkts[index];
  while (ptr) {
    Pe_HashTableNode* tmp = ptr;
    ptr = ptr->next;
    const void* x = table->traits.extract(tmp->elt);
    if (table->traits.compare(x, key) == 0) {
      table->traits.free(tmp->elt);
      Pe_free(tmp);
      break;
    }
  }
}

void PeHashTable_remove2(Pe_HashTable* table, const void* elt) {
  PeHashTable_remove(table, table->traits.extract(elt));
}

void* PeHashTable_find(Pe_HashTable* table, const void* key) {

}

void* PeHashTable_find2(Pe_HashTable* table, const void* elt) {
  return PeHashTable_find()
}

void PeHashTable_foreach(Pe_HashTable* table,
                         Pe_HashTableVisitor visitor,
                         void* opaque) {

}

int PeHashTable_isEmpty(Pe_HashTable* table) {
  return table->num_elts == 0;
}

void PeHashTable_clear(Pe_HashTable* table) {
  for (size_t i = 0; i < table->num_bkts; ++i) {
    Pe_HashTableNode* node = table->bkts[i];
    Pe_HashTableNode* next;
    while (node) {
      next = node->next;
      table->traits.free(node->elt);
      Pe_free(node);
      node = next;
    }
    table->bkts[i] = NULL;
  }
  table->num_elts = 0;
}

void PeHashTable_free(Pe_HashTable* table) {
  PeHashTable_clear(table);
  Pe_free(table->bkts);
  Pe_free(table);
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
  free(p);
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
