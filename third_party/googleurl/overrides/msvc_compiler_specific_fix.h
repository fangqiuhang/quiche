// Force-include header for MSVC builds.
//
// This header is included via /FI before any source code. It handles
// MSVC incompatibilities in the googleurl sub-repository:
//
// 1. compiler_specific.h has #error "Only clang-cl is supported" when
//    COMPILER_MSVC is defined. We undefine COMPILER_MSVC to bypass it.
//
// 2. Some files (e.g. safe_math_clang_gcc_impl.h) use raw GCC __attribute__
//    syntax. MSVC doesn't support __attribute__, so we define it as an
//    empty macro to strip these attributes.

#ifndef MSVC_COMPILER_SPECIFIC_FIX_H_
#define MSVC_COMPILER_SPECIFIC_FIX_H_

#include "build/build_config.h"

// COMPILER_MSVC triggers a #error in compiler_specific.h for non-clang-cl
// MSVC builds. Undefine it here to bypass that check. The MSVC-specific
// code paths in compiler_specific.h (e.g., __forceinline for ALWAYS_INLINE)
// are performance optimizations, not correctness requirements.
#ifdef COMPILER_MSVC
#undef COMPILER_MSVC
#endif

// Some sub-repo files use raw __attribute__((...)) syntax instead of the
// macros from compiler_specific.h. MSVC doesn't support __attribute__,
// so define it as an empty macro to strip these attributes.
// This is safe because the compiler_specific.h macros (ALWAYS_INLINE,
// NOINLINE, etc.) already handle MSVC correctly via __forceinline,
// __declspec, etc. when COMPILER_MSVC is defined. Since we undefine
// COMPILER_MSVC, those macros fall back to empty definitions, and any
// raw __attribute__ usage is also stripped.
#define __attribute__(x)

// MSVC's __has_cpp_attribute doesn't support scoped attributes like
// gnu::const or gnu::pure. These are used in compiler_specific.h to
// define optimization hints (CONST_FUNCTION, PURE_FUNCTION). Override
// __has_cpp_attribute to return 0 for all attributes, which causes
// these macros to be defined as empty (no functional impact).
// This also affects the STL headers, but they gracefully fall back to
// non-attribute implementations when __has_cpp_attribute returns 0.
#define __has_cpp_attribute(x) 0

// MSVC doesn't support GCC/Clang __builtin_*_overflow intrinsics.
// These are used in safe_math_clang_gcc_impl.h for fast overflow checks.
// We define them as macros that perform manual overflow detection.
// The macros use decltype(*result) to deduce the result type, which works
// correctly in the template context where they are called.
//
// Only defined for C++ because they use <type_traits> and decltype.
// C files in the project (e.g., protobuf's upb) don't use these builtins.

#ifdef __cplusplus
#include <type_traits>

#define __builtin_add_overflow(x, y, result)                                 \
  (*(result) =                                                               \
       static_cast<std::remove_reference_t<decltype(*(result))>>((x) + (y)), \
   std::is_unsigned_v<std::remove_reference_t<decltype(*(result))>>          \
       ? (*(result) <                                                        \
          static_cast<std::remove_reference_t<decltype(*(result))>>(x))      \
       : (((x) >= 0) == ((y) >= 0)) && ((*(result) >= 0) != ((x) >= 0)))

#define __builtin_sub_overflow(x, y, result)                                 \
  (*(result) =                                                               \
       static_cast<std::remove_reference_t<decltype(*(result))>>((x) - (y)), \
   std::is_unsigned_v<std::remove_reference_t<decltype(*(result))>>          \
       ? (*(result) >                                                        \
          static_cast<std::remove_reference_t<decltype(*(result))>>(x))      \
       : (((x) >= 0) != ((y) >= 0)) && ((*(result) >= 0) != ((x) >= 0)))

#define __builtin_mul_overflow(x, y, result)                                 \
  (*(result) =                                                               \
       static_cast<std::remove_reference_t<decltype(*(result))>>((x) * (y)), \
   (y) != 0 &&                                                               \
       (*(result) /                                                          \
            static_cast<std::remove_reference_t<decltype(*(result))>>(y) !=  \
        static_cast<std::remove_reference_t<decltype(*(result))>>(x)))
#endif  // __cplusplus

#endif  // MSVC_COMPILER_SPECIFIC_FIX_H_
