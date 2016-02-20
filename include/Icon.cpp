#include "Icon.h"

namespace gui
{
	Icon::Icon(const sf::Texture& tex, const bool transparencyCheck)
	{
		setTexture(tex, transparencyCheck);
	}

	Icon::Icon(const Icon& copy)
		: Hoverable(copy), spr(copy.spr)
	{
		if (copy.transparency) transparency.reset(new TransparencyMap(*copy.transparency));
	}

	Icon::Icon(Icon&& temp)
		: Hoverable(std::move(temp)), transparency(std::move(temp.transparency)), spr(temp.spr) {}

	const bool Icon::contains(const sf::Vector2f& pos)const
	{
		if (spr.getGlobalBounds().contains(pos))
		{
			if (!transparency) return true;
			if (!(*transparency)[sf::Vector2i(pos.x - spr.getPosition().x, pos.y - spr.getPosition().y)]) return true;
		}
		return false;
	}

	const sf::FloatRect Icon::getGlobalBounds()const
	{
		return spr.getGlobalBounds();
	}

	const sf::Vector2f&  Icon::getPosition()const
	{
		return spr.getPosition();
	}

	const sf::Texture& Icon::getTexture()const
	{
		return *spr.getTexture();
	}

	const bool Icon::getTransparencyCheck()const
	{
		return transparency != nullptr;
	}

	const sf::IntRect& Icon::getTextureRect()const
	{
		return spr.getTextureRect();
	}

	Icon& Icon::setPosition(const float x, const float y)
	{
		spr.setPosition(x, y);
		return *this;
	}

	Icon& Icon::setTexture(const sf::Texture& tex, const bool transparencyCheck)
	{
		spr.setTexture(tex);
		setTransparencyCheck(transparencyCheck);
		return *this;
	}

	Icon& Icon::setTransparencyCheck(const bool newTransparencyCheck)
	{
		if (!newTransparencyCheck) { transparency.reset(); return *this; }
		transparency.reset(new TransparencyMap(*spr.getTexture()));
		return *this;
	}

	Icon& Icon::setTextureRect(const sf::IntRect& rect)
	{
		spr.setTextureRect(rect);
		return *this;
	}

	void Icon::draw(sf::RenderTarget& target, sf::RenderStates states)const
	{
		target.draw(spr, states);
		Hoverable::draw(target, states);
	}


	Icon::TransparencyMap::TransparencyMap(const sf::Texture& tex)
	{
		mapSize = sf::Vector2i(tex.getSize());
		generateTransparencyMap(tex);
	}

	Icon::TransparencyMap::TransparencyMap(const TransparencyMap& copy)
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

	const bool Icon::TransparencyMap::operator[](const sf::Vector2i& position)const
	{
		if (transparency != nullptr && position.x >= 0 && position.y >= 0 && position.x < mapSize.x && position.y < mapSize.y)
			return transparency[position.x][position.y];
		return true;
	}

	void Icon::TransparencyMap::generateTransparencyMap(const sf::Texture& tex)
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