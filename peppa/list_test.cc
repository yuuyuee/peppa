// Copyright 2023 The Peppa Authors.

#include <vector>
#include "peppa/list.h"
#include "gtest/gtest.h"

namespace {

struct Data {
  int value;
  PeNode node;

  static int kConstructCount;
  static int kDestructCount;
  Data() { ++kConstructCount; }
  ~Data() { ++kDestructCount; }
};

int Data::kConstructCount = 0;
int Data::kDestructCount = 0;

std::vector<int> RingToVector(PeNode* h) {
  std::vector<int> vec;
  PeNode* n;
  PeList_FOREACH(n, h) {
    Data* p = PeList_DATA(n, Data, node);
    vec.push_back(p->value);
  }
  return vec;
}

void FreeRing(PeNode* h) {
  while (!PeList_EMPTY(h)) {
    PeNode* n = PeList_NEXT(h);
    PeList_REMOVE(n);
    Data* p = PeList_DATA(n, Data, node);
    delete p;
  }
}

TEST(RingTest, InsertHead) {
  PeNode h;
  Data* p = nullptr;

  PeList_INIT(&h);

  for (int i = 0; i < 10; ++i) {
    p = new Data;
    p->value = i;
    PeList_INIT(&p->node);
    PeList_INSERT_HEAD(&h, &p->node);
  }

  PeNode* c;
  int index = 10;
  PeList_FOREACH(c, &h) {
    Data* p = PeList_DATA(c, Data, node);
    EXPECT_EQ(p->value, --index);
  }

  FreeRing(&h);
  EXPECT_EQ(Data::kConstructCount, Data::kDestructCount);
}

TEST(RingTest, InsertTail) {
  PeNode h;
  Data* p = nullptr;

  PeList_INIT(&h);

  for (int i = 0; i < 10; ++i) {
    p = new Data;
    p->value = i;
    PeList_INIT(&p->node);
    PeList_INSERT_TAIL(&h, &p->node);
  }

  PeNode* c;
  int index = -1;
  PeList_FOREACH(c, &h) {
    Data* p = PeList_DATA(c, Data, node);
    EXPECT_EQ(p->value, ++index);
  }

  FreeRing(&h);
  EXPECT_EQ(Data::kConstructCount, Data::kDestructCount);
}

TEST(RingTest, ADD) {
  PeNode h1, h2;
  Data* p = nullptr;
  std::vector<int> vec1, vec2, vec3;

  PeList_INIT(&h1);
  PeList_INIT(&h2);

  for (int i = 0; i < 5; ++i) {
    p = new Data;
    p->value = i;
    PeList_INIT(&p->node);

    if (i % 2 == 0) {
      PeList_INSERT_TAIL(&h1, &p->node);
      vec1.push_back(p->value);
    } else {
      PeList_INSERT_TAIL(&h2, &p->node);
      vec2.push_back(p->value);
    }
  }

  for (const auto& v : vec2)
    vec1.push_back(v);

  PeList_ADD(&h1, &h2);
  PeList_INIT(&h2);
  std::vector<int> exp1{0, 2, 4, 1, 3};
  EXPECT_EQ(vec1, exp1);

  FreeRing(&h1);
  FreeRing(&h2);
  EXPECT_EQ(Data::kConstructCount, Data::kDestructCount);
}

TEST(RingTest, SplitMove) {
  PeNode h1, h2, h3, h4;
  Data* p = nullptr;

  PeList_INIT(&h1);
  PeList_INIT(&h2);
  PeList_INIT(&h3);
  PeList_INIT(&h4);

  for (int i = 0; i < 5; ++i) {
    p = new Data;
    p->value = i;
    PeList_INIT(&p->node);
    if (i % 2 == 0) {
      PeList_INSERT_TAIL(&h1, &p->node);
    } else {
      PeList_INSERT_TAIL(&h2, &p->node);
    }
  }

  // test PeList_SPLIT
  // h1 -> 0, 2, 4
  PeNode* n = PeList_NEXT(PeList_NEXT(&h1));
  p = PeList_DATA(n, Data, node);
  EXPECT_EQ(p->value, 2);
  PeList_SPLIT(&h1, n, &h3);
  std::vector<int> exp1{0};
  std::vector<int> exp3{2, 4};
  EXPECT_EQ(RingToVector(&h1), exp1);
  EXPECT_EQ(RingToVector(&h3), exp3);

  // test PeList_MOVE
  // h2 -> 1, 3
  PeList_MOVE(&h2, &h4);
  EXPECT_TRUE(PeList_EMPTY(&h2));
  std::vector<int> exp4{1, 3};
  EXPECT_EQ(RingToVector(&h4), exp4);

  FreeRing(&h1);
  FreeRing(&h2);
  FreeRing(&h3);
  FreeRing(&h4);
  EXPECT_EQ(Data::kConstructCount, Data::kDestructCount);
}

}  // anonymous namespace
