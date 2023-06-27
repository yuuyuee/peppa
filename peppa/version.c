/* Copyright 2023 The Peppa Authors. */

#include "peppa/version.h"

int PeVersion_getVersion() {
  return Pe_VERSION;
}

const char* PeVersion_getVersionString() {
  return Pe_VERSION_S;
}

const char* PeVersion_getCopyRight() {
  static const char* kCopyRight = "Copyright 2023 The Peppa Authors";
  return kCopyRight;
}

