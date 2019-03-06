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

	namespace Score
	{
		static float baseMultiplier;
		static float cumulativeBonusMultiplier;
		static float cumulativeBonusMultiplierCurrent;
		static float cumulativeBonusMultiplierMax;
		inline int applyMultipliers(int baseIncrease)
		{
			return baseIncrease * cumulativeBonusMultiplierCurrent * baseMultiplier;
		}
	}

	enum DIFFICULTY { TEST, EASY, NORMAL, HARD };

	static DIFFICULTY currentDifficulty = DIFFICULTY::EASY;

	inline void setDifficulty(DIFFICULTY diff)
	{
		currentDifficulty = diff;
		Score::cumulativeBonusMultiplierCurrent = 1.0f;
		switch (diff)
		{
		case DIFFICULTY::TEST:
			Player::missingHealthHealBonus = 0.6f;
			Player::healthDrainModifier = -1000;
			Player::maxHealthModifier = 100000;
			Score::baseMultiplier = 0.f;
			Score::cumulativeBonusMultiplier = 0.0f;
			Score::cumulativeBonusMultiplierMax = 0.0f;
			break;
		case DIFFICULTY::EASY:
			Player::missingHealthHealBonus = 0.35f;
			Player::highHealthDrainPenalty = 0.15f;
			Score::baseMultiplier = 1.0f;
			Score::cumulativeBonusMultiplier = 0.03f;
			Score::cumulativeBonusMultiplierMax = 1.15f;
			break;
		case DIFFICULTY::NORMAL:
			Player::missingHealthHealBonus = 0.3f;
			Player::healthDrainModifier = 1;
			Player::highHealthDrainPenalty = 0.5f;
			Player::maxHealthModifier = -7500;
			Player::eatHealModifier = -300;
			Player::attackHealthCostModifier = 70;
			Mage::attackDamageModifier = 10;
			Mage::attackRateModifier = 5;
			Mage::touchDamageModifier = 20;
			Mage::movementSpeedModifier = 1;
			Mage::healthDrainModifier = 3;
			Citizen::movementSpeedModifier = 1;
			Score::baseMultiplier = 2.0f;
			Score::cumulativeBonusMultiplier = 0.06f;
			Score::cumulativeBonusMultiplierMax = 1.3f;
			break;
		case DIFFICULTY::HARD:
			Player::missingHealthHealBonus = 0.22f;
			Player::healthDrainModifier = 1;
			Player::highHealthDrainPenalty = 5.0f;
			Player::maxHealthModifier = -9300;
			Player::eatHealModifier = -450;
			Player::attackHealthCostModifier = 200;
			Mage::attackDamageModifier = 80;
			Mage::attackRateModifier = 5;
			Mage::touchDamageModifier = 95;
			Mage::movementSpeedModifier = 2;
			Mage::healthDrainModifier = 7;
			Citizen::movementSpeedModifier = 1;
			Score::baseMultiplier = 9.0f;
			Score::cumulativeBonusMultiplier = 0.07f;
			Score::cumulativeBonusMultiplierMax = 2.7f;
			break;
		default:
			break;
		}
	}
}

#endif