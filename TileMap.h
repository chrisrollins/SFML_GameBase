#ifndef TILEMAP_H
#define TILEMAP_H

#include "SFML/Graphics.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>

#define F(n) static_cast<float>(n)
#define I(n) static_cast<int>(n)

namespace Engine
{
	class TileMap : public sf::Drawable, public sf::Transformable
	{
	private:
		std::vector<sf::Vector2f> safeSpawnPositions;
		sf::Vector2u tileStdSize;
		unsigned int numPerLine;
		unsigned int numPerColumn;
		int* tiles;
		sf::VertexArray m_vertices;
		sf::Texture m_tileset;
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
		{
			// apply the transform
			states.transform *= getTransform();

			// apply the tileset texture
			states.texture = &m_tileset;

			// draw the vertex array
			target.draw(m_vertices, states);
		}
	public:

		bool load(const std::string& tileset, const std::string& mapTable)
		{
			// load the tileset texture
			if (!this->m_tileset.loadFromFile(tileset))
				return false;

			int * tileTable;
			tileTable = readFromFile(mapTable);

			this->tiles = tileTable;
			// resize the vertex array to fit the level size
			m_vertices.setPrimitiveType(sf::Quads);
			m_vertices.resize(numPerLine * numPerColumn * 4);

			// populate the vertex array, with one quad per tile
			for (unsigned int i = 0; i < numPerLine; ++i)
			{
				for (unsigned int j = 0; j < numPerColumn; ++j)
				{
					// get the current tile number
					int tileNumber = tiles[i + j * numPerLine];

					// find its position in the tileset texture
					int tu = tileNumber % (m_tileset.getSize().x / this->tileStdSize.x);
					int tv = tileNumber / (m_tileset.getSize().x / this->tileStdSize.x);

					// get a pointer to the current tile's quad
					sf::Vertex* quad = &m_vertices[(i + j * numPerLine) * 4];

					// define its 4 corners

					quad[0].position = sf::Vector2f(F(i * this->tileStdSize.x), F(j * this->tileStdSize.y));
					quad[1].position = sf::Vector2f(F((i + 1) * this->tileStdSize.x), F(j * this->tileStdSize.y));
					quad[2].position = sf::Vector2f(F((i + 1) * this->tileStdSize.x), F((j + 1) * this->tileStdSize.y));
					quad[3].position = sf::Vector2f(F(i * this->tileStdSize.x), F((j + 1) * this->tileStdSize.y));

					// define its 4 texture coordinates
					quad[0].texCoords = sf::Vector2f(F(tu * this->tileStdSize.x), F(tv * this->tileStdSize.y));
					quad[1].texCoords = sf::Vector2f(F((tu + 1) * this->tileStdSize.x), F(tv * this->tileStdSize.y));
					quad[2].texCoords = sf::Vector2f(F((tu + 1) * this->tileStdSize.x), F((tv + 1) * this->tileStdSize.y));
					quad[3].texCoords = sf::Vector2f(F(tu * this->tileStdSize.x), F((tv + 1) * this->tileStdSize.y));
				}
			}

			//initialize safe spawn positions
			for (unsigned int i = 1; i < this->numPerLine - 1; i++)
			{
				for (unsigned int j = 1; j < this->numPerColumn - 1; j++)
				{
					bool anyObstacles = false;
					for (auto tile : {
						this->getTileAt(i - 1, j - 1),
						this->getTileAt(i - 1, j),
						this->getTileAt(i, j - 1),
						this->getTileAt(i + 1, j + 1),
						this->getTileAt(i + 1, j),
						this->getTileAt(i, j + 1),
						this->getTileAt(i - 1, j + 1),
						this->getTileAt(i + 1, j - 1)
						})
					{
						if (isTileTypeObstacle(tile))
						{
							anyObstacles = true;
							break;
						}
					}
					if (!anyObstacles)
					{
						sf::Vector2f pos = this->getTileCenter(i, j);
						this->safeSpawnPositions.push_back(pos);
					}
				}
			}

			return true;
		}

		static bool isTileTypeObstacle(int tileType)
		{
			return (tileType == 0 || tileType == 1 || tileType == 2 || tileType == 9 || tileType == 10 || tileType == 11 || tileType == 18 || tileType == 19 || tileType == 20);
		}

		static bool isTileTypeTrap(int tileType)
		{
			return (tileType == 6 || tileType == 7 || tileType == 8 || tileType == 15 || tileType == 16 || tileType == 17 || tileType == 25 || tileType == 26);
		}

		bool isObstacle(sf::Vector2f position) const
		{
			int row = I(position.x / F(this->tileSize().x));
			int column = I(position.y / F(this->tileSize().y));
			int tileType = this->getTileAt(row, column);
			return isTileTypeObstacle(tileType);
		}

		bool isTrap(sf::Vector2f position) const
		{
			int row = I(position.x / F(this->tileSize().x));
			int column = I(position.y / F(this->tileSize().y));
			int tileType = this->getTileAt(row, column);
			return isTileTypeTrap(tileType);
		}

		sf::Vector2f getTileCenter(int i, int j) const
		{
			return { F(this->tileSize().x * i), F(this->tileSize().x * j) };
		}

		const std::vector<sf::Vector2f>& getSafeSpawnPositions() const
		{
			return this->safeSpawnPositions;
		}

		sf::FloatRect currTile(sf::Vector2f position) const
		{
			float row = position.x / F(this->tileSize().x);
			float column = position.y / F(this->tileSize().y);
			return sf::FloatRect(row * this->tileSize().x, column * F(this->tileSize().y), F(this->tileSize().x), F(this->tileSize().y));
		}

		unsigned int width() const
		{
			return this->numPerLine;
		}

		unsigned int height() const
		{
			return this->numPerColumn;
		}

		sf::Vector2u tileSize() const
		{
			return this->tileStdSize;
		}

		int getTileAt(int i, int j) const
		{
			return tiles[i + j * this->width()];
		}

		int* readFromFile(std::string mapTable)
		{
			std::ifstream fin(mapTable.c_str());

			if (!fin) {
				std::cout << "Unable to open input file " << mapTable << std::endl;
				exit(1);
			}
			//read width and size from the file
			fin >> this->numPerLine;
			fin >> this->numPerColumn;
			fin >> this->tileStdSize.x;
			fin >> this->tileStdSize.y;

			//determine the size of the array
			int size = this->numPerColumn * this->numPerLine;
			int *tileTable = nullptr; // pointer to int
			tileTable = new int[size];
			for (int i = 0; i < size; i++) { fin >> tileTable[i]; }
			fin.close();
			return tileTable;
		}
	};
}

#undef F
#undef I

#endif
