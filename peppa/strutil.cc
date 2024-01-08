// Copyright 2023 The Peppa Authors.

#include "peppa/strutil.h"

#include <cassert>
#include <cstring>
#include <algorithm>

namespace peppa {
void Split(const char* s, size_t n, const char* sep,
           std::vector<std::string>* res,
           bool ignore_empty, int max_split) {
  assert(s != nullptr && sep != nullptr && res != nullptr);
  const char* end = s + n;
  if (end == s) {
    res->clear();
    return;
  }
  const char* sep_end = sep + strlen(sep);
  assert(sep_end > sep);
  std::vector<std::string> tmp;
  std::string buf;
  bool end_with_sep;
  for (; s != end; ++s) {
    auto pos = std::find(sep, sep_end, *s);
    if (pos == sep_end) {
      end_with_sep = false;
      buf.push_back(*s);
    } else if (!buf.empty() || !ignore_empty) {
      end_with_sep = true;
      tmp.push_back(buf);
      buf.clear();
      if (max_split > 0 && tmp.size() >= static_cast<size_t>(max_split)) {
        tmp.swap(*res);
        return;
      }
    }
  }
  if (!buf.empty() || (end_with_sep && !ignore_empty))
    tmp.push_back(buf);
  tmp.swap(*res);
}

}  // namespace peppa