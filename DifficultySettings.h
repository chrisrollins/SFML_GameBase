#ifndef DIFFICULTY_SETTINGS_HEADER
#define DIFFICULTY_SETTINGS_HEADER

#include <string>

namespace DifficultySettings
{
	namespace Map
	{
		static std::string picture;
		static std::string fileName;
	}
	// modifiers are added to the base value
	// example: Player::maxHealthModifier increases (if positive) or decreases (if negative) the player's max health
	namespace Player
	{
		static int maxHealthModifier;
		static int eatHealModifier;
		static int eatDrainFreezeDuration;
		static float missingHealthHealBonus;
		static int healthDrainModifier;
		static float highHealthDrainPenalty;
		static int attackHealthCostModifier;
	}

	namespace Mage
	{
		static int attackDamageModifier;
		static int touchDamageModifier;
		static int movementSpeedModifier;
		static int healthDrainModifier;
		static float blastSpeedModifier;
	}

	namespace Citizen
	{
		static int movementSpeedModifier;
	}

	namespace Score
	{
		static float multiplierPerSecond;
		static float baseMultiplier;
		static float cumulativeBonusMultiplier;
		static float cumulativeBonusMultiplierCurrent;
		static float cumulativeBonusMultiplierMax;
		inline int applyMultipliers(int baseIncrease)
		{
			int result = baseIncrease * static_cast<int>(cumulativeBonusMultiplierCurrent * baseMultiplier);
			return (result > 0) ? result : 1;
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
			Map::picture = "tileset.png";
			Map::fileName = "map_easy.txt";
			Player::missingHealthHealBonus = 0.6f;
			Player::healthDrainModifier = -1000;
			Player::maxHealthModifier = 100000;
			Player::eatDrainFreezeDuration = 12;
			Score::baseMultiplier = 0.f;
			Score::cumulativeBonusMultiplier = 0.0f;
			Score::cumulativeBonusMultiplierMax = 0.0f;
			Score::multiplierPerSecond = 0.0f;
			break;
		case DIFFICULTY::EASY:
			Map::picture = "tileset.png";
			Map::fileName = "map_easy.txt";
			Player::missingHealthHealBonus = 0.6f;
			Player::highHealthDrainPenalty = 0.15f;
			Player::eatDrainFreezeDuration = 12;
			Score::baseMultiplier = 1.0f;
			Score::cumulativeBonusMultiplier = 0.03f;
			Score::cumulativeBonusMultiplierMax = 1.15f;
			Score::multiplierPerSecond = -0.002f;
			break;
		case DIFFICULTY::NORMAL:
			Map::picture = "tileset.png";
			Map::fileName = "map_normal.txt";
			Player::missingHealthHealBonus = 0.3f;
			Player::healthDrainModifier = 1;
			Player::highHealthDrainPenalty = 0.4f;
			Player::eatDrainFreezeDuration = 11;
			Player::maxHealthModifier = -7500;
			Player::eatHealModifier = -300;
			Player::attackHealthCostModifier = 70;
			Mage::attackDamageModifier = 10;
			Mage::blastSpeedModifier = 0.25f;
			Mage::touchDamageModifier = 20;
			Mage::movementSpeedModifier = 1;
			Mage::healthDrainModifier = 1;
			Citizen::movementSpeedModifier = 1;
			Score::baseMultiplier = 1.97f;
			Score::cumulativeBonusMultiplier = 0.06f;
			Score::cumulativeBonusMultiplierMax = 1.3f;
			Score::multiplierPerSecond = 0.0005f;
			break;
		case DIFFICULTY::HARD:
			Map::picture = "tileset.png";
			Map::fileName = "map_insane.txt";
			Player::missingHealthHealBonus = 0.20f;
			Player::healthDrainModifier = 1;
			Player::highHealthDrainPenalty = 5.0f;
			Player::eatDrainFreezeDuration = 10;
			Player::maxHealthModifier = -9000;
			Player::eatHealModifier = -450;
			Player::attackHealthCostModifier = 260;
			Mage::attackDamageModifier = 80;
			Mage::blastSpeedModifier = 1.6f;
			Mage::touchDamageModifier = 95;
			Mage::movementSpeedModifier = 2;
			Mage::healthDrainModifier = 2;
			Citizen::movementSpeedModifier = 1;
			Score::baseMultiplier = 2.5f;
			Score::cumulativeBonusMultiplier = 0.02f;
			Score::cumulativeBonusMultiplierMax = 10.f;
			Score::multiplierPerSecond = 0.006f;
			break;
		default:
			break;
		}
	}
}

#endif