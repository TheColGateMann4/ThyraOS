#pragma once 

namespace memory
{
	static inline void memcpy(void* dest, int dest_size, const void* src, int src_size)
	{
		unsigned char* d = static_cast<unsigned char*>(dest);
		const unsigned char* s = static_cast<const unsigned char*>(src);

		for(int i = 0; i < src_size; i++)
			*d++ = *s++;

	}

	template<class T, class V>
	static inline T bit_cast(const V& v)
	{
		static_assert(sizeof(T) == sizeof(V), "types varied in size");

		T t;
		memcpy(&t, sizeof(T), &v, sizeof(V));
		return t;
	}
};
