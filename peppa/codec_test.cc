// Copyright 2022 The Peppa Authors.

#include "peppa/codec.h"
#include "gtest/gtest.h"

namespace {

TEST(CodecTest, AccessInt) {
  char buffer[8];
  for (int i = -100; i < 100; ++i) {
    // access int8_t
    SetInt8(buffer, i);
    EXPECT_EQ(GetInt8(buffer), i);

    // access int16_t
    SetInt16(buffer, i);
    EXPECT_EQ(GetInt16(buffer), i);

    // access int32_t
    SetInt32(buffer, i);
    EXPECT_EQ(GetInt32(buffer), i);

    // access int64_t
    SetInt64(buffer, i);
    EXPECT_EQ(GetInt64(buffer), i);
  }

  // access int8_t min/max
  SetInt8(buffer, INT8_MIN);
  EXPECT_EQ(GetInt8(buffer), INT8_MIN);

  SetInt8(buffer, INT8_MAX);
  EXPECT_EQ(GetInt8(buffer), INT8_MAX);

  // access int16_t/uint16_t min/max
  SetInt16(buffer, INT16_MIN);
  EXPECT_EQ(GetInt16(buffer), INT16_MIN);

  SetInt16(buffer, INT16_MAX);
  EXPECT_EQ(GetInt16(buffer), INT16_MAX);

  // access int32_t/uint32_t min/max
  SetInt32(buffer, INT32_MIN);
  EXPECT_EQ(GetInt32(buffer), INT32_MIN);

  SetInt32(buffer, INT32_MAX);
  EXPECT_EQ(GetInt32(buffer), INT32_MAX);

  // access int64_t/uint64_t min/max
  SetInt64(buffer, INT64_MIN);
  EXPECT_EQ(GetInt64(buffer), INT64_MIN);

  SetInt64(buffer, INT64_MAX);
  EXPECT_EQ(GetInt64(buffer), INT64_MAX);
}

TEST(CodecTest, AccessUInt) {
  char buffer[8];
  for (unsigned int i = 0; i < 100; ++i) {
    // access uint8_t
    SetUInt8(buffer, i);
    EXPECT_EQ(GetUInt8(buffer), i);

    // access uint16_t
    SetUInt16(buffer, i);
    EXPECT_EQ(GetUInt16(buffer), i);

    // access uint32_t
    SetUInt32(buffer, i);
    EXPECT_EQ(GetUInt32(buffer), i);

    // access uint64_t
    SetUInt64(buffer, i);
    EXPECT_EQ(GetUInt64(buffer), i);
  }

  // access uint8_t max
  SetInt8(buffer, UINT8_MAX);
  EXPECT_EQ(GetUInt8(buffer), UINT8_MAX);

  // access uint16_t max
  SetUInt16(buffer, UINT16_MAX);
  EXPECT_EQ(GetUInt16(buffer), UINT16_MAX);

  // access uint32_t max
  SetUInt32(buffer, UINT32_MAX);
  EXPECT_EQ(GetUInt32(buffer), UINT32_MAX);

  // access uint64_t max
  SetUInt64(buffer, UINT64_MAX);
  EXPECT_EQ(GetUInt64(buffer), UINT64_MAX);
}

TEST(CodecTest, AccessZigZag) {
  for (int i = -100; i < 100; ++i) {
    // access int8_t
    EXPECT_EQ(ZigZagDecode8(ZigZagEnode8(i)), i);

    // access int16_t
    EXPECT_EQ(ZigZagDecode16(ZigZagEnode16(i)), i);

    // access int32_t
    EXPECT_EQ(ZigZagDecode32(ZigZagEnode32(i)), i);

    // access int64_t
    EXPECT_EQ(ZigZagDecode64(ZigZagEnode64(i)), i);
  }

  // access int8_t min/max
  EXPECT_EQ(ZigZagDecode8(ZigZagEnode8(INT8_MIN)), INT8_MIN);
  EXPECT_EQ(ZigZagDecode8(ZigZagEnode8(INT8_MAX)), INT8_MAX);

  // access int16_t min/max
  EXPECT_EQ(ZigZagDecode16(ZigZagEnode16(INT16_MIN)), INT16_MIN);
  EXPECT_EQ(ZigZagDecode16(ZigZagEnode16(INT16_MAX)), INT16_MAX);

  // access int32_t min/max
  EXPECT_EQ(ZigZagDecode32(ZigZagEnode32(INT32_MIN)), INT32_MIN);
  EXPECT_EQ(ZigZagDecode32(ZigZagEnode32(INT32_MAX)), INT32_MAX);

  // access int64_t min/max
  EXPECT_EQ(ZigZagDecode64(ZigZagEnode64(INT64_MIN)), INT64_MIN);
  EXPECT_EQ(ZigZagDecode64(ZigZagEnode64(INT64_MAX)), INT64_MAX);
}

TEST(CodecTest, AccessVarint) {
  char buffer[5];
  char* end = &(buffer[0]) + 5;
  uint32_t expect;
  for (unsigned int i = 0; i < 100; ++i) {
    const void* p = SetVarint32(buffer, i);
    EXPECT_LE(p, end);
    p = GetVarint32(buffer, &expect);
    EXPECT_LE(p, end);
    EXPECT_EQ(i, expect);
  }

  // access max
  const void* p = SetVarint32(buffer, UINT32_MAX);
  EXPECT_LE(p, end);
  p = GetVarint32(buffer, &expect);
  EXPECT_LE(p, end);
  EXPECT_EQ(UINT32_MAX, expect);
}

}  // anonymous namespace
