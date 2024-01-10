// Copyright 2023 The Peppa Authors.

#ifndef PEPPA_ERROR_H_
#define PEPPA_ERROR_H_

#include <cstddef>
#include <cassert>

#include "peppa/config.h"

#define PP_MKERRNUM_IMPL(a, b, c, d)  \
  (((unsigned)(a) << 24) | ((b) << 16) | ((c) << 8) | (d))

# define PP_MKERRNUM(a, b, c, d) PP_MKERRNUM_IMPL(a, b, c, d)

#define PP_SYNTAX_ERROR Pe_MKERRNUM(0, 'S', 'Y', 'E')

namespace peppa {
PP_NAMESPACE_BEGIN

// Put a description of the errnum in "buf".
const char* StrError(int errnum, char* buf, size_t size);

PP_NAMESPACE_END
}  // namespace peppa

#define PP_ASSERT(cond)     \
  (PP_EXPECT_FALSE(cond) ?  \
    static_cast<void>(0) : [] { assert(false && PP_STRINGFY(cond)); }())

#endif  // PEPPA_ERROR_H_
