#ifndef __mattflow_decorators_h
#define __mattflow_decorators_h

// Memory Management
#define CALLER_DELETE
#define CALLEE_DELETE

// Data Direction
#define OUT
#define IN
#define INOUT

#if defined(DEBUG)
#  define debug_printf(...) printf(__VA_ARGS__)
#else
#  define debug_printf(...)
#  if !defined(NDEBUG)
#    define NDEBUG
#  endif  // !defined(NDEBUG)
#endif

#endif // __mattflow_decorators_h
