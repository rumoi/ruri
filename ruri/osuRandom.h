#pragma once
namespace oRand
{

	const double REAL_UNIT_INT = 1.0 / (INT_MAX + 1.0);
	const double REAL_UNIT_UINT = 1.0 / (UINT_MAX + 1.0);
	const DWORD Wi = 273326509;
	const DWORD Yi = 842502087, Zi = 3579807591;

}

struct osuRandomInstance {
	DWORD w;
	DWORD bitBuffer;
	int bitBufferIdx;
	DWORD x, y, z;

	osuRandomInstance(const int seed) {
		x = (DWORD)seed;
		y = oRand::Yi;
		z = oRand::Zi;
		w = oRand::Wi;
		bitBufferIdx = 32;
	}

	void Reinitialise(const int seed) {
		x = (DWORD)seed;
		y = oRand::Yi;
		z = oRand::Zi;
		w = oRand::Wi;
		bitBufferIdx = 32;
	}

};

#define oR_NextUInt(r)[&]()->DWORD{								\
	const DWORD t = (r.x ^ (r.x << 11));						\
	r.x = r.y; r.y = r.z;r.z = r.w;								\
	return (r.w = (r.w ^ (r.w >> 19)) ^ (t ^ (t >> 8)));		\
	}()


#define oR_Next(r)[&]()->int{													\
	const DWORD t = (r.x ^ (r.x << 11));										\
	r.x = r.y; r.y = r.z;r.z = r.w;												\
	return (int)(0x7FFFFFFF & (r.w = (r.w ^ (r.w >> 19)) ^ (t ^ (t >> 8))));	\
	}()


#define oR_NextupperBound(r, upperBound)[&]()->int{										\
	if(upperBound < 0)return -1;														\
	const DWORD t = (r.x ^ (r.x << 11));												\
	r.x = r.y; r.y = r.z;r.z = r.w;														\
	return (int)((oRand::REAL_UNIT_INT*(int)(0x7FFFFFFF & (r.w = (r.w ^ (r.w >> 19))	\
			^ (t ^ (t >> 8)))))*int(upperBound));										\
	}()


#define oR_NextRange(r,lowerBound, upperBound)[&]()->int{																						\
	if(lowerBound > upperBound)return -1;																										\
	const DWORD t = (r.x ^ (r.x << 11));																										\
	r.x = r.y; r.y = r.z;r.z = r.w;																												\
	const int range = upperBound - lowerBound;																									\
	if (range < 0)																																\
		return lowerBound + (int)((oRand::REAL_UNIT_UINT*(r.w = (r.w ^ (r.w >> 19)) ^ (t ^ (t >> 8))))*(upperBound - (long)lowerBound));		\
																																				\
	return lowerBound + (int)((oRand::REAL_UNIT_INT*(int)(0x7FFFFFFF & (r.w = (r.w ^ (r.w >> 19)) ^ (t ^ (t >> 8)))))*range);					\
	}()


#define oR_NextDouble(r, upperBound)[&]()->double{														\
	if(upperBound < 0)return -1;																		\
	const DWORD t = (r.x ^ (r.x << 11));																\
	r.x = r.y; r.y = r.z;r.z = r.w;																		\
	return (oRand::REAL_UNIT_INT*(int)(0x7FFFFFFF & (r.w = (r.w ^ (r.w >> 19)) ^ (t ^ (t >> 8)))));		\
	}()


#define oR_NextBool(r)[&]()->bool{																\
	if (r.bitBufferIdx != 32){r.bitBufferIdx++;return ((r.bitBuffer >>= 1) & 0x1) == 1;}		\
	const DWORD t = (r.x ^ (r.x << 11));														\
	r.x = r.y; r.y = r.z;r.z = r.w;																\
	r.bitBuffer = r.w = (r.w ^ (r.w >> 19)) ^ (t ^ (t >> 8));r.bitBufferIdx = 1;				\
	return (r.bitBuffer & 0x1) == 1;															\
	}()


#define oR_SkipRandom(r,c)for(int ufdihsh = 0; ufdihsh < c; ufdihsh++)oR_NextBool(r);
