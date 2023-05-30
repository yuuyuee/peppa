// Copyright 2023 The Peppa Authors.

#include <vector>
#include "peppa/internal/ring.h"
#include "gtest/gtest.h"

namespace {

struct Data {
  int value;
  Ring node;

  static int kConstructCount;
  static int kDestructCount;
  Data() { ++kConstructCount; }
  ~Data() { ++kDestructCount; }
};

int Data::kConstructCount = 0;
int Data::kDestructCount = 0;

std::vector<int> RingToVector(Ring* h) {
  std::vector<int> vec;
  Ring* n;
  PP_RING_FOREACH(n, h) {
    Data* p = PP_RING_DATA(n, Data, node);
    vec.push_back(p->value);
  }
  return vec;
}

void FreeRing(Ring* h) {
  while (!PP_RING_EMPTY(h)) {
    Ring* n = PP_RING_NEXT(h);
    PP_RING_REMOVE(n);
    Data* p = PP_RING_DATA(n, Data, node);
    delete p;
  }
}

TEST(RingTest, InsertHead) {
  Ring h;
  Data* p = nullptr;

  PP_RING_INIT(&h);

  for (int i = 0; i < 10; ++i) {
    p = new Data;
    p->value = i;
    PP_RING_INIT(&p->node);
    PP_RING_INSERT_HEAD(&h, &p->node);
  }

  Ring* c;
  int index = 10;
  PP_RING_FOREACH(c, &h) {
    Data* p = PP_RING_DATA(c, Data, node);
    EXPECT_EQ(p->value, --index);
  }

  FreeRing(&h);
  EXPECT_EQ(Data::kConstructCount, Data::kDestructCount);
}

TEST(RingTest, InsertTail) {
  Ring h;
  Data* p = nullptr;

  PP_RING_INIT(&h);

  for (int i = 0; i < 10; ++i) {
    p = new Data;
    p->value = i;
    PP_RING_INIT(&p->node);
    PP_RING_INSERT_TAIL(&h, &p->node);
  }

  Ring* c;
  int index = -1;
  PP_RING_FOREACH(c, &h) {
    Data* p = PP_RING_DATA(c, Data, node);
    EXPECT_EQ(p->value, ++index);
  }

  FreeRing(&h);
  EXPECT_EQ(Data::kConstructCount, Data::kDestructCount);
}

TEST(RingTest, ADD) {
  Ring h1, h2;
  Data* p = nullptr;
  std::vector<int> vec1, vec2, vec3;

  PP_RING_INIT(&h1);
  PP_RING_INIT(&h2);

  for (int i = 0; i < 5; ++i) {
    p = new Data;
    p->value = i;
    PP_RING_INIT(&p->node);

    if (i % 2 == 0) {
      PP_RING_INSERT_TAIL(&h1, &p->node);
      vec1.push_back(p->value);
    } else {
      PP_RING_INSERT_TAIL(&h2, &p->node);
      vec2.push_back(p->value);
    }
  }

  for (const auto& v : vec2)
    vec1.push_back(v);

  PP_RING_ADD(&h1, &h2);
  PP_RING_INIT(&h2);
  std::vector<int> exp1{0, 2, 4, 1, 3};
  EXPECT_EQ(vec1, exp1);

  FreeRing(&h1);
  FreeRing(&h2);
  EXPECT_EQ(Data::kConstructCount, Data::kDestructCount);
}

TEST(RingTest, SplitMove) {
  Ring h1, h2, h3, h4;
  Data* p = nullptr;

  PP_RING_INIT(&h1);
  PP_RING_INIT(&h2);
  PP_RING_INIT(&h3);
  PP_RING_INIT(&h4);

  for (int i = 0; i < 5; ++i) {
    p = new Data;
    p->value = i;
    PP_RING_INIT(&p->node);
    if (i % 2 == 0) {
      PP_RING_INSERT_TAIL(&h1, &p->node);
    } else {
      PP_RING_INSERT_TAIL(&h2, &p->node);
    }
  }

  // test PP_RING_SPLIT
  // h1 -> 0, 2, 4
  Ring* n = PP_RING_NEXT(PP_RING_NEXT(&h1));
  p = PP_RING_DATA(n, Data, node);
  EXPECT_EQ(p->value, 2);
  PP_RING_SPLIT(&h1, n, &h3);
  std::vector<int> exp1{0};
  std::vector<int> exp3{2, 4};
  EXPECT_EQ(RingToVector(&h1), exp1);
  EXPECT_EQ(RingToVector(&h3), exp3);

  // test PP_RING_MOVE
  // h2 -> 1, 3
  PP_RING_MOVE(&h2, &h4);
  EXPECT_TRUE(PP_RING_EMPTY(&h2));
  std::vector<int> exp4{1, 3};
  EXPECT_EQ(RingToVector(&h4), exp4);

  FreeRing(&h1);
  FreeRing(&h2);
  FreeRing(&h3);
  FreeRing(&h4);
  EXPECT_EQ(Data::kConstructCount, Data::kDestructCount);
}

}  // anonymous namespace
