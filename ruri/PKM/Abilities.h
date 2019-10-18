#pragma once


namespace Abilities{

	enum AbilityFlags{
		Foe_onHit = 1 << 0,
		Self_onHit = 1 << 1,
		For_onEffect = 1 << 2,
		Self_onEffect = 1 << 3,
		onEnter = 1 << 4,
		EveryTurn = 1 << 5,
		OnContact = 1 << 6,
		WeatherAffected = 1 << 7

		//OutofBattle = 1 << 1
	};

	enum :u8 {
		Stench,
		Drizzle

	};



}


