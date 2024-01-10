// Copyright 2023 The Peppa Authors.

#ifndef PEPPA_STRUTIL_H_
#define PEPPA_STRUTIL_H_

#include <vector>
#include <string>

namespace peppa {
void Split(const char* s, size_t n, const char* sep,
           std::vector<std::string>* res,
           bool ignore_empty, int max_split);
}  // namespace peppa

#endif  // PEPPA_STRUTIL_H_