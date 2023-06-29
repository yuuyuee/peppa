/* Copyright 2023 The Peppa Authors. */

#include "peppa/version.h"

int Pe_getVersion() {
  return PeVer_VERSION;
}

const char* Pe_getVersionString() {
  return PeVer_VERSION_S;
}

const char* Pe_getCopyRight() {
  return "Copyright 2023 The Peppa Authors";
}
