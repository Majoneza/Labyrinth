#pragma once
#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <vector>
#include <SFML\Graphics.hpp>

/*

	EACH ROW OF TEXT BOX

*/

class Row
{
public:
	Row(
		const float& x,
		const float& y,
		const sf::Font& font,
		unsigned int characterSize,
		const sf::Color& color,
		unsigned int colSpacing,
		sf::Uint32 cols
	);

	Row(
		unsigned int colSpacing,
		sf::Uint32 cols
	);

	sf::Text& operator[](size_t index);

	void onRender(sf::RenderWindow& win);

	void setPosition(const float& x, const float& y);

	void setPosition(const sf::Vector2f& position);

	void setCharacterSize(const unsigned int& size);

	void setColor(const sf::Color& color);

	void setFont(const sf::Font& font);

	void update();

	void clear();

	const sf::Vector2f& getPosition() const;

	sf::FloatRect getBounds() const;

	size_t getSize() const;

	std::vector<sf::Text>::iterator begin();
	
	std::vector<sf::Text>::const_iterator begin() const;

	std::vector<sf::Text>::iterator end();

	std::vector<sf::Text>::const_iterator end() const;
private:
	std::vector<sf::Text> row;

	unsigned int colSpacing;
};

/*

	TEXT BOX

*/

class TextBox
{
public:
	TextBox(
		const float& x,
		const float& y,
		const sf::Font& font,
		unsigned int characterSize,
		const sf::Color& color,
		unsigned int lineSpacing,
		unsigned int colSpacing,
		sf::Uint32 rows,
		sf::Uint32 cols
	);

	Row& operator[](size_t index);

	void onRender(sf::RenderWindow& win);

	void center(const float& window_size_x, const float& window_size_y);

	void centerX(const float& window_size_x);

	void centerY(const float& window_size_y);

	void setPosition(const float& x, const float& y);

	void setPosition(const sf::Vector2f& position);

	void setCharacterSize(const unsigned int& size);

	void setColor(const sf::Color& color);

	void setFont(const sf::Font& font);

	void update();

	void clear();

	const sf::Vector2f& getPosition() const;

	sf::FloatRect getBounds() const;

	size_t getSize() const;

	std::vector<Row>::iterator begin();

	std::vector<Row>::const_iterator begin() const;

	std::vector<Row>::iterator end();

	std::vector<Row>::const_iterator end() const;
private:
	std::vector<Row> textBox;

	unsigned int lineSpacing;
};

#endif