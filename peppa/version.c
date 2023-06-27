/* Copyright 2023 The Peppa Authors. */

#include "peppa/version.h"

int PeVer_getVersion() {
  return PeVer_VERSION;
}

const char* PeVer_getVersionString() {
  return PeVer_VERSION_S;
}

const char* PeVer_getCopyRight() {
  return "Copyright 2023 The Peppa Authors";
}
