#ifndef __mattflow_compat_h
#define __mattflow_compat_h

/**************\
 * OS Defines *
\**************/
 
#if defined(_WIN32)
#   define MATTFLOW_OS_WINDOWS
#endif // defined(_WIN32)

#if defined(__linux__)
#    define MATTFLOW_OS_LINUX
#endif

#if defined(__APPLE__)
#    define MATTFLOW_OS_MAC
#endif

/***************************\
 * Compiler & Arch Defines *
\***************************/

#if defined(_WIN64)
#   define MATTFLOW_ARCH_64
#elif defined(_WIN32)
#   define MATTFLOW_ARCH_32
#endif // defined(_WIN32)

#if defined(_MSC_VER)
#   define MATTFLOW_COMPILER_MSVC
#elif defined(__clang__) // defined(_MSC_VER)
#   define MATTFLOW_COMPILER_CLANG
#   if defined(__i386__)
#       if defined(__x86_64__)
#           define MATTFLOW_ARCH_64
#       else // defined(__x86_64__)
#           define MATTFLOW_ARCH_32
#       endif // defined(__x86_64__)
#   endif // defined(__i386__)
#elif defined(__GNUC__) || defined(__GNUG__) // defined(__clang__)
#   define MATTFLOW_COMPILER_GCC
#   if defined(__i386__)
#       if defined(__x86_64__)
#           define MATTFLOW_ARCH_64
#       else
#           define MATTFLOW_ARCH_32
#       endif // defined(__x86_64__)
#   endif // defined(__i386__)
#endif // defined(__GNUC__) || defined(__GNUG__)

#endif // __mattflow_compat_h
