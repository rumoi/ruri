#pragma once

#ifndef _H__BEATMAP_
#define _H__BEATMAP_

enum class BeatmapErrorCode { Success, NoData, HeaderFail, TimingFail, NoteFail };

namespace BEATMAP {

	_inline int ReadInt(const u8* Start, const u8* End) {
		int Res = 0;

		const bool Neg = (Start == End) ? false : *Start == (u8)'-';

		while (Start++ != End)
			if (const u8 c = *(Start - 1); c >= (u8)'0' && c <= (u8)'9')
				Res = (Res * 10) + (c - (u8)'0');

		return !Neg ? Res : -Res;
	}

	const auto ReadDouble = [](const u8* Start, const u8* End)->double {

		while (unlikely(Start != End && *Start == ' '))Start++;

		const bool Negative = (*Start == (u8)'-' && ++Start);

		double Res = 0;
		Start--;
		while (++Start != End)
			if (const u8 c = *Start; c >= (u8)'0' && c <= (u8)'9')
				Res = (Res * 10.f) + (c - (u8)'0');
			else break;

		if (Start != End && *Start == (u8)'.') {

			size_t c = (size_t)Start + 1;
			double e = 0.;

			while (++Start != End)
				if (const u8 c = *Start; c >= (u8)'0' && c <= (u8)'9')
					e = (e * 10.) + (c - '0');
				else break;

			Res += e / std::pow(10., size_t(Start - c));
		}

		return !Negative ? Res : -Res;
	};

	struct _RawBeatmapMultiData {
		double Length;
		std::vector<std::pair<short, short>> Data;

		_RawBeatmapMultiData(const u8* Start, const u8* End, double Length) : Length(Length) {

			if (Start == End)return;

			Data.resize(std::count_if(Start, End, [](const u8 c) {return c == '|'; }));

			size_t o = 0;
		Start:
			for (; Start != End; Start++) {

				if (*Start == '|') {

					const u8* SubStart = ++Start;

					for (; Start <= End; Start++) {
						switch (*Start) {
						case '|':case ',':
							Data[o++].second = (short)ReadInt(SubStart, Start);
							goto Start;
						case ':':
							Data[o].first = (short)ReadInt(SubStart, Start);
							SubStart = Start + 1;
						default:break;
						}

					}


				}

			}



		}

	};

	struct _RawBeatmapObject {
		u32 timeDelta : 19, repeatCount : 13;
		u32 x : 10, y : 9, Type : 5, Sound : 4, MultiType : 4;
		u32 startTime; u32 MultiOffset;

		_RawBeatmapObject(u32 x, u32 y, u32 startTime, u8 Type, u8 Sound) : x(x), y(y), startTime(startTime), Type(Type), Sound(Sound) {
			timeDelta = 0;
			repeatCount = 0;
			MultiType = 0;
			MultiOffset = 0;
		}

	};

	struct _TimingBPM {
		int Offset;
		//double Multiplier;
		double BeatLength;
		_TimingBPM(int Offset,/* double Multiplier,*/ double BeatLength) :Offset(Offset)/*, Multiplier(Multiplier)*/, BeatLength(BeatLength) {};
	};

	struct _RawBeatmap {

		u32 MapVersion : 4, MapMode : 2, BID : 26;
		float CircleSize, OverallDifficulty, ApproachRate;
		double SliderMultiplier, SliderTickRate;

		std::vector<_RawBeatmapObject> RawNotes;
		std::vector<_RawBeatmapMultiData> RawMulti;

		std::vector<_TimingBPM> TimingPoints;

	};

	enum class HitObject : u8
	{
		Normal = 1,
		Slider = 2,
		NewCombo = 4,
		Spinner = 8,
		Hold = 128,
		NonZeroLength = Slider | Spinner
	};
	enum class CurveTypes : u8
	{
		Catmull,
		Bezier,
		Linear,
		PerfectCurve
	};

	_inline u8 GetCurve(const u8 v) {
		switch (v) {
		case (u8)'C':
			return (u8)CurveTypes::Catmull;
		case (u8)'B':
			return (u8)CurveTypes::Bezier;
		case (u8)'L':
			return (u8)CurveTypes::Linear;
		case (u8)'P':
			return (u8)CurveTypes::PerfectCurve;
		default:
			return (u8)CurveTypes::Bezier;
		}
	}

	constexpr size_t _strlen_(const char* s)noexcept {
		size_t V = 0;
		while (s[V++]);
		return V--;
	}

	template<typename T>
	const u8* ReadLines(const u8* Start, const u8* End, T Lamb) {

		const u8* const unEnd = End;

		Start -= (size_t)Start % 16;
		End -= (size_t)End % 16;

		const auto Mask = _mm_set1_epi8('\n');

		const u8* LineStart = Start;

		while (Start != End) {

			const u8* LineEnd = Start;

			const auto Check = _mm_load_si128((const __m128i*)LineEnd);

			u32 res = _mm_movemask_epi8(_mm_cmpeq_epi8(Mask, Check));

			if (res) {
			CalculateLine:
				while (!(res & 1)) {
					LineEnd++;
					res >>= 1;
				}

				res >>= 1;//Might cause issues on a singular '\n'

				if (likely(*(--LineEnd) == (u8)'\r')) {

					if (Lamb(LineStart, LineEnd))
						return LineEnd + 2;

					LineStart = LineEnd += 2;
				}
				if (unlikely(res))
					goto CalculateLine;
			}

			Start += 16;
		}

		Start -= 16;

		for (; Start < unEnd; Start++) {
			if (*Start == '\n' && *(Start - 1) == '\r') {

				Start--;

				if (Lamb(LineStart, Start))
					return Start + 2;

				LineStart = Start += 2;
			}
		}

		return Start;
	}

	[[nodiscard]] BeatmapErrorCode ReadBeatmap(u32 ID, const VEC(u8)& MapFile, _RawBeatmap& Output) {

		Output.BID = ID;
		Output.ApproachRate = -1.f;

		const u8* Start = MapFile.data();
		const u8* End = MapFile.data() + MapFile.size();

		{
			constexpr char* VersionText = "osu file format v";

			if (constexpr size_t Size = _strlen_(VersionText) - 1;
				MapFile.size() <= Size + 2 || memcmp(MapFile.data(), VersionText, Size))
				return BeatmapErrorCode::NoData;
			else Start += Size;
		}

		Output.MapVersion = ReadInt(Start, Start + 2);

		Start = ReadLines(Start, End, [&Output](const u8* LineStart, const u8* LineEnd)->bool {

			size_t LineSize = LineEnd - LineStart;

			for (const u8* c = LineStart; c != LineEnd; c++)
				if (const u8 v = *c; v == (u8)':' || v == (u8)',') {//, is here to quick escape event data.
					LineSize = c - LineStart;
					break;
				}

			if (LineSize > 3) {

				switch (WSTI<size_t>(std::string_view((const char*)LineStart, LineSize))) {

					case WSTI<size_t>("StackLeniency") :
						break;

						case WSTI<size_t>("Mode") :
							Output.MapMode = (u8)ReadInt(LineStart + LineSize + 1, LineEnd);
							break;

							case WSTI<size_t>("CircleSize") :
								Output.CircleSize = (float)ReadDouble(LineStart + LineSize + 1, LineEnd);
								break;

								case WSTI<size_t>("OverallDifficulty") :
									Output.OverallDifficulty = (float)ReadDouble(LineStart + LineSize + 1, LineEnd);
									break;

									case WSTI<size_t>("ApproachRate") :
										Output.ApproachRate = (float)ReadDouble(LineStart + LineSize + 1, LineEnd);
										break;

										case WSTI<size_t>("SliderMultiplier") :
											Output.SliderMultiplier = (float)ReadDouble(LineStart + LineSize + 1, LineEnd);
											break;

											case WSTI<size_t>("SliderTickRate") :
												Output.SliderTickRate = (float)ReadDouble(LineStart + LineSize + 1, LineEnd);
												break;

												case WSTI<size_t>("[TimingPoints]") :
													return 1;

												default:
													break;
				}

			}
			return 0;
			});
		{

			Output.TimingPoints.reserve(128);

			double LastRealBPM = 10.;
			double LastMultiplier = 0.;

			Start = ReadLines(Start, End, [&Output, &LastRealBPM, &LastMultiplier](const u8* LineStart, const u8* LineEnd)->bool {

				if (LineStart == LineEnd)
					return 0;

				if (*LineStart == (u8)'[')
					return 1;

				const u8* const Start = LineStart;

				size_t C = 0;
				const u8* TimingEnds[2] = {};

				for (; LineStart != LineEnd; LineStart++) {

					if (*LineStart == (u8)',') {
						TimingEnds[C] = LineStart;
						if (C++)break;
					}
				}

				if (TimingEnds[0] && TimingEnds[1]) {

					const int Time = ReadDouble(Start, TimingEnds[0]);
					double CurrentMultiplier = ReadDouble(TimingEnds[0] + 1, TimingEnds[1]);

					if (CurrentMultiplier != LastMultiplier) {

						LastMultiplier = CurrentMultiplier;

						if (CurrentMultiplier > 0.)
							LastRealBPM = CurrentMultiplier = al_clamp(CurrentMultiplier, 10., 1000.);
						else CurrentMultiplier = al_clamp(double(LastRealBPM * (-CurrentMultiplier * 0.01)), 10.f, 1000.f);

						Output.TimingPoints.emplace_back(Time, CurrentMultiplier);
					}
				}
				return 0;
				});
		}

		Start -= CONSTX<size_t, _strlen_("[HitObjects]\r\n")>::value;

		int aaaa = 2;

		if (size_t HitObjects = std::string_view((const char*)Start, End - Start).find("[HitObjects]"); HitObjects != std::string::npos) {

			Output.RawNotes.reserve(Output.TimingPoints.size() << 6);
			Output.RawMulti.reserve(Output.TimingPoints.size() << 5);//Find proper value for this


			Start += CONSTX<size_t, _strlen_("HitObjects]\r\n")>::value + HitObjects;

			Start = ReadLines(Start, End, [&Output](const u8* LineStart, const u8* const LineEnd)->bool {

				const auto Split = [&LineStart, &LineEnd]() {

					while (LineStart != LineEnd) {
						if (*LineStart == (u8)',')
							return LineStart++;
						LineStart++;
					}
					return (const u8*)0;
				};

				const u8* const Start = LineStart;

				const u8* Comma[5] = { Split(),Split(),Split(),Split(),Split() };

				if (likely(Comma[0] && Comma[1] && Comma[2] && Comma[3])) {

					auto& Note = Output.RawNotes.emplace_back(
						(u32)ReadInt(Start, Comma[0]),//x
						(u32)ReadInt(Comma[0] + 1, Comma[1]),//y
						(u32)ReadInt(Comma[1] + 1, Comma[2]),//time
						(u8)ReadInt(Comma[2] + 1, Comma[3]),//type
						(u8)ReadInt(Comma[3] + 1, Comma[4] ? Comma[4] : LineEnd)//Sound
					);

					if ((Note.Type & (u32)HitObject::NonZeroLength) && Comma[4]) {

						if (Note.Type & (u32)HitObject::Slider) {

							const u8* MultiDataEnd = Split();
							const u8* RepeatEnd = Split();
							const u8* LengthEnd = Split();

							if (MultiDataEnd && RepeatEnd) {
								Note.repeatCount = (u32)ReadInt(MultiDataEnd + 1, RepeatEnd);
								Note.MultiOffset = Output.RawMulti.size();
								Note.MultiType = GetCurve(*++Comma[4]);//Might need to check this.
								Output.RawMulti.emplace_back(Comma[4], MultiDataEnd, ReadDouble(RepeatEnd + 1, LengthEnd ? LengthEnd : LineEnd));
							}
						}
						else if (Note.Type & (u32)HitObject::Spinner) {

							const u8* LengthEnd = Split();

							LengthEnd = LengthEnd ? LengthEnd : LineEnd;

							Note.timeDelta = ReadInt(Comma[4] + 1, LengthEnd) - Note.startTime;
						}
					}
				}

				return 0;
				});

		}

		Output.RawMulti.shrink_to_fit();

		if (Output.ApproachRate == -1.f)
			Output.ApproachRate = Output.OverallDifficulty;

		return BeatmapErrorCode::Success;
	}
}

#endif