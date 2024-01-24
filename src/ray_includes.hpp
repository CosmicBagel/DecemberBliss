//------------
// The point of wrapping the includes here is to silence external errors
//------------
#pragma once
// only including three things here, but there are so many warnings to ignore
// it gets its own header file now

// don't give warnings for external stuff that can't be controlled
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#pragma clang diagnostic ignored "-Wmissing-field-initializers"
#endif
#if defined(__GNUC__) || defined(__MINGW32__)  // gcc
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
#if defined(_MSC_VER)  // msvc
//(defined last, because sometimes when using clang with msvc, this will also be
//defined even tho we're getting clang errors/warnings)
#pragma warning(push)
#pragma warning(disable : 4244)
#endif

#include <raylib.h>
#include <rlgl.h>

#ifdef _MSC_VER
#pragma warning(default : 4244)
#pragma warning(pop)
#endif
#if defined(__GNUC__) || defined(__MINGW32__)  // gcc
#pragma GCC diagnostic pop
#endif
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
