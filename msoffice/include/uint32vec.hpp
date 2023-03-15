#pragma once
/**
	Copyright (C) 2012 Cybozu Labs, Inc., all rights reserved.
*/
#include <cybozu/inttype.hpp>
#include <stdio.h>
#include <assert.h>
#ifdef _WIN32
	#include <winsock2.h>
	#include <intrin.h>
#else
	#if defined(__arm__)
        // ABr: 32-bit is not supported :(
        #define UINT32VEC_UNSUPPORTED
    #else
        #if defined(__aarch64__)
            // ABr: unclear if we should target 128 / 256 vectors
            #define __AVX2__

            // ABr: see simd readme - target the *original architecture*
            #ifdef __AVX2__
                #define SIMDE_X86_AVX_ENABLE_NATIVE_ALIASES
                #define SIMDE_X86_AVX2_ENABLE_NATIVE_ALIASES
                #include "../../simde-master/simde/x86/avx2.h"
            #else
                #define SIMDE_X86_AVX_ENABLE_NATIVE_ALIASES
                #include "../../simde-master/simde/x86/avx.h"
            #endif

            // ABr: clang fails on a couple of simd macros.
            #ifdef __clang__
                #define ARM_HANDLE_BUGGY_CLANG
            #endif
        #else
			#ifdef __linux__
				#include <x86intrin.h>
			#endif
		#endif
	#endif
#endif

template<size_t size>
struct Uint32VecT {
};

#ifdef __AVX2__
#define UINT32VEC_USE_AVX2
template<>
struct Uint32VecT<8> {
	static const int size = 8;
	__m256i x_;
	Uint32VecT()
	{
	}
	Uint32VecT(__m256i x)
		: x_(x)
	{
	}
	Uint32VecT(uint32_t x)
		: x_(_mm256_set1_epi32(x))
	{
	}
	// m = [x7:x6:x5:x4:x3:x2:x1:x0]
	Uint32VecT(uint32_t x7, uint32_t x6, uint32_t x5, uint32_t x4, uint32_t x3, uint32_t x2, uint32_t x1, uint32_t x0)
		: x_(_mm256_set_epi32(x7, x6, x5, x4, x3, x2, x1, x0))
	{
	}
	// m = [x7:x6:x5:x4:x3:x2:x1:x0]
	void set(uint32_t x7, uint32_t x6, uint32_t x5, uint32_t x4, uint32_t x3, uint32_t x2, uint32_t x1, uint32_t x0)
	{
		x_ = _mm256_set_epi32(x7, x6, x5, x4, x3, x2, x1, x0);
	}
	void clear()
	{
		*this = _mm256_setzero_si256();
	}
	void put(const char *msg = 0) const
	{
		uint32_t v[8];
		memcpy(&v, &x_, sizeof(v));
		if (msg) printf("%s", msg);
		printf("%08x:%08x:%08x:%08x", v[3], v[2], v[1], v[0]);
		if (msg) putchar('\n');
	}
};

inline Uint32VecT<8> operator<<(const Uint32VecT<8>& a, const int n)
{
	return _mm256_slli_epi32(a.x_, n);
}
inline Uint32VecT<8> operator>>(const Uint32VecT<8>& a, const int n)
{
	return _mm256_srli_epi32(a.x_, n);
}

inline Uint32VecT<8> operator+(const Uint32VecT<8>& a, const Uint32VecT<8>& b)
{
	return _mm256_add_epi32(a.x_, b.x_);
}

// return (~a) & b
inline Uint32VecT<8> andn(const Uint32VecT<8>& a, const Uint32VecT<8>& b)
{
	return _mm256_andnot_si256(a.x_, b.x_);
}

inline Uint32VecT<8> operator|(const Uint32VecT<8>& a, const Uint32VecT<8>& b)
{
	return _mm256_or_si256(a.x_, b.x_);
}

inline Uint32VecT<8> operator&(const Uint32VecT<8>& a, const Uint32VecT<8>& b)
{
	return _mm256_and_si256(a.x_, b.x_);
}

inline Uint32VecT<8> operator^(const Uint32VecT<8>& a, const Uint32VecT<8>& b)
{
	return _mm256_xor_si256(a.x_, b.x_);
}

typedef Uint32VecT<8> Uint32Vec;

#else

template<>
struct Uint32VecT<4> {
	static const int size = 4;
	simde__m128i x_;
	Uint32VecT()
	{
	}
	Uint32VecT(simde__m128i x)
		: x_(x)
	{
	}
	Uint32VecT(uint32_t x)
		: x_(simde_mm_set1_epi32(x))
	{
	}
	// m = [x3:x2:x1:x0]
	Uint32VecT(uint32_t x3, uint32_t x2, uint32_t x1, uint32_t x0)
		: x_(simde_mm_set_epi32(x3, x2, x1, x0))
	{
	}
	// m = [x3:x2:x1:x0]
	void set(uint32_t x3, uint32_t x2, uint32_t x1, uint32_t x0)
	{
		x_ = simde_mm_set_epi32(x3, x2, x1, x0);
	}
	void clear()
	{
		*this = simde_mm_setzero_si128();
	}
	void put(const char *msg = 0) const
	{
		uint32_t v[4];
		memcpy(&v, &x_, sizeof(v));
		if (msg) printf("%s", msg);
		printf("%08x:%08x:%08x:%08x", v[3], v[2], v[1], v[0]);
		if (msg) putchar('\n');
	}
};

inline Uint32VecT<4> operator<<(const Uint32VecT<4>& a, const int n)
{
	return simde_mm_slli_epi32(a.x_, n);
}
inline Uint32VecT<4> operator>>(const Uint32VecT<4>& a, const int n)
{
	return simde_mm_srli_epi32(a.x_, n);
}

inline Uint32VecT<4> operator+(const Uint32VecT<4>& a, const Uint32VecT<4>& b)
{
	return simde_mm_add_epi32(a.x_, b.x_);
}

// return (~a) & b
inline Uint32VecT<4> andn(const Uint32VecT<4>& a, const Uint32VecT<4>& b)
{
	return simde_mm_andnot_si128(a.x_, b.x_);
}

inline Uint32VecT<4> operator|(const Uint32VecT<4>& a, const Uint32VecT<4>& b)
{
	return simde_mm_or_si128(a.x_, b.x_);
}

inline Uint32VecT<4> operator&(const Uint32VecT<4>& a, const Uint32VecT<4>& b)
{
	return simde_mm_and_si128(a.x_, b.x_);
}

inline Uint32VecT<4> operator^(const Uint32VecT<4>& a, const Uint32VecT<4>& b)
{
	return simde_mm_xor_si128(a.x_, b.x_);
}

typedef Uint32VecT<4> Uint32Vec;
#endif

