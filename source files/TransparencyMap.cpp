#include "../include/GUI/TransparencyMap.h"

namespace gui
{
	TransparencyMap::TransparencyMap(const sf::Texture& tex)
	{
		m_mapSize = sf::Vector2i(tex.getSize());
		generateTransparencyMap(tex);
	}

	TransparencyMap::TransparencyMap(const TransparencyMap& copy)
	{
		m_mapSize = copy.m_mapSize;
		if (copy.m_transparency)
		{
			m_transparency.reset(new std::unique_ptr<bool[]>[m_mapSize.x]);
			for (unsigned short i = 0, end = m_mapSize.x; i != end; i++)
			{
				m_transparency[i].reset(new bool[m_mapSize.y]);
				for (unsigned short j = 0, end1 = m_mapSize.y; j != end1; j++)
					m_transparency[i][j] = copy.m_transparency[i][j];
			}
		}
	}

	const bool TransparencyMap::operator[](const sf::Vector2i& position)const
	{
		if (m_transparency != nullptr && position.x >= 0 && position.y >= 0 && position.x < m_mapSize.x && position.y < m_mapSize.y)
			return m_transparency[position.x][position.y];
		return true;
	}

	void TransparencyMap::generateTransparencyMap(const sf::Texture& tex)
	{
		const sf::Image transpCheck(tex.copyToImage());

		m_transparency.reset(new std::unique_ptr<bool[]>[m_mapSize.x]);
		for (unsigned short i = 0; i < m_mapSize.x; i++)
		{
			m_transparency[i].reset(new bool[m_mapSize.y]);
			for (unsigned short j = 0; j < m_mapSize.y; j++)
				m_transparency[i][j] = transpCheck.getPixel(i, j).a < 25 ? true : false;
		}
	}
}