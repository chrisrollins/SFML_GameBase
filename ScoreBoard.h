#ifndef SCOREBOARD_HEADER
#define SCOREBOARD_HEADER

#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <vector>
#include "DifficultySettings.h"
#include "GameObject.h"

namespace Engine
{
	struct ScoreEntry
	{
	public:
		ScoreEntry() : ScoreEntry("", 0, 0)
		{

		}
		ScoreEntry(std::string name, int score, int timeInSeconds) : score(score), time(timeInSeconds)
		{			
			size_t length = name.size();
			if (length > 255) { length = 255; }
			if (length > 0) { memcpy(this->name, name.c_str(), length);	}
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
	private:
		int score;
		int time;
		char name[256] = { };
	};

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
			return numScores;
		}
		const ScoreEntry* getScores() const
		{
			return this->scores;
		}
		void loadFromDataFile(std::string fileName)
		{
			std::ifstream file(fileName, std::ios::binary);
			file.read(reinterpret_cast<char*>(this->scores), sizeof(ScoreEntry) * numScores);
			file.close();
		}
		void writeToDataFile(std::string fileName)
		{
			std::ofstream file(fileName, std::ios::binary | std::ios::trunc);
			file.write(reinterpret_cast<char*>(this->scores), sizeof(ScoreEntry) * numScores);
			file.close();
		}
	};

	class ScoreBoard : public GraphicalGameObject
	{
	public:
		ScoreBoard() : GraphicalGameObject(sf::RectangleShape()), easyFileName("easyScores.dat"), normalFileName("normalScores.dat"), hardFileName("hardScores.dat")
		{
			this->xButtonTexture.loadFromFile("x_button.png");
			this->xButtonSprite.setTexture(this->xButtonTexture);
			this->easyScores.loadFromDataFile(this->easyFileName);
			this->normalScores.loadFromDataFile(this->normalFileName);
			this->hardScores.loadFromDataFile(this->hardFileName);
			this->font.loadFromFile("zombie.ttf");
			for (auto txt : { &this->easyColumn, &this->normalColumn, &this->hardColumn })
			{
				txt->setFillColor({ 255, 0, 0, 255 });
				txt->setOutlineColor({ 255, 0, 0, 255 });
				txt->setFont(this->font);
			}
			sf::Vector2f bgSize(static_cast<float>(Screen::windowWidth), static_cast<float>(Screen::windowHeight));
			this->backgroundPtr()->setSize(bgSize);
			this->backgroundPtr()->setFillColor({ 10, 10, 10, 255 });
			this->backgroundPtr()->setPosition(0.f, 0.f);
			this->xButtonSprite.setPosition({ bgSize.x - 55.f, 15.f });
			float colSpacing = bgSize.x / 4.f + 80.f;
			float easyColumnX = 20.f;
			float normalColumnX = easyColumnX + colSpacing;
			float hardColumnX = normalColumnX + colSpacing;
			this->easyColumn.setPosition(easyColumnX, 70.f);
			this->normalColumn.setPosition(normalColumnX, 80.f);
			this->hardColumn.setPosition(hardColumnX, 90.f);
			updateColumnText(&this->easyColumn, &this->easyScores);
			updateColumnText(&this->normalColumn, &this->normalScores);
			updateColumnText(&this->hardColumn, &this->hardScores);
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
			if (this->xButtonSprite.getGlobalBounds().contains(static_cast<float>(mouse.x), static_cast<float>(mouse.y)))
			{
				this->screen->remove(this);
			}
		}
		void draw(sf::RenderWindow& win)
		{			
			win.draw(*this->graphic);
			win.draw(this->xButtonSprite);
			win.draw(this->easyColumn);
			win.draw(this->normalColumn);
			win.draw(this->hardColumn);
		}
	private:
		void updateColumnText(sf::Text* column, ScoreList* list)
		{
			std::ostringstream stream;
			const ScoreEntry* scoreArray = list->getScores();
			const char* columnLabel = (column == &this->easyColumn) ? "Easy" : (column == &this->normalColumn) ? "Normal" : "Hard";			
			const int nameWidth = 10;
			const int scoreWidth = 7;
			const int timeWidth = 5;
			stream << setw(nameWidth + scoreWidth + timeWidth) << columnLabel << '\n';
			stream << std::setw(nameWidth) << "Name" << "     " << std::setw(scoreWidth) << "Score" << "     " << setw(timeWidth) << "Time\n";
			for (int i = list->getBoardSize() - 1; i >= 0; i--)
			{
				stream << std::left << std::setw(nameWidth) << scoreArray[i].getName() << "  -  " << std::setw(scoreWidth) << std::to_string(scoreArray[i].getScore()) << "  -  " << setw(timeWidth) << std::to_string(scoreArray[i].getTimeInSeconds()) << '\n';
			}
			//std::cout << stream.str() << std::endl;
			column->setString(stream.str());			
		}
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
		sf::Sprite xButtonSprite;
		sf::RectangleShape* backgroundPtr() { return dynamic_cast<sf::RectangleShape*>(this->graphic); }
	};
}

#endif