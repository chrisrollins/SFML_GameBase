#ifndef DIFFICULTYSETTINGS_H
#define DIFFICULTYSETTINGS_H

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
		static int maxPotionNumModifier;
		static int potionMakingCitizenRequired;
	}

	namespace Mage
	{
		static int attackDamageModifier;
		static int touchDamageModifier;
		static int movementSpeedModifier;
		static int healthDrainModifier;
		static int mageHealthModifier;
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

	enum class DIFFICULTY { TEST, EASY, NORMAL, HARD };

	static DIFFICULTY currentDifficulty = DIFFICULTY::EASY;

	inline void setDifficulty(DIFFICULTY diff)
	{
		Player::maxHealthModifier = 0;
		Player::eatHealModifier = 0;
		Player::eatDrainFreezeDuration = 0;
		Player::missingHealthHealBonus = 0.f;
		Player::healthDrainModifier = 0;
		Player::highHealthDrainPenalty = 0.f;
		Player::attackHealthCostModifier = 0;
		Player::maxPotionNumModifier = 0;
		Player::potionMakingCitizenRequired = 0;
		Mage::attackDamageModifier = 0;
		Mage::touchDamageModifier = 0;
		Mage::movementSpeedModifier = 0;
		Mage::healthDrainModifier = 0;
		Mage::mageHealthModifier = 0;
		Mage::blastSpeedModifier = 0.f;
		Citizen::movementSpeedModifier = 0;
		Score::multiplierPerSecond = 0.f;
		Score::baseMultiplier = 0.f;
		Score::cumulativeBonusMultiplier = 0.f;
		Score::cumulativeBonusMultiplierMax = 0.f;
		Score::cumulativeBonusMultiplierCurrent = 1.0f;
		currentDifficulty = diff;
		switch (diff)
		{
		case DIFFICULTY::TEST:
			Map::picture = "tileset.png";
			Map::fileName = "map_easy.txt";
			Player::missingHealthHealBonus = 0.6f;
			Player::healthDrainModifier = -100000;
			Player::maxHealthModifier = 100000;
			Player::maxPotionNumModifier = 9994;
			Player::eatDrainFreezeDuration = 12;
			Player::potionMakingCitizenRequired = 1;
			Mage::attackDamageModifier = -1000;
			Mage::touchDamageModifier = -1000;
			Mage::healthDrainModifier = -1000;
			Score::baseMultiplier = 0.f;
			Score::cumulativeBonusMultiplier = 0.0f;
			Score::cumulativeBonusMultiplierMax = 0.0f;
			Score::multiplierPerSecond = 0.0f;
			break;
		case DIFFICULTY::EASY:
			Map::picture = "tileset.png";
			Map::fileName = "map_easy.txt";
			Player::missingHealthHealBonus = 0.5f;
			Player::highHealthDrainPenalty = 0.4f;
			Player::eatDrainFreezeDuration = 12;
			Player::potionMakingCitizenRequired = 4;
			Score::baseMultiplier = 1.0f;
			Score::cumulativeBonusMultiplier = 0.02f;
			Score::cumulativeBonusMultiplierMax = 1.10f;
			Score::multiplierPerSecond = -0.001f;
			break;
		case DIFFICULTY::NORMAL:
			Map::picture = "tileset.png";
			Map::fileName = "map_normal.txt";
			Player::missingHealthHealBonus = 0.3f;
			Player::healthDrainModifier = 1;
			Player::highHealthDrainPenalty = 0.7f;
			Player::eatDrainFreezeDuration = 11;
			Player::maxHealthModifier = -4500;
			Player::attackHealthCostModifier = 150;
			Player::potionMakingCitizenRequired = 5;
			Player::maxPotionNumModifier = -1;
			Mage::attackDamageModifier = 50;
			Mage::blastSpeedModifier = 0.8f;
			Mage::touchDamageModifier = 10;
			Mage::movementSpeedModifier = 1;
			Mage::healthDrainModifier = 1;
			Mage::mageHealthModifier = 2;
			Citizen::movementSpeedModifier = 1;
			Score::baseMultiplier = 1.5f;
			Score::cumulativeBonusMultiplier = 0.03f;
			Score::cumulativeBonusMultiplierMax = 1.29f;
			Score::multiplierPerSecond = 0.0005f;
			break;
		case DIFFICULTY::HARD:
			Map::picture = "tileset.png";
			Map::fileName = "map_insane.txt";
			Player::missingHealthHealBonus = 0.20f;
			Player::healthDrainModifier = 1;
			Player::highHealthDrainPenalty = 3.0f;
			Player::eatDrainFreezeDuration = 10;
			Player::maxHealthModifier = -6000;
			Player::attackHealthCostModifier = 390;
			Player::potionMakingCitizenRequired = 6;
			Player::maxPotionNumModifier = -2;
			Mage::attackDamageModifier = 150;
			Mage::blastSpeedModifier = 1.5f;
			Mage::touchDamageModifier = 40;
			Mage::movementSpeedModifier = 2;
			Mage::healthDrainModifier = 2;
			Mage::mageHealthModifier = 7;
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
