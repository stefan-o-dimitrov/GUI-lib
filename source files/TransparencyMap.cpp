#include "../include/GUI/TransparencyMap.h"

namespace gui
{
	TransparencyMap::TransparencyMap(const sf::Texture& tex)
	{
		mapSize = sf::Vector2i(tex.getSize());
		generateTransparencyMap(tex);
	}

	TransparencyMap::TransparencyMap(const TransparencyMap& copy)
	{
		mapSize = copy.mapSize;
		if (copy.transparency)
		{
			transparency.reset(new std::unique_ptr<bool[]>[mapSize.x]);
			for (unsigned short i = 0, end = mapSize.x; i != end; i++)
			{
				transparency[i].reset(new bool[mapSize.y]);
				for (unsigned short j = 0, end1 = mapSize.y; j != end1; j++)
					transparency[i][j] = copy.transparency[i][j];
			}
		}
	}

	const bool TransparencyMap::operator[](const sf::Vector2i& position)const
	{
		if (transparency != nullptr && position.x >= 0 && position.y >= 0 && position.x < mapSize.x && position.y < mapSize.y)
			return transparency[position.x][position.y];
		return true;
	}

	void TransparencyMap::generateTransparencyMap(const sf::Texture& tex)
	{
		const sf::Image transpCheck(tex.copyToImage());

		transparency.reset(new std::unique_ptr<bool[]>[mapSize.x]);
		for (unsigned short i = 0; i < mapSize.x; i++)
		{
			transparency[i].reset(new bool[mapSize.y]);
			for (unsigned short j = 0; j < mapSize.y; j++)
				transparency[i][j] = transpCheck.getPixel(i, j).a < 25 ? true : false;
		}
	}
}