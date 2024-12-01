#pragma once
#include <boost/current_function.hpp>

#define NKAssert(condition, message) \
    _NKAssert((condition), message, __FILE__, BOOST_CURRENT_FUNCTION, __LINE__)

// some error messages contain line numbers, so I want to use them where I see them.
// that way I can have a more accurate decompilation
#define ENFORCE_LINE(expected) static_assert(__LINE__ == (expected), "This must be at line " #expected)

bool _NKAssert(bool condition, const char* message, const char* file, const char* function, int line);
