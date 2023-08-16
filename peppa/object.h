/* Copyright 2023 The Peppa Authors. */

#ifndef PEPPA_OBJECT_H_
#define PEPPA_OBJECT_H_

#include <stdint.h>

enum {
  PeObject_NONE = 0,
  PeObject_BOOL,
  PeObject_INT,
  PeObject_NUM,
  PeObject_STR,
  PeObject_LIST,
  PeObject_DICT,
  PeObject_FUNC,
};

/* Type of none */

/* Type of boolean */
typedef int Pe_Boolean;
#define PeBool_TRUE 1
#define PeBool_FALSE 0

/* Type of integer */
typedef int64_t Pe_Integer;

/* Type of float */
typedef double Pe_Number;

/* Type of string */
typedef struct Pe_String {

} Pe_String;

/* Type of array */
typedef struct Pe_List {

} Pe_Array;

/* Type of dictionary */
typedef struct Pe_Dict {

} Pe_Dict;

/* _abs
 * _add
 * _
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 * */



typedef struct PeObject {
  int type;
  union {
    Pe_Integer integer;
    Pe_Number number;
    void* ptr;
  } v;
} PeObject;

#endif  /* PEPPA_OBJECT_H_ */
