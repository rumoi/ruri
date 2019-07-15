#pragma once





namespace pp_Standard{

	//Taken directly from oppai-ng
	
	#define SINGLE_SPACING 125.0f
	#define STAR_SCALING_FACTOR 0.0675f /* star rating multiplier */
	#define EXTREME_SCALING_FACTOR 0.5f /* used to mix aim/speed stars */
	#define STRAIN_STEP 400.0f /* diffcalc uses peak strains of 400ms chunks */
	#define DECAY_WEIGHT 0.9f /* peak strains are added in a weighed sum */
	#define MAX_SPEED_BONUS 45.0f /* ~330BPM 1/4 streams */
	#define MIN_SPEED_BONUS 75.0f /* ~200BPM 1/4 streams */
	#define ANGLE_BONUS_SCALE 90
	#define AIM_TIMING_THRESHOLD 107
	#define SPEED_ANGLE_BONUS_BEGIN (5 * M_PI / 6)
	#define AIM_ANGLE_BONUS_BEGIN (M_PI / 3)

	static float const decay_base[] = { 0.3f, 0.15f }; /* strains decay per interval */
	static float const weight_scaling[] = { 1400.0f, 26.25f }; /* balances aim/speed */

	struct _AuxNote {
		float n_x, n_y, Angle;
	};

	template<typename T>
	_inline int ceil(T x) {
		return (int)x + (x > (int)x);
	}

	std::pair<float, float> d_weigh_strains(VEC(float)&& Strains){

		float total = 0, difficulty = 0, weight = 1.0f;

		/* sort strains from highest to lowest */

		std::sort(begin(Strains), end(Strains), [](const float A, const float B) {return A > B; });

		for (auto& Strain : Strains){
			total += (float)pow(Strain, 1.2);
			difficulty += Strain * weight;
			weight *= DECAY_WEIGHT;
		}

		return {difficulty,total};
	}

	std::pair<float,float> calc_Strain(const _RawBeatmap* BM, const VEC(_AuxNote) &Aux ,const int type, const float Multiplier){

		/*
		* the first object doesn't generate a strain,
		* so we begin with an incremented interval end
		*/

		float Interval = (float)ceil(BM->Notes[0].startTime / (STRAIN_STEP * Multiplier)) * STRAIN_STEP * Multiplier;

		VEC(float) highest_strains;
		float max_strain = 0.f;

		const auto& Notes = BM->Notes;

		float prev_strain = 0.f;
		float prev_d_distance = 0.f;
		float prev_delta_time = 0.f;

		for (size_t i = 0; i < Notes.size(); i++){

			const auto o = Notes[i];
			float prev_time = 0.f, cur_strain = 0.f;

			if (i > 0){
				prev_time = float(Notes[i - 1].startTime);

				{
					float res = 0;
					const float time_elapsed = (o.startTime - Notes[i - 1].startTime) / Multiplier;
					const float decay = (float)pow(decay_base[type], time_elapsed / 1000.0f);

					const float delta_time = time_elapsed;

					/* this implementation doesn't account for sliders */
					if (o.Type & (OBJ_SLIDER | OBJ_CIRCLE)) {

						float diff[] = {Aux[i].n_x - Aux[i-1].n_x,Aux[i].n_y - Aux[i-1].n_y };

						int is_single;//unused?

						const float d_distance = v2f_len(diff);

						res = d_spacing_weight(d_distance, time_elapsed, prev_d_distance,
							prev_delta_time, Aux[i].Angle, type, &is_single) * weight_scaling[type];

						prev_d_distance = d_distance;

					}else prev_d_distance = 0.f;

					cur_strain = prev_strain * decay + res;

				}

			}

			{
				while (o.startTime > Interval) {

					highest_strains.push_back(max_strain);

					if (likely(i))
						max_strain = prev_strain *
							(float)pow(decay_base[type], (Interval - prev_time) / 1000.0f);
					else max_strain = 0;

					Interval += STRAIN_STEP * Multiplier;
				}
				max_strain = al_max(max_strain, cur_strain);
			}
			prev_strain = cur_strain;
		}

		/*
		* the peak strain will not be saved for
		* the last section in the above loop
		*/
		highest_strains.push_back(max_strain);

		return d_weigh_strains(_M(highest_strains));
	}

	void CalcDiff(const _RawBeatmap* BM){

		VEC(_AuxNote) AuxNote;
		AuxNote.resize(BM->Notes.size());

		float radius = (
			(PLAYFIELD_WIDTH / 16.0f) *
			(1.0f - 0.7f * ((float)BM->Headers.CircleSize - 5.0f) / 5.0f)
			);

		float scaling_factor = 52.0f / radius;

		if (radius < CIRCLESIZE_BUFF_TRESHOLD) {
			scaling_factor *=
				1.0f + al_min((CIRCLESIZE_BUFF_TRESHOLD - radius), 5.0f) / 50.0f;
		}

		for (size_t i = 0; i < BM->Notes.size(); i++) {

			const auto& Note = BM->Notes[i];

			AuxNote[i].n_x = float(Note.x) * scaling_factor;
			AuxNote[i].n_y = float(Note.y) * scaling_factor;

			if (likely(i >= 2)){

				const float v1[] = { AuxNote[i-2].n_x - AuxNote[i-1].n_x,AuxNote[i-2].n_y - AuxNote[i-1].n_y }
				, v2[] = { AuxNote[i].n_x - AuxNote[i-1].n_x,AuxNote[i].n_y - AuxNote[i-1].n_y };

				AuxNote[i].Angle = (float)fabs(atan2(v1[0] * v2[1] - v1[1] * v2[0], v1[0] * v2[0] + v1[1] * v2[1]));

			}
			else AuxNote[i].Angle = get_nan();

		}

		{
			auto [diff, total] = calc_Strain(BM, AuxNote, 0, 1.f);
			printf("RURI: %f | %f\n", diff, total);
		}
		{
			auto [diff, total] = calc_Strain(BM, AuxNote, 1, 1.f);
			printf("RURI: %f | %f\n", diff, total);
		}

	}


}









