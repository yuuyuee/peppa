// Copyright 2023 The Peppa Authors.

#ifndef PEPPA_FORMAT_H_
#define PEPPA_FORMAT_H_

#include <cstddef>
#include <cstdarg>

#include <string>

#include "peppa/config.h"

namespace peppa {
PP_NAMESPACE_BEGIN

// This is an simple wrapper for vsnprintf.
size_t Format(char* buf, size_t size, const char* fmt, ...);
size_t Format(char* buf, size_t size, const char* fmt, va_list ap);

PP_NAMESPACE_END
}  // namespace peppa

#endif  // PEPPA_FORMAT_H_
