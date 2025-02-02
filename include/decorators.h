#ifndef __mattflow_decorators_h
#define __mattflow_decorators_h

#include "debug.h"

#if defined(RUNNER)
#  if !defined(DEBUG)
#    define DEBUG
#  endif  // !defined(DEBUG)
#endif    // defined(RUNNER)

// Memory Management
#define CALLER_DELETE
#define CALLEE_DELETE

// Data Direction
#define VALOUT
#define VALIN
#define VALINOUT

#if defined(DEBUG)
#  define mfassert(condition, ...)                                                     \
    if (!(condition)) mattflow::Debug::get().print_or_throw(__VA_ARGS__)
#else
#  define mfassert(...)
#  if !defined(NDEBUG)
#    define NDEBUG
#  endif  // !defined(NDEBUG)
#endif

// Non-copyable / non-movable macros.
#define MATTFLOW_NON_COPYABLE(TYPE)                                                    \
  TYPE(const TYPE& rhs)            = delete;                                           \
  TYPE& operator=(const TYPE& rhs) = delete

#define MATTFLOW_NON_MOVABLE(TYPE)                                                     \
  TYPE(TYPE&& rhs)            = delete;                                                \
  TYPE& operator=(TYPE&& rhs) = delete

// Variant Constexpr

#if defined(MATTFLOW_OS_MAC)
#  define MATTFLOW_VARIANT_CONSTEXPR const
#else
#  define MATTFLOW_VARIANT_CONSTEXPR constexpr
#endif

#endif  // __mattflow_decorators_h
