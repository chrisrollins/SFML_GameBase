#ifndef DIFFICULTY_SETTINGS_HEADER
#define DIFFICULTY_SETTINGS_HEADER

namespace DifficultySettings
{
	// modifiers are added to the base value
	// example: Player::maxHealthModifier increases (if positive) or decreases (if negative) the player's max health
	namespace Player
	{
		static int maxHealthModifier;
		static int eatHealModifier;
		static float missingHealthHealBonus;
		static int healthDrainModifier;
		static float highHealthDrainPenalty;
		static int attackHealthCostModifier;
	}

	namespace Mage
	{
		static int attackDamageModifier;
		static int attackRateModifier;
		static int touchDamageModifier;
		static int movementSpeedModifier;
		static int healthDrainModifier;
	}

	namespace Citizen
	{
		static int movementSpeedModifier;
	}

	enum DIFFICULTY { TEST, EASY, NORMAL, HARD };

	static DIFFICULTY currentDifficulty = DIFFICULTY::EASY;

	inline void setDifficulty(DIFFICULTY diff)
	{
		currentDifficulty = diff;
		switch (diff)
		{
		case DIFFICULTY::TEST:
			Player::missingHealthHealBonus = 0.5;
			Player::healthDrainModifier = -1000;
			Player::maxHealthModifier = 100000;
			break;
		case DIFFICULTY::EASY:
			Player::missingHealthHealBonus = 0.25;
			break;
		case DIFFICULTY::NORMAL:
			Player::missingHealthHealBonus = 0.2;
			Player::healthDrainModifier = 2;
			Player::highHealthDrainPenalty = 0.25;
			Player::maxHealthModifier = -7500;
			Player::eatHealModifier = -500;
			Player::attackHealthCostModifier = 75;
			Mage::attackDamageModifier = 10;
			Mage::attackRateModifier = 5;
			Mage::touchDamageModifier = 15;
			Mage::movementSpeedModifier = 1;
			Mage::healthDrainModifier = 2;
			Citizen::movementSpeedModifier = 1;
			break;
		case DIFFICULTY::HARD:
			Player::missingHealthHealBonus = 0.1;
			Player::healthDrainModifier = 5;
			Player::highHealthDrainPenalty = 2.5;
			Player::maxHealthModifier = -9000;
			Player::eatHealModifier = -750;
			Player::attackHealthCostModifier = 150;
			Mage::attackDamageModifier = 40;
			Mage::attackRateModifier = 5;
			Mage::touchDamageModifier = 100;
			Mage::movementSpeedModifier = 2;
			Mage::healthDrainModifier = 6;
			Citizen::movementSpeedModifier = 1;
			break;
		default:
			break;
		}
	}
}

#endif