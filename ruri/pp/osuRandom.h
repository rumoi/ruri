#pragma once
namespace oRand
{
	const double REAL_UNIT_INT = 1.0 / (INT_MAX + 1.0);
	const double REAL_UNIT_UINT = 1.0 / (UINT_MAX + 1.0);
	const DWORD Wi = 273326509, Yi = 842502087, Zi = 3579807591;
}

struct osuRandomInstance{

	DWORD w;
	DWORD bitBuffer;
	int bitBufferIdx;
	DWORD x, y, z;

	osuRandomInstance(const int seed){
		x = (DWORD)seed;
		y = oRand::Yi;
		z = oRand::Zi;
		w = oRand::Wi;
		bitBufferIdx = 32;
	}

	void Reinitialise(const int seed){
		x = (DWORD)seed;
		y = oRand::Yi;
		z = oRand::Zi;
		w = oRand::Wi;
		bitBufferIdx = 32;
	}

};

#define SWAP const DWORD t = (r.x ^ (r.x << 11)); r.x = r.y; r.y = r.z;r.z = r.w;	

#define oR_NextUInt(R)[](osuRandomInstance& r)->DWORD{			\
	SWAP														\
	return (r.w = (r.w ^ (r.w >> 19)) ^ (t ^ (t >> 8)));		\
	}(R)


#define oR_Next(R)[&](osuRandomInstance& r)->int{								\
	SWAP																		\
	return (int)(0x7FFFFFFF & (r.w = (r.w ^ (r.w >> 19)) ^ (t ^ (t >> 8))));	\
	}(R)


#define oR_NextupperBound(R, upperBound)[&](osuRandomInstance& r)->int{					\
	if(upperBound < 0)return -1;														\
	SWAP																				\
	return (int)((oRand::REAL_UNIT_INT*(int)(0x7FFFFFFF & (r.w = (r.w ^ (r.w >> 19))	\
			^ (t ^ (t >> 8)))))*int(upperBound));										\
	}(R)


#define oR_NextRange(R,lowerBound, upperBound)[&](osuRandomInstance& r)->int{																	\
	if(lowerBound > upperBound)return -1;																										\
	SWAP																																		\
	const int range = upperBound - lowerBound;																									\
	if (range < 0)																																\
		return lowerBound + (int)((oRand::REAL_UNIT_UINT*(r.w = (r.w ^ (r.w >> 19)) ^ (t ^ (t >> 8))))*(upperBound - (long)lowerBound));		\
																																				\
	return lowerBound + (int)((oRand::REAL_UNIT_INT*(int)(0x7FFFFFFF & (r.w = (r.w ^ (r.w >> 19)) ^ (t ^ (t >> 8)))))*range);					\
	}(R)


#define oR_NextDouble(R, upperBound)[&](osuRandomInstance& r)->double{									\
	if(upperBound < 0)return -1;																		\
	SWAP																								\
	return (oRand::REAL_UNIT_INT*(int)(0x7FFFFFFF & (r.w = (r.w ^ (r.w >> 19)) ^ (t ^ (t >> 8)))));		\
	}(R)


#define oR_NextBool(R)[&](osuRandomInstance& r)->bool{											\
	if (r.bitBufferIdx != 32){r.bitBufferIdx++;return ((r.bitBuffer >>= 1) & 0x1) == 1;}		\
	SWAP																						\
	r.bitBuffer = r.w = (r.w ^ (r.w >> 19)) ^ (t ^ (t >> 8));r.bitBufferIdx = 1;				\
	return (r.bitBuffer & 0x1) == 1;															\
	}(R)


#define oR_SkipRandom(r,c){}{}
#undef SWAP