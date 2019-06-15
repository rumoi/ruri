#pragma once

enum AchGeneral {
	Combo500 = 1 << 0,
	Combo750 = 1 << 1,
	Combo1000 = 1 << 2,
	Combo2000 = 1 << 3,
	PassStar1 = 1 << 4,
	PassStar2 = 1 << 5,
	PassStar3 = 1 << 6,
	PassStar4 = 1 << 7,
	PassStar5 = 1 << 8,
	PassStar6 = 1 << 9,
	PassStar7 = 1 << 10,
	PassStar8 = 1 << 11,
	FCStar1 = 1 << 12,
	FCStar2 = 1 << 13,
	FCStar3 = 1 << 14,
	FCStar4 = 1 << 15,
	FCStar5 = 1 << 16,
	FCStar6 = 1 << 17,
	FCStar7 = 1 << 18,
	FCStar8 = 1 << 19/*,
	TotalHits1 = 1 << 20,
	TotalHits2 = 1 << 21,
	TotalHits3 = 1 << 22,
	TotalHits4 = 1 << 23,*/
};

struct _Achievement{
	DWORD osuGeneral;
	DWORD taikoGeneral;
	DWORD ctbGeneral;
	DWORD maniaGeneral;
	uint64_t Hushush;


	_Achievement() {
		osuGeneral = 0;
		taikoGeneral = 0;
		ctbGeneral = 0;
		maniaGeneral = 0;
		Hushush = 0;
	}

};


#define nAch(name, desc, icon) (std::string(icon + "+" + name + "+" + desc + "/"))


const std::string ComboSteps[] = {"500","750","1000","2000"};


const std::string Ach_GeneralOsu[] = {
	"osu-skill-pass-1+Rising Star+Can't go forward without the first steps./","osu-skill-pass-2+Constellation Prize+Definitely not a consolation prize. Now things start getting hard!/","osu-skill-pass-3+Building Confidence+Oh, you've SO got this./","osu-skill-pass-4+Insanity Approaches+You're not twitching, you're just ready./","osu-skill-pass-5+These Clarion Skies+Everything seems so clear now./","osu-skill-pass-6+Above and Beyond+A cut above the rest./","osu-skill-pass-7+Supremacy+All marvel before your prowess./","osu-skill-pass-8+Absolution+My god, you're full of stars!/",
	"osu-skill-fc-1+Totality+All the notes. Every single one./","osu-skill-fc-2+Business As Usual+Two to go, please./","osu-skill-fc-3+Building Steam+Hey, this isn't so bad./","osu-skill-fc-4+Moving Forward+Bet you feel good about that./","osu-skill-fc-5+Paradigm Shift+Surprisingly difficult./","osu-skill-fc-6+Anguish Quelled+Don't choke./","osu-skill-fc-7+Never Give Up+Excellence is its own reward./","osu-skill-fc-8+Aberration+They said it couldn't be done. They were wrong./"
};

const std::string Ach_GeneralTaiko[] = {
	"taiko-skill-pass-1+My First Don+Marching to the beat of your own drum. Literally./","taiko-skill-pass-2+Katsu Katsu Katsu+Hora! Izuko!/","taiko-skill-pass-3+Not Even Trying+Muzukashii? Not even./","taiko-skill-pass-4+Face Your Demons+The first trials are now behind you, but are you a match for the Oni?/","taiko-skill-pass-5+The Demon Within+No rest for the wicked./","taiko-skill-pass-6+Drumbreaker+Too strong./","taiko-skill-pass-7+The Godfather+You are the Don of Dons./","taiko-skill-pass-8+Rhythm Incarnate+Feel the beat. Become the beat./",
	"taiko-skill-fc-1+Keeping Time+Don, then katsu. Don, then katsu../","taiko-skill-fc-2+To Your Own Beat+Straight and steady./","taiko-skill-fc-3+Big Drums+Bigger scores to match./","taiko-skill-fc-4+Adversity Overcome+Difficult? Not for you./","taiko-skill-fc-5+Demonslayer+An Oni felled forevermore./","taiko-skill-fc-6+Rhythm's Call+Heralding true skill./","taiko-skill-fc-7+Time Everlasting+Not a single beat escapes you./","taiko-skill-fc-8+The Drummer's Throne+Percussive brilliance befitting royalty alone./"
};
const std::string Ach_GeneralCTB[] = {
	"ctb-skill-pass-1+A Slice Of Life+Hey, this fruit catching business isn't bad./","ctb-skill-pass-2+Dashing Ever Forward+Fast is how you do it./","ctb-skill-pass-3+Zesty Disposition+No scurvy for you, not with that much fruit./","ctb-skill-pass-4+Hyperdash ON!+Time and distance is no obstacle to you./","ctb-skill-pass-5+It's Raining Fruit+And you can catch them all./","ctb-skill-pass-6+Fruit Ninja+Legendary techniques./","ctb-skill-pass-7+Dreamcatcher+No fruit, only dreams now./","ctb-skill-pass-8+Lord of the Catch+Your kingdom kneels before you./",
	"ctb-skill-fc-1+Sweet And Sour+Apples and oranges, literally./","ctb-skill-fc-2+Reaching The Core+The seeds of future success./","ctb-skill-fc-3+Clean Platter+Clean only of failure. It is completely full, otherwise./","ctb-skill-fc-4+Between The Rain+No umbrella needed./","ctb-skill-fc-5+Addicted+That was an overdose?/","ctb-skill-fc-6+Quickening+A dash above normal limits./","ctb-skill-fc-7+Supersonic+Faster than is reasonably necessary./","ctb-skill-fc-8+Dashing Scarlet+Speed beyond mortal reckoning./"
};
const std::string Ach_GeneralMania[] = {
	"mania-skill-pass-1+First Steps+It isn't 9-to-5, but 1-to-9. Keys, that is./","mania-skill-pass-2+No Normal Player+Not anymore, at least./","mania-skill-pass-3+Impulse Drive+Not quite hyperspeed, but getting close./","mania-skill-pass-4+Hyperspeed+Woah./","mania-skill-pass-5+Ever Onwards+Another challenge is just around the corner./","mania-skill-pass-6+Another Surpassed+Is there no limit to your skills?/","mania-skill-pass-7+Extra Credit+See me after class./","mania-skill-pass-8+Maniac+There's just no stopping you./",
	"mania-skill-fc-1+Keystruck+The beginning of a new story/","mania-skill-fc-2+Keying In+Finding your groove./","mania-skill-fc-3+Hyperflow+You can *feel* the rhythm./","mania-skill-fc-4+Breakthrough+Many skills mastered, rolled into one./","mania-skill-fc-5+Everything Extra+Giving your all is giving everything you have./","mania-skill-fc-6+Level Breaker+Finesse beyond reason/","mania-skill-fc-7+Step Up+A precipice rarely seen./","mania-skill-fc-8+Behind The Veil+Supernatural!/"
};


const std::string* Ach_GeneralNames[] = { &Ach_GeneralOsu[0],&Ach_GeneralTaiko[0], &Ach_GeneralCTB[0],&Ach_GeneralMania[0]};

void Ach_General(const DWORD Diff, const DWORD GameMode, std::string *res){

	if (!Diff)return;

	for (DWORD i = 0; i < 20; i++)
		if (Diff & (1 << i)){
			if(i < 4)
				*res += nAch(ComboSteps[i] + " Combo", ComboSteps[i] + " big ones! You're moving up in the world!", "osu-combo-" + ComboSteps[i]);
			else *res += Ach_GeneralNames[GameMode][i - 4];
		}
}



void CalculateAchievement(_Achievement &New, const _Achievement Old, const DWORD GameMode, std::string *res) {
	

	{
		DWORD Diff = 0;

		if (GameMode == 0){//Switch case looked even more gross.
			Diff = (New.osuGeneral ^ Old.osuGeneral) & New.osuGeneral;
			New.osuGeneral = Old.osuGeneral | Diff;
		}else if (GameMode == 1){
			Diff = (New.taikoGeneral ^ Old.taikoGeneral) & New.taikoGeneral;
			New.taikoGeneral = Old.taikoGeneral | Diff;
		}else if (GameMode == 2) {
			Diff = (New.ctbGeneral ^ Old.ctbGeneral) & New.ctbGeneral;
			New.ctbGeneral = Old.ctbGeneral | Diff;
		}else if (GameMode == 3) {
			Diff = (New.maniaGeneral ^ Old.maniaGeneral) & New.maniaGeneral;
			New.maniaGeneral = Old.maniaGeneral | Diff;
		}

		Ach_General(Diff, GameMode, res);
	}

	if (res->size())
		res->pop_back();//Removes trailing '/'
}


