// Copyright 2023 The Peppa Authors.

extern "C" {
#include "peppa/list.h"
}

#include <vector>
#include "gtest/gtest.h"

namespace {

struct Data {
  int value;
  Pe_ListNode node;

  static int kConstructCount;
  static int kDestructCount;
  Data() { ++kConstructCount; }
  ~Data() { ++kDestructCount; }
};

int Data::kConstructCount = 0;
int Data::kDestructCount = 0;

std::vector<int> RingToVector(Pe_ListNode* h) {
  std::vector<int> vec;
  Pe_ListNode* n;
  PeList_foreach(n, h) {
    Data* p = Pe_CONTAINER_OF(n, Data, node);
    vec.push_back(p->value);
  }
  return vec;
}

void FreeList(Pe_ListNode* h) {
  while (!PeList_empty(h)) {
    Pe_ListNode* n = PeList_next(h);
    PeList_remove(n);
    Data* p = Pe_CONTAINER_OF(n, Data, node);
    delete p;
  }
}

TEST(RingTest, InsertHead) {
  Pe_ListNode h;
  Data* p = nullptr;

  PeList_init(&h);

  for (int i = 0; i < 10; ++i) {
    p = new Data;
    p->value = i;
    PeList_init(&p->node);
    PeList_insertHead(&h, &p->node);
  }

  Pe_ListNode* c;
  int index = 10;
  PeList_foreach(c, &h) {
    Data* p = Pe_CONTAINER_OF(c, Data, node);
    EXPECT_EQ(p->value, --index);
  }

  FreeList(&h);
  EXPECT_EQ(Data::kConstructCount, Data::kDestructCount);
}

TEST(RingTest, InsertTail) {
  Pe_ListNode h;
  Data* p = nullptr;

  PeList_init(&h);

  for (int i = 0; i < 10; ++i) {
    p = new Data;
    p->value = i;
    PeList_init(&p->node);
    PeList_insertTail(&h, &p->node);
  }

  Pe_ListNode* c;
  int index = -1;
  PeList_foreach(c, &h) {
    Data* p = Pe_CONTAINER_OF(c, Data, node);
    EXPECT_EQ(p->value, ++index);
  }

  FreeList(&h);
  EXPECT_EQ(Data::kConstructCount, Data::kDestructCount);
}

TEST(RingTest, ADD) {
  Pe_ListNode h1, h2;
  Data* p = nullptr;
  std::vector<int> vec1, vec2, vec3;

  PeList_init(&h1);
  PeList_init(&h2);

  for (int i = 0; i < 5; ++i) {
    p = new Data;
    p->value = i;
    PeList_init(&p->node);

    if (i % 2 == 0) {
      PeList_insertTail(&h1, &p->node);
      vec1.push_back(p->value);
    } else {
      PeList_insertTail(&h2, &p->node);
      vec2.push_back(p->value);
    }
  }

  for (const auto& v : vec2)
    vec1.push_back(v);

  PeList_add(&h1, &h2);
  PeList_init(&h2);
  std::vector<int> exp1{0, 2, 4, 1, 3};
  EXPECT_EQ(vec1, exp1);

  FreeList(&h1);
  FreeList(&h2);
  EXPECT_EQ(Data::kConstructCount, Data::kDestructCount);
}

TEST(RingTest, SplitMove) {
  Pe_ListNode h1, h2, h3, h4;
  Data* p = nullptr;

  PeList_init(&h1);
  PeList_init(&h2);
  PeList_init(&h3);
  PeList_init(&h4);

  for (int i = 0; i < 5; ++i) {
    p = new Data;
    p->value = i;
    PeList_init(&p->node);
    if (i % 2 == 0) {
      PeList_insertTail(&h1, &p->node);
    } else {
      PeList_insertTail(&h2, &p->node);
    }
  }

  // test PeList_split
  // h1 -> 0, 2, 4
  Pe_ListNode* n = PeList_next(PeList_next(&h1));
  p = Pe_CONTAINER_OF(n, Data, node);
  EXPECT_EQ(p->value, 2);
  PeList_split(&h1, n, &h3);
  std::vector<int> exp1{0};
  std::vector<int> exp3{2, 4};
  EXPECT_EQ(RingToVector(&h1), exp1);
  EXPECT_EQ(RingToVector(&h3), exp3);

  // test PeList_move
  // h2 -> 1, 3
  PeList_move(&h2, &h4);
  EXPECT_TRUE(PeList_empty(&h2));
  std::vector<int> exp4{1, 3};
  EXPECT_EQ(RingToVector(&h4), exp4);

  FreeList(&h1);
  FreeList(&h2);
  FreeList(&h3);
  FreeList(&h4);
  EXPECT_EQ(Data::kConstructCount, Data::kDestructCount);
}

}  // anonymous namespace
