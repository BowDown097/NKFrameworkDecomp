#pragma once
#include <boost/current_function.hpp>

#define NKAssert(condition, message) \
    _NKAssert(condition, message, __FILE__, BOOST_CURRENT_FUNCTION, __LINE__)

bool _NKAssert(bool condition, const char* message, const char* file, const char* function, int line);
