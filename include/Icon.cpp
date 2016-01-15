#include "Icon.h"

gui::Icon::Icon(const sf::Texture& tex, const bool transparencyCheck)
{
	spr.setTexture(tex, transparencyCheck);
}

gui::Icon::Icon(const Icon& _lVal)
	: Hoverable(_lVal), spr(_lVal.spr)
{
	if (_lVal.transparency) transparency.reset(new TransparencyMap(*_lVal.transparency));
}

gui::Icon::Icon(Icon&& _rVal)
	: Hoverable(std::move(_rVal)), transparency(std::move(_rVal.transparency)), spr(_rVal.spr) {}

gui::Icon& gui::Icon::operator=(const Icon& _lVal)
{
	Hoverable::operator=(_lVal);
	if (_lVal.transparency) transparency.reset(new TransparencyMap(*_lVal.transparency));
	spr = _lVal.spr;
	return *this;
}

gui::Icon& gui::Icon::operator=(Icon&& _rVal)
{
	Hoverable::operator=(std::move(_rVal));
	transparency = std::move(_rVal.transparency);
	spr = _rVal.spr;
	return *this;
}

const bool gui::Icon::contains(const sf::Vector2f& pos)const
{
	if (spr.getGlobalBounds().contains(pos))
	{
		if (!transparency) return true;
		if (pos.x >= spr.getPosition().x &&
			pos.y >= spr.getPosition().y &&
			pos.x < spr.getTexture()->getSize().x &&
			pos.y < spr.getTexture()->getSize().y &&
			!(*transparency)[sf::Vector2i(pos.x - spr.getPosition().x, pos.y - spr.getPosition().y)])
			 return true;
	}
	return false;
}

const sf::FloatRect gui::Icon::getGlobalBounds()const
{
	return spr.getGlobalBounds();
}

const sf::Vector2f&  gui::Icon::getPosition()const
{
	return spr.getPosition();
}

const sf::Texture& gui::Icon::getTexture()const
{
	return *spr.getTexture();
}

const bool gui::Icon::getTransparencyCheck()const
{
	return transparency != nullptr;
}

const sf::IntRect& gui::Icon::getTextureRect()const
{
	return spr.getTextureRect();
}

gui::Icon& gui::Icon::setPosition(const float x, const float y)
{
	spr.setPosition(x, y);
	return *this;
}

gui::Icon& gui::Icon::setTexture(const sf::Texture& tex, const bool transparencyCheck)
{
	spr.setTexture(tex);
	setTransparencyCheck(transparencyCheck);
	return *this;
}

gui::Icon& gui::Icon::setTransparencyCheck(const bool _tVal)
{
	if (!_tVal){ transparency.reset(); return *this; }
	transparency.reset(new TransparencyMap(*spr.getTexture()));
	return *this;
}

gui::Icon& gui::Icon::setTextureRect(const sf::IntRect& rect)
{
	spr.setTextureRect(rect);
	return *this;
}

void gui::Icon::draw(sf::RenderTarget& target, sf::RenderStates states)const
{
	target.draw(spr, states);
	Hoverable::draw(target, states);
}


gui::Icon::TransparencyMap::TransparencyMap(const sf::Texture& tex)
{
	mapSize = sf::Vector2i(tex.getSize());
	generateTransparencyMap(tex);
}

gui::Icon::TransparencyMap::TransparencyMap(const TransparencyMap& _rVal)
{
	mapSize = _rVal.mapSize;
	if (_rVal.transparency)
	{
		transparency.reset(new std::unique_ptr<bool[]>[mapSize.x]);
		for (unsigned short i = 0, end = mapSize.x; i != end; i++)
		{
			transparency[i].reset(new bool[mapSize.y]);
			for (unsigned short j = 0, end1 = mapSize.y; j != end1; j++)
				transparency[i][j] = _rVal.transparency[i][j];
		}
	}
}

const bool gui::Icon::TransparencyMap::operator[](const sf::Vector2i& _rVal)const
{
	if (transparency != nullptr && _rVal.x >= 0 && _rVal.y >= 0 && _rVal.x < mapSize.x && _rVal.y < mapSize.y) return transparency[_rVal.x][_rVal.y];
	return true;
}

gui::Icon::TransparencyMap& gui::Icon::TransparencyMap::operator=(const TransparencyMap& _rVal)
{
	mapSize = _rVal.mapSize;
	if (_rVal.transparency)
	{
		transparency.reset(new std::unique_ptr<bool[]>[mapSize.x]);
		for (unsigned short i = 0, end = mapSize.x; i != end; i++)
		{
			transparency[i].reset(new bool[mapSize.y]);
			for (unsigned short j = 0, end1 = mapSize.y; j != end1; j++)
			{
				transparency[i][j] = _rVal.transparency[i][j];
			}
		}
	}
	return *this;
}

void gui::Icon::TransparencyMap::generateTransparencyMap(const sf::Texture& tex)
{
	const sf::Image transpCheck = tex.copyToImage();
	const sf::Vector2f texSize = sf::Vector2f(tex.getSize());

	transparency.reset(new std::unique_ptr<bool[]>[transpCheck.getSize().x]);
	for (unsigned short i = 0, end = texSize.x; i != end; i++)
	{
		transparency[i].reset(new bool[transpCheck.getSize().y]);
		for (unsigned short j = 0, end1 = texSize.y; j != end1; j++)
			transparency[i][j] = !transpCheck.getPixel(i, j).a;
	}
}