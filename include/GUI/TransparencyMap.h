#ifndef GUI_TRANSPARENCY_MAP
#define GUI_TRANSPARENCY_MAP

#include <SFML/Graphics/Texture.hpp>
#include <memory>

namespace gui
{
	class TransparencyMap
	{
	public:
		TransparencyMap(const sf::Texture& texture);
		TransparencyMap(const TransparencyMap& copy);
		TransparencyMap(TransparencyMap&& temp) = default;

		const bool operator[](const sf::Vector2i& coordinates)const;

		void generateTransparencyMap(const sf::Texture& texture);

	private:
		std::unique_ptr<std::unique_ptr<bool[]>[]> m_transparency = nullptr;
		sf::Vector2i                               m_mapSize = sf::Vector2i(0, 0);
	};
}

#endif