#pragma once

namespace PKM{

	enum class Type : u8{
		Normal, Fight, Flying, Poison, Ground, Rock, Bug, Ghost, Steel,
		Fire, Water, Grass, Electric, Psychic, Ice, Dragon, Dark, Fairy,
		Count,
	};

	enum class Ability : u8{
		None
	};

	enum class Nature : u8 {
		Hardy,Lonely,Brave,Adamant,Naughty,Bold,Docile,Relaxed,Impish,Lax,Timix,Hasty,
		Serious,Jolly,Naive,Modest,Mild,Quiet,BashFul,Rash,Calm,Gentle,Sassy,Careful,Quirky,
		Count
	};

	enum class Stat : u8 {
		HP,Attack,Defence,SpAtk,SpDef,Speed
	};

	enum Effective : u8{Not,NotVery,Normal,Super};

	constexpr Effective effective_table[] = {
		/*Normal*/Normal,Normal,Normal,Normal,Normal,NotVery,Normal,Not,NotVery,Normal,Normal,Normal,Normal,Normal,Normal,Normal,Normal,Normal,
		/*Fight*/Super,Normal,NotVery,NotVery,Normal,Super,NotVery,Not,Super,Normal,Normal,Normal,Normal,NotVery,Super,Normal,Super,NotVery,
		/*Flying*/Normal,Super,Normal,Normal,Normal,NotVery,Super,Normal,NotVery,Normal,Normal,Super,NotVery,Normal,Normal,Normal,Normal,Normal,
		/*Poison*/Normal,Normal,Normal,NotVery,NotVery,NotVery,Normal,NotVery,Not,Normal,Normal,Super,Normal,Normal,Normal,Normal,Normal,Super,
		/*Ground*/Normal,Normal,Not,Super,Normal,Super,NotVery,Normal,Super,Super,Normal,NotVery,Super,Normal,Normal,Normal,Normal,Normal,
		/*Rock*/Normal,NotVery,Super,Normal,NotVery,Normal,Super,Normal,NotVery,Super,Normal,Normal,Normal,Normal,Super,Normal,Normal,Normal,
		/*Bug*/Normal,NotVery,NotVery,NotVery,Normal,Normal,Normal,NotVery,NotVery,NotVery,Normal,Super,Normal,Super,Normal,Normal,Super,NotVery,
		/*Ghost*/Not,Normal,Normal,Normal,Normal,Normal,Normal,Super,Normal,Normal,Normal,Normal,Normal,Super,Normal,Normal,NotVery,Normal,
		/*Steel*/Normal,Normal,Normal,Normal,Normal,Super,Normal,Normal,NotVery,NotVery,NotVery,Normal,NotVery,Normal,Super,Normal,Normal,Super,
		/*Fire*/Normal,Normal,Normal,Normal,Normal,NotVery,Super,Normal,Super,NotVery,NotVery,Super,Normal,Normal,Super,NotVery,Normal,Normal,
		/*Water*/Normal,Normal,Normal,Normal,Super,Super,Normal,Normal,Normal,Super,NotVery,NotVery,Normal,Normal,Normal,NotVery,Normal,Normal,
		/*Grass*/Normal,Normal,NotVery,NotVery,Super,Super,NotVery,Normal,NotVery,NotVery,Super,NotVery,Normal,Normal,Normal,NotVery,Normal,Normal,
		/*Electric*/Normal,Normal,Super,Normal,Not,Normal,Normal,Normal,Normal,Normal,Super,NotVery,NotVery,Normal,Normal,NotVery,Normal,Normal,
		/*Psychic*/Normal,Super,Normal,Super,Normal,Normal,Normal,Normal,NotVery,Normal,Normal,Normal,Normal,NotVery,Normal,Normal,Not,Normal,
		/*Ice*/Normal,Normal,Super,Normal,Super,Normal,Normal,Normal,NotVery,NotVery,NotVery,Super,Normal,Normal,NotVery,Super,Normal,Normal,
		/*Dragon*/Normal,Normal,Normal,Normal,Normal,Normal,Normal,Normal,NotVery,Normal,Normal,Normal,Normal,Normal,Normal,Super,Normal,Not,
		/*Dark*/Normal,NotVery,Normal,Normal,Normal,Normal,Normal,Super,Normal,Normal,Normal,Normal,Normal,Super,Normal,Normal,NotVery,NotVery,
		/*Fairy*/Normal,Super,Normal,NotVery,Normal,Normal,Normal,Normal,NotVery,NotVery,Normal,Normal,Normal,Normal,Normal,Super,Super,Normal
	};

	#include "PKM/Species_Data.h"

	struct _Stage{


	};

	#include "PKM/Abilities.h"

	struct PokeMon{

		u16 Species;
		u16 Item;
		u8 Ability;
		u8 Nature;

		u32 OT;

		union {
			struct{u32 HP:5,Attack:5,Defence:5,Speed:5,SpAtk:5,SpDef:5;};//2 wasted bits
			u32 Bytes;
		} IV;

		struct{
			u8 HP, Attack, Defence, Speed, SpAtk, SpDef;
			size_t getCount()const noexcept{
				return (size_t)HP + (size_t)Attack + (size_t)Defence +
					   (size_t)Speed + (size_t)SpAtk + (size_t)SpDef;
			}
		} EV;

	};



}


