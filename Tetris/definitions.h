#pragma once

#include <cstdint>

#define ASSERTIONS_ENABLED true

#if ASSERTIONS_ENABLED
    inline void reportAssertionFailure(const char *msg1, const char* file, int line)
    {
        //fprintf(stdout, "Assertion Failure: %s, at %s, line %d", msg1, file, line);
    }

    #define ASSERT(expr) \
        if (expr) {} \
        else \
        { \
		reportAssertionFailure(#expr, __FILE__, __LINE__); \
			__debugbreak(); \
		}
    #else
    #define ASSERT(expr)
#endif

#define global_variable static
#define internal_static static

static_assert(sizeof(float) == 4, "Float is unexpected size");
#define f32 float
#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define u64 uint64_t
#define i8 int8_t
#define i16 int16_t
#define i32 int32_t
#define i64 int64_t
#define u32f uint_fast32
#define i32f int_fast32


#define Kilobytes(val) ((val) * 1024LL)
#define Megabytes(val) (Kilobytes(val) * 1024LL)
#define Gigabytes(val) (Megabytes(val) * 1024LL)