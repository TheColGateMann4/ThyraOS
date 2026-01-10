#pragma once
#include "Memory.h"

namespace math
{
	constexpr float _pi = 3.1415926535f;

	static inline float abs(float v)
	{
		return  v < 0.0f ? -v : v;
	}

	static inline float sqrt(float v)
	{
		if(v <= 0.0f)
			return 0.0f;

		float x = v;

		// Netwton iterations
		x = 0.5f * (x + v / x);
		x = 0.5f * (x + v / x);
		x = 0.5f * (x + v / x);
		x = 0.5f * (x + v / x);

		return x;
	}

	static inline float pow(float v, float p)
	{
		if(v <= 0.0f)
			return 0.0f;

		// intiger exponent handling
		int ip = static_cast<int>(p);
		if(static_cast<float>(ip) == p)
		{
			float r = 1.0f;
			float b = v;
			int e = ip;

			if(e < 0)
			{
				e = -e;
				b = 1.0f / b;
			}

			while(e)
			{
				if(e & 1)
					r *= b;
				b *= b;
				e >>= 1;
			}

			return r;
		}

		int bits = memory::bit_cast<int>(v);

		int exp = ((bits >> 23) & 0xFF) - 127;

		// normalize mantissa to [1, 2)
		bits = (bits & 0x7FFFFF) | 0x3F800000;
		float mant = memory::bit_cast<float>(bits);

		// log2(v) approx= exp + (mant - 1)
		float log2v = static_cast<float>(exp) + (mant - 1.0f);

		float x = p * log2v;

		// split into intiger and fractional parts
		int xi = static_cast<int>(x);
		float xf =x - static_cast<float>(xi);

		// exp2(xf) approx= polynomial
		float exp2f = 1.0f +
			xf * (0.69314718f +
			xf * (0.24022651f +
			xf * 0.05550411f));

		// rebuild float from exponent
		int out = static_cast<int>(xi + 127) << 23;
		float scale = memory::bit_cast<float>(out);

		return scale * exp2f;
	}

	static inline float cos(float v)
	{
		int div = static_cast<int>(v / _pi);
		v = v - (div * _pi);

		char sign = 1;
		if (div % 2 != 0)
			sign = -1;

		float result = 1.0;
		float inter = 1.0;
		float num = v * v;

		for (int i = 1; i <= 6; i++)
		{
			float comp = 2.0 * i;
			float den = comp * (comp - 1.0);
			inter *= num / den;

			if (i % 2 == 0)
				result += inter;
			else
				result -= inter;
		}

		return sign * result;
	}

	static inline float acos(float v)
	{
		if(v <= -1.0f)
			return _pi;
		if(v >= 1.0f)
			return 0.0f;

		float x = abs(v);
		float res = -0.0187293f * x + 0.0742610f;
		res = res * x - 0.2121144f; 
		res = res * x + 1.5707288; 
		res *= sqrt(1.0f - x);

		return v >= 0.0f ? res : _pi - res;
	}

	static inline float floor(float v)
	{
		int i = static_cast<int>(v);

		if(static_cast<float>(i) > v)
			i--;

		return static_cast<float>(i);
	}

	static inline float ceil(float v)
	{
		int i = static_cast<int>(v);

		if(static_cast<float>(i) < v)
			i++;

		return static_cast<float>(i);
	}

	static inline float trunc(float v)
	{
		return static_cast<float>(static_cast<int>(v));
	}

	static inline float fmod(float v, float m)
	{
		if(m == 0.0f)
			return v;
		if(v == 0.0f)
			return 0.0f;

		float div = v / m; 
		float numWithoutReminder = trunc(div);

		return (div - numWithoutReminder) * m;
	}
};
