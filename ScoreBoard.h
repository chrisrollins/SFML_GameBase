#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include "Menu.h"
#include "FileLoadException.h"
#include "DifficultySettings.h"
#include "GameObject.h"
#include "Screen.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <vector>

namespace Engine
{
	struct ScoreEntry
	{
	private:
		int score;
		int time;
		char name[256] = { };
	public:
		ScoreEntry() : ScoreEntry("", 0, 0) {}

		ScoreEntry(std::string name, int score, int timeInSeconds) : score(score), time(timeInSeconds)
		{
			size_t length = name.size();
			if (length > 255) { length = 255; }
			if (length > 0) { memcpy(this->name, name.c_str(), length); }
		}

		int getScore() const
		{
			return this->score;
		}

		int getTimeInSeconds() const
		{
			return this->time;
		}

		std::string getName() const
		{
			return std::string(this->name);
		}

		bool operator >(const ScoreEntry& other) const
		{
			return this->score > other.getScore();
		}

		bool operator <(const ScoreEntry& other) const
		{
			return this->score < other.getScore();
		}

		bool operator ==(const ScoreEntry& other) const
		{
			return this->score == other.getScore();
		}

		static const int rankWidth = 8;
		static const int nameWidth = 16;
		static const int scoreWidth = 12;
		static const int timeWidth = 8;
	};

	inline std::ostream& operator << (std::ostream& os, const ScoreEntry& entry)
	{
		if (entry.getName().length() >= 10) { os << entry.getName().substr(0, 10); }
		else { os << entry.getName(); }
		os << std::setw(ScoreEntry::scoreWidth) << std::to_string(entry.getScore());
		int minute = static_cast<int>(entry.getTimeInSeconds() / 60);
		int second = static_cast<int>(entry.getTimeInSeconds() - minute * 60);
		if (minute < 10) { os << "0"; }
		os << minute << ":";
		if (second < 10) { os << "0"; }
		os << second;
		return os;
	}

	class ScoreList
	{
	private:
		static int sortFunc(const void* ptr1, const void* ptr2)
		{
			const ScoreEntry* s1 = reinterpret_cast<const ScoreEntry*>(ptr1);
			const ScoreEntry* s2 = reinterpret_cast<const ScoreEntry*>(ptr2);
			if ((*s1) < (*s2)) { return -1; }
			else if ((*s1) > (*s2)) { return 1; }
			else { return 0; }
		}
		static const size_t numScores = 10;
		ScoreEntry scores[numScores] = { };
	public:
		bool add(ScoreEntry s)
		{
			int scoreValue = s.getScore();
			if (scoreValue < this->scores[0].getScore()) { return false; }
			this->scores[0] = s;
			qsort(this->scores, numScores, sizeof(ScoreEntry), sortFunc);
			return true;
		}

		size_t getBoardSize() const
		{
			return this->numScores;
		}

		const ScoreEntry* getScores() const
		{
			return this->scores;
		}

		void loadFromDataFile(std::string fileName)
		{
			std::ifstream file(fileName, std::ios::binary);
			if (file.bad()) { throw GameException::DataFileLoadException(fileName); }
			file.read(reinterpret_cast<char*>(this->scores), sizeof(ScoreEntry) * numScores);
			file.close();
		}

		void writeToDataFile(std::string fileName)
		{
			std::ofstream file(fileName, std::ios::binary | std::ios::trunc);
			if (file.bad()) { throw GameException::DataFileLoadException(fileName); }
			file.write(reinterpret_cast<char*>(this->scores), sizeof(ScoreEntry) * numScores);
			file.close();
		}
	};

	class ScoreBoard : public GraphicalGameObject
	{
	private:
		std::string easyFileName;
		std::string normalFileName;
		std::string hardFileName;
		ScoreList easyScores;
		ScoreList normalScores;
		ScoreList hardScores;
		sf::Font font;
		sf::Text easyColumn;
		sf::Text normalColumn;
		sf::Text hardColumn;
		sf::Texture xButtonTexture;
		sf::Sprite xButton;
		sf::Texture ezButtonTexture;
		sf::Sprite ezButton;
		sf::Texture normalButtonTexture;
		sf::Sprite normalButton;
		sf::Texture insaneButtonTexture;
		sf::Sprite insaneButton;
		sf::Texture backgroundTexture;
		sf::Sprite background;
		sf::Sprite* backgroundPtr() { return dynamic_cast<sf::Sprite*>(this->graphic); }
		bool showEasy = false;
		bool showNormal = false;
		bool showInsane = false;
		bool showScoreChoice = false; // used here to control when return to the first page of scoreboard
		void updateColumnText(sf::Text* column, ScoreList* list)
		{
			std::ostringstream stream;
			const ScoreEntry* scoreArray = list->getScores();
			const char* columnLabel = (column == &this->easyColumn) ? "Easy" : (column == &this->normalColumn) ? "Normal" : "Insane";
			stream << std::setw((ScoreEntry::rankWidth + ScoreEntry::nameWidth + ScoreEntry::scoreWidth + ScoreEntry::timeWidth) / 2) << columnLabel << std::endl;
			stream << std::left << std::setw(ScoreEntry::rankWidth) << "Rank" << std::setw(ScoreEntry::nameWidth) << "Name"
				<< std::setw(ScoreEntry::scoreWidth) << "Score" << std::setw(ScoreEntry::timeWidth) << "Time" << std::endl;
			for (int i = list->getBoardSize() - 1; i >= 0 && scoreArray[i].getScore() != 0; i--)
			{
				stream << std::left << std::setw(ScoreEntry::rankWidth) << list->getBoardSize() - i << std::setw(ScoreEntry::nameWidth);
				stream << scoreArray[i] << std::endl;
			}
			//std::cout << stream.str() << std::endl;
			column->setString(stream.str());
		}
	public:
		ScoreBoard() : GraphicalGameObject(sf::RectangleShape()), easyFileName("easyScores.dat"), normalFileName("normalScores.dat"), hardFileName("hardScores.dat")
		{
			if (!this->xButtonTexture.loadFromFile("x_button.png")) { throw GameException::ImageFileLoadException("x_button.png"); }
			this->xButton.setTexture(this->xButtonTexture);
			if (!this->backgroundTexture.loadFromFile("scoreboard.png")) { throw GameException::ImageFileLoadException("scoreboard.png"); }
			this->background.setTexture(this->backgroundTexture);
			if (!this->ezButtonTexture.loadFromFile("easy_button.png")) { throw GameException::ImageFileLoadException("easy_button.png"); }
			this->ezButton.setTexture(this->ezButtonTexture);
			if (!this->normalButtonTexture.loadFromFile("normal_button.png")) { throw GameException::ImageFileLoadException("normal_button.png"); };
			this->normalButton.setTexture(this->normalButtonTexture);
			if (!this->insaneButtonTexture.loadFromFile("insane_button.png")) { throw GameException::ImageFileLoadException("insane_button.png"); };
			this->insaneButton.setTexture(insaneButtonTexture);
			this->easyScores.loadFromDataFile(this->easyFileName);
			this->normalScores.loadFromDataFile(this->normalFileName);
			this->hardScores.loadFromDataFile(this->hardFileName);
			if (!this->font.loadFromFile("harting.ttf")) { throw GameException::FontFileLoadException("harting.ttf"); }
			for (auto txt : { &this->easyColumn, &this->normalColumn, &this->hardColumn })
			{
				txt->setFillColor({ 179, 45, 0, 255 });
				txt->setFont(this->font);
				txt->setStyle(sf::Text::Bold);
				txt->setCharacterSize(30);
				txt->setLineSpacing(1.5f);
				txt->setPosition({ 130.f, 80.f });
			}
			sf::Vector2f bgSize(static_cast<float>(Screen::windowWidth), static_cast<float>(Screen::windowHeight));
			this->xButton.setPosition({ bgSize.x - 55.f, 15.f });
			this->ezButton.setPosition({ 50.f, 150.f });
			this->normalButton.setPosition({ 375.f, 150.f });
			this->insaneButton.setPosition({ 700.f, 150.f });
			updateColumnText(&this->easyColumn, &this->easyScores);
			updateColumnText(&this->normalColumn, &this->normalScores);
			updateColumnText(&this->hardColumn, &this->hardScores);
			for (auto obj : Menu::getCurrentMenu()->getMenuObjects()) { if (obj != this) { obj->disableEvents(); } }
		}

		void RemovedFromScreen()
		{
			for (auto obj : Menu::getCurrentMenu()->getMenuObjects()) { if (obj != this) { obj->enableEvents(); } }
		}

		bool add(ScoreEntry s, DifficultySettings::DIFFICULTY difficulty)
		{
			ScoreList* list = nullptr;
			sf::Text* column = nullptr;
			std::string* dataFileName = nullptr;
			switch (difficulty)
			{
			case DifficultySettings::DIFFICULTY::EASY:
				list = &this->easyScores;
				column = &this->easyColumn;
				dataFileName = &this->easyFileName;
				break;
			case DifficultySettings::DIFFICULTY::NORMAL:
				list = &this->normalScores;
				column = &this->normalColumn;
				dataFileName = &this->normalFileName;
				break;
			case DifficultySettings::DIFFICULTY::HARD:
				list = &this->hardScores;
				column = &this->hardColumn;
				dataFileName = &this->hardFileName;
				break;
			default:
				return false;
			}
			if (!list->add(s)) { return false; }
			list->writeToDataFile(*dataFileName);
			updateColumnText(column, list);
			return true;
		}

		void MouseButtonReleased(sf::Event e)
		{
			if (e.mouseButton.button != sf::Mouse::Button::Left) { return; }
			sf::Vector2i mouse = this->screen->getMousePosition();
			if (!this->showScoreChoice && this->xButton.getGlobalBounds().contains(static_cast<float>(mouse.x), static_cast<float>(mouse.y)))
			{
				this->screen->remove(this);
			}
			if (this->showScoreChoice && this->xButton.getGlobalBounds().contains(static_cast<float>(mouse.x), static_cast<float>(mouse.y)))
			{
				this->showEasy = false;
				this->showNormal = false;
				this->showInsane = false;
			}
			if (this->ezButton.getGlobalBounds().contains(static_cast<float>(mouse.x), static_cast<float>(mouse.y)))
			{
				this->showEasy = true;
				this->showScoreChoice = true;
			}
			if (this->normalButton.getGlobalBounds().contains(static_cast<float>(mouse.x), static_cast<float>(mouse.y)))
			{
				this->showNormal = true;
				this->showScoreChoice = true;
			}
			if (this->insaneButton.getGlobalBounds().contains(static_cast<float>(mouse.x), static_cast<float>(mouse.y)))
			{
				this->showInsane = true;
				this->showScoreChoice = true;
			}
		}

		void draw(sf::RenderWindow& win)
		{
			win.clear();
			if (!this->showEasy && !this->showNormal && !this->showInsane)
			{
				win.draw(this->background);
				win.draw(this->ezButton);
				win.draw(this->normalButton);
				win.draw(this->insaneButton);
				win.draw(this->xButton);
				this->showScoreChoice = false;
			}
			else if (this->showEasy)
			{
				win.draw(this->easyColumn);
				win.draw(this->xButton);
			}
			else if (this->showNormal)
			{
				win.draw(this->normalColumn);
				win.draw(this->xButton);
			}
			else
			{
				win.draw(this->hardColumn);
				win.draw(this->xButton);
			}
		}
	};
}

#endif
