#pragma once

#include "osuRandom.h"

#define LOAD_FILE(Filename)													\
	[&]{																	\
		std::ifstream file(Filename, std::ios::binary | std::ios::ate);		\
		if (!file.is_open())												\
			return std::vector<byte>();										\
																			\
		const size_t pos = file.tellg();									\
		std::vector<byte> rVec(pos + 1);									\
		file.seekg(0, std::ios::beg);										\
		file.read((char*)&rVec[0], pos);									\
		file.close();														\
		rVec[rVec.size()-1] = 0;											\
																			\
		return rVec;														\
	}()																		


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
	DWORD p1 = 0;																		\
	while (*sP >= '0' && *sP <= '9'){													\
		p1 = (p1 * 10) + (*sP - '0');													\
		sP++;																			\
	}																					\
																						\
	if (*sP != '.')																		\
		return (Negative) ? -double(p1) : double(p1);									\
																						\
	sP++;																				\
	DWORD p2 = 0;																		\
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



#define _READINT32(s) [](const char* sP){					\
				if(!sP)return 0;							\
				const bool Negative = (*sP == '-');			\
				if(Negative)sP++;							\
															\
				int r = 0;									\
															\
				while(*sP >= '0' && *sP <= '9'){			\
					r = (r * 10) + (*sP - '0');				\
					sP++;									\
				}											\
				return (Negative) ? -r : r;					\
			}(s)

#define CLAMP(v,minv,maxv) min(max(v,minv),maxv)

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

struct _MapHeaders{
	float HPDrainRate, CircleSize, OverallDifficulty, SliderMultiplier, SliderTickRate, ApproachRate;
	byte MapVersion;
	byte MapMode;
};

struct _RawBeatmapObject {
	int x, y, startTime,endTime;
	byte Type, Sound, MultiType, repeatCount;//WARNING: Might want to change repeat count to a larger size if need be.
	double Length;
	std::vector<vec2_i> MultiData;
};

enum HitObjectType
{
	Hit_Normal = 1,
	Hit_Slider = 2,
	Hit_NewCombo = 4,
	Hit_Spinner = 8,
	Hit_Hold = 128
};


int new_pp(const std::string &Input) {


	std::chrono::steady_clock::time_point sTime = std::chrono::steady_clock::now();

	const std::vector<byte> &RawMap = LOAD_FILE(Input);

	if (RawMap.size() <= 17 || memcmp(&RawMap[0], "osu file format v", 17))
		return PPFAIL_NOBEATMAP;

	const char* Start = (char*)&RawMap[0];
	const char* End = (char*)&RawMap[RawMap.size()];

	const char* CO = Start + 17;;

	byte MapVersion = 0;
	byte MapMode = 0;

	float HPDrainRate, CircleSize, OverallDifficulty, SliderMultiplier, SliderTickRate, ApproachRate;

	while (CO != End && *CO >= '0' && *CO <= '9') {
		MapVersion *= 10;
		MapVersion += *CO - '0';
		CO++;
	}

	const char* RevertCO = CO;

	FindNext(CO, End, "\nMode: ");
	if (!CO)CO = RevertCO;
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

		#define DIFFREAD(name)  FindNext(DiffRead, TimingStart, "\n"#name":"); name = _READDOUBLE_FAST(DiffRead); DiffRead = DiffStart;	

		DIFFREAD(HPDrainRate);
		DIFFREAD(CircleSize);
		DIFFREAD(OverallDifficulty);
		DIFFREAD(SliderMultiplier);
		DIFFREAD(SliderTickRate);
		DIFFREAD(ApproachRate);

		if (ApproachRate = 0.f)
			ApproachRate = OverallDifficulty;
	}

	TimingStart += 2;//This assumes the file has the windows return carriage. It slightly malforming the first timing point does not matter as it will be set to the lowest possible value anyway.

	std::vector<_TimingBPM> TimingPoints;
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


	std::vector<_RawBeatmapObject> RawBeatData;
	RawBeatData.reserve(1024);

	while (1) {

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
					if (raw.Type & Hit_Slider) {
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
					else if (raw.Type & Hit_Spinner)
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

		if (Count > 3)
			RawBeatData.push_back(raw);

		HitObjectStart = HitObjectEnd;
	}
	if (!RawBeatData.size())
		return PPFAIL_NONOTES;

	const unsigned long long TTime = std::chrono::duration_cast<std::chrono::nanoseconds> (std::chrono::steady_clock::now() - sTime).count();
	

	for (DWORD i = 0; i < RawBeatData.size(); i++) {
		
		if (RawBeatData[i].Type & Hit_Slider){

			for (DWORD z = 0; z < RawBeatData[i].MultiData.size(); z++) {

				printf("%i> x:%i | y:%i\n", i, RawBeatData[i].MultiData[z].x, RawBeatData[i].MultiData[z].y);

			}

		}

	}
	printf("entireTime: %f\n", double(double(TTime) * .000001));
	//printf("Time: %f\nnNoteCount: %llu\n", double(double(TTime) * .000001), RawBeatData.size());
	return 0;
}