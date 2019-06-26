#pragma once

#include "osuRandom.h"																		

#define FindNext(memstart, memend, needle)									\
		if(memstart && memend){												\
			const DWORD Needed = strlen(needle);							\
			DWORD cCount = 0;												\
			while (memstart < memend) {										\
				if (*memstart == needle[cCount]){							\
					cCount++;												\
					if (cCount == Needed)									\
						break;												\
				}else{memstart -= cCount; cCount = 0;}						\
																			\
			memstart++;														\
			}																\
			if (memstart >= memend)memstart = 0;							\
			else memstart++;												\
		}


#define _READDOUBLE(s) [](const char* sP){					\
				if (!sP)return 0.;							\
				const bool Negative = (*sP == '-');			\
				if (Negative)sP++;							\
															\
				double r = 0.;								\
															\
				while (*sP >= '0' && *sP <= '9') {			\
					r = (r * 10.0) + (*sP - '0');			\
					sP++;									\
				}											\
				if (*sP == '.') {							\
					sP++;									\
					int c = 0;								\
					double e = 0.;							\
					while (*sP >= '0' && *sP <= '9') {		\
						e = (e * 10.) + (*sP - '0');		\
						sP++;								\
						c++;								\
					}										\
					r += e / std::pow(10., c);				\
				}											\
				return (Negative) ? -r : r;					\
			}(s)


#define _READDOUBLE_FAST(s) [](const char* sP){											\
	if (!sP)return 0.;																	\
	const bool Negative = (*sP == '-');													\
	if (Negative)sP++;																	\
																						\
	int p1 = 0;																			\
	while (*sP >= '0' && *sP <= '9'){													\
		p1 = (p1 * 10) + (*sP - '0');													\
		sP++;																			\
	}																					\
																						\
	if (*sP != '.')																		\
		return (Negative) ? -double(p1) : double(p1);									\
																						\
	sP++;																				\
	int p2 = 0;																			\
	byte c = 0;																			\
	while (c != 5 && *sP >= '0' && *sP <= '9'){											\
		p2 = (p2 * 10) + (*sP - '0');													\
		sP++;																			\
		c++;																			\
	}																					\
																						\
	if(c != 5){																			\
		switch(c){																		\
		case 1:p2 *= 10000;break;														\
		case 2:p2 *= 1000;break;														\
		case 3:p2 *= 100;break;															\
		case 4:p2 *= 10; break;															\
		case 0:																			\
		default:break;																	\
		}																				\
																						\
	}																					\
																						\
	const double r = (p1 == 0 && p2 == 0) ? 0. : double(p1) + (double(p2) * 0.00001);	\
																						\
	return (Negative) ? -r : r;}(s)

#define CLAMP(v,minv,maxv) al_min(al_max(v,minv),maxv)

#define PPFAIL_NOBEATMAP 1
#define PPFAIL_HEADERFAIL 2
#define PPFAIL_TIMINGFAIL 3
#define PPFAIL_NONOTES 4


struct _TimingBPM {
	int Offset;
	double Multiplier;
	double BeatLength;
	_TimingBPM(int Offset, double Multiplier, double BeatLength) :Offset(Offset), Multiplier(Multiplier), BeatLength(BeatLength){};
};

struct vec2_i {
	int x, y;
};

/*
#define _READ_BFLOAT(s)\
	[](const char* sP)->byte{\
		int p1 = 0;\
		int p2 = 0;\
		while (*sP >= '0' && *sP <= '9'){\
			p1 = (p1 * 10) + (*sP - '0');\
			sP++;\
		}\
		if (*sP == '.') {\
			sP++;\
			if(*sP > '0' && *sP <= '9')\
				p2 = (*sP - '0');\
		}\
		p1 = CLAMP(p1, 0, 10);\
		p2 = CLAMP(p2, 0, 9);\
		return byte(p1) + byte(p2 << 3);\
	}(s)
#define UNPACK_BFLOAT(s)[&](){return float(s & 15) + float((s & (15 << 3)) >> 3) * 0.1f;}()
*/
struct _MapHeaders{
	float CircleSize, OverallDifficulty, ApproachRate;
	byte MapVersion, MapMode;
	double SliderMultiplier, SliderTickRate;
};

struct _RawBeatmapObject {
	short x, y;
	int startTime,endTime;
	byte Type, Sound, MultiType, repeatCount;//WARNING: Might want to change repeat count to a larger size if need be.
	double Length;
	std::vector<vec2_i> MultiData;
};

struct _RawBeatmap {
	_MapHeaders Headers;
	std::vector<_TimingBPM> TimingPoints;
	std::vector<_RawBeatmapObject> Notes;
};

enum HitObjectType
{
	Hit_Normal = 1,
	Hit_Slider = 2,
	Hit_NewCombo = 4,
	Hit_Spinner = 8,
	Hit_Hold = 128
};


#include "pp_osu.h"
#include "pp_taiko.h"
#include "pp_ctb.h"
#include "pp_mania.h"


#include <map>

std::map<const DWORD, const _RawBeatmap> RawMap_Cache;

std::shared_mutex RawMap_Cache_Lock;


int pp_ReadRawMapData(const std::string &Input, _RawBeatmap &Output){


	

	const std::vector<byte> &RawMap = LOAD_FILE(Input);

	if (RawMap.size() <= 17 || memcmp(&RawMap[0], "osu file format v", 17))
		return PPFAIL_NOBEATMAP;

	const char* Start = (char*)&RawMap[0];
	const char* End = (char*)&RawMap[RawMap.size()];

	const char* CO = Start + 17;
	
	_MapHeaders& Headers = Output.Headers;

	byte& MapVersion = Headers.MapVersion;
	byte& MapMode = Headers.MapMode;

	float& CircleSize = Headers.CircleSize;
	float& OverallDifficulty = Headers.OverallDifficulty;
	float& ApproachRate = Headers.ApproachRate;
	double& SliderMultiplier = Headers.SliderMultiplier;
	double& SliderTickRate = Headers.SliderTickRate;

	while (CO != End && *CO >= '0' && *CO <= '9') {
		MapVersion *= 10;
		MapVersion += *CO - '0';
		CO++;
	}

	const char* RevertCO = CO;

	FindNext(CO, End, "\nMode: ");

	if (!CO)CO = RevertCO;//I did not check but I assume old maps (07) would not have a mode.
	else MapMode = *CO - '0';

	const char* DiffStart = CO;
	FindNext(DiffStart, End, "\n[Difficulty]");
	const char* TimingStart = DiffStart;
	FindNext(TimingStart, End, "\n[TimingPoints]");
	const char* HitObjectStart = TimingStart;
	FindNext(HitObjectStart, End, "\n[HitObjects]");

	if (!DiffStart || !TimingStart || !HitObjectStart)
		return PPFAIL_HEADERFAIL;

	{
		const char* DiffRead = DiffStart;

		//#define DIFFREAD(name) FindNext(DiffRead, TimingStart, "\n"#name":"); name = _READ_BFLOAT(DiffRead); DiffRead = DiffStart
		#define DIFFREAD(name) FindNext(DiffRead, TimingStart, "\n"#name":"); name = _READDOUBLE(DiffRead); DiffRead = DiffStart	

		DIFFREAD(CircleSize);
		DIFFREAD(OverallDifficulty);
		DIFFREAD(ApproachRate);
		DIFFREAD(SliderMultiplier);
		DIFFREAD(SliderTickRate);
		
		if (ApproachRate == 0.f)//I know this could cause issues if the ar is just truly set to 0. But who cares.
			ApproachRate = OverallDifficulty;
	}

	TimingStart += 2;//This assumes the file has the windows return carriage. It slightly malforming the first timing point does not matter as it will be set to the lowest possible value anyway.

	std::vector<_TimingBPM> &TimingPoints = Output.TimingPoints;
	TimingPoints.reserve(128);
	
	double LastRealBPM = 10.;
	double LastMultiplier = 0.;

	while (1) {
		const char* TimingEnd = TimingStart;

		FindNext(TimingEnd, HitObjectStart, "\n");

		if (!TimingEnd || TimingStart == TimingEnd)break;

		bool SecondData = 0;

		const int tOffset = _READINT32(TimingStart);
		double tBPM = 0.f;

		for (const char* i = TimingStart; i < TimingEnd; i++) {
			if (*i == ',') {
				i++;
				tBPM = _READDOUBLE(i);
				SecondData = 1;
				break;
			}
		}

		if (SecondData) {
			if (tBPM != LastMultiplier) {//Ignore timing points not related to bpm.
				LastMultiplier = tBPM;
				TimingPoints.push_back({
					tOffset,tBPM,
					[&]()->double {
						if (tBPM > 0) {
							LastRealBPM = CLAMP(tBPM,10.,1000.);
							return tBPM;
						}
						return CLAMP(double(LastRealBPM * (-tBPM * 0.01)), 10., 1000.);
					}()
				});
			}
		}
		else break;

		TimingStart = TimingEnd;

	}

	if (!TimingPoints.size())
		return PPFAIL_TIMINGFAIL;

	TimingPoints[0].Offset = INT_MIN;

	HitObjectStart += 2;

	std::vector<_RawBeatmapObject> &RawBeatData = Output.Notes;
	RawBeatData.reserve(1024);

	while (1){
		const char* HitObjectEnd = HitObjectStart;

		FindNext(HitObjectEnd, End, "\n");

		if (!HitObjectEnd || HitObjectStart == HitObjectEnd)break;


		_RawBeatmapObject raw;

		raw.x = _READINT32(HitObjectStart);

		byte Count = 0;

		for (const char* i = HitObjectStart; i < HitObjectEnd; i++) {
			if (*i == ',') {
				i++;

				if (Count == 0)
					raw.y = _READINT32(i);
				else if (Count == 1) {
					raw.startTime = _READINT32(i);
					raw.endTime = raw.startTime;
				}else if (Count == 2)
					raw.Type = byte(_READINT32(i));
				else if (Count == 3) {
					raw.Sound = byte(_READINT32(i));
				}
				else if (Count == 4) {
					if (MapMode != 3 && raw.Type & Hit_Slider) {
						raw.MultiData.reserve(16);
						raw.MultiType = *i;
						i++;
						for (; i < HitObjectEnd; i++) {

							if (*i == ',') { i--; break; }

							if (*i == '|') {
								i++;

								const int x = _READINT32(i);
								int y = INT_MIN;

								const char* sr = i;

								while (HitObjectEnd - 1 > sr) {
									if (*sr == ':') {
										sr++;
										y = _READINT32(sr);
										break;
									}sr++;
								}

								if (y != INT_MIN)
									raw.MultiData.push_back({ x, y });
							}
						}
						Count++;
						break;
					}
					else if (MapMode == 3 || raw.Type & Hit_Spinner)
						raw.endTime = _READINT32(i);

				}
				else if (Count == 5)
					raw.repeatCount = byte(CLAMP(_READINT32(i), 0, 255));
				else if (Count == 6 && raw.Type & Hit_Slider)
					raw.Length = _READDOUBLE(i);
				else break;

				Count++;
			}
		}

		if (Count > 3)RawBeatData.push_back(raw);

		HitObjectStart = HitObjectEnd;
	}
	if (!RawBeatData.size())
		return PPFAIL_NONOTES;
	//printf("Time: %f\nnNoteCount: %llu\n", double(double(TTime) * .000001), RawBeatData.size());
	return 0;
}

int pp_GetRawMapData(const DWORD ID, _RawBeatmap &d){

	{
		S_MUTEX_SHARED_LOCKGUARD(RawMap_Cache_Lock);

		auto it = RawMap_Cache.find(ID);

		if (it != RawMap_Cache.end()){
			d = it->second;
			return 0;
		}

	}

	const int res = pp_ReadRawMapData(BEATMAP_PATH + std::to_string(ID) + ".osu", d);

	if (!res){

		RawMap_Cache_Lock.lock();

		RawMap_Cache.insert(std::pair<const DWORD, const _RawBeatmap>(ID, d));

		RawMap_Cache_Lock.unlock();

	}
	return res;
}