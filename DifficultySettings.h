#ifndef DIFFICULTY_SETTINGS_HEADER
#define DIFFICULTY_SETTINGS_HEADER

namespace DifficultySettings
{
	// modifiers are added to the base value
	// example: Player::maxHealthModifier increases (if positive) or decreases (if negative) the player's max health
	namespace Player
	{
		int maxHealthModifier = 0;
		int eatHealModifier = 0;
		int healthDrainModifier = 0;
		int attackRangeModifier = 0;
		int attackHealthCostModifier = 0;
	}

	namespace Mage
	{
		int attackDamageModifier = 0;
		int attackRateModifier = 0;
		int touchDamageModifier = 0;
		int movementSpeedModifier = 0;
	}

	namespace Citizen
	{
		int movementSpeedModifier = 0;
	}
}

#endif
