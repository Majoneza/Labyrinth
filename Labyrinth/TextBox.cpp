#include "stdafx.h"
#include "TextBox.hpp"

Row::Row(
	const float& x,
	const float& y,
	const sf::Font& font,
	unsigned int characterSize,
	const sf::Color& color,
	unsigned int colSpacing,
	sf::Uint32 cols
) :
	row(cols),
	colSpacing(colSpacing)
{
	for (sf::Text& text : row)
	{
		text.setCharacterSize(characterSize);
		text.setFont(font);
		text.setFillColor(color);
	}
	setPosition(x, y);
}

Row::Row(
	unsigned int colSpacing,
	sf::Uint32 cols
) :
	colSpacing(colSpacing),
	row(cols)
{
	setPosition(0, 0);
}

sf::Text& Row::operator[](size_t index)
{
	return row[index];
}

void Row::onRender(sf::RenderWindow& win)
{
	for (const sf::Text& text : row)
	{
		win.draw(text);
	}
}

void Row::setPosition(const float& x, const float& y)
{
	row[0].setPosition(x, y);
	for (size_t i = 1; i < row.size(); i++)
	{
		row[i].setPosition(row[i - 1].getPosition().x + row[i - 1].getGlobalBounds().width + colSpacing, row[i - 1].getPosition().y);
	}
}

void Row::setPosition(const sf::Vector2f& position)
{
	row[0].setPosition(position);
	for (size_t i = 1; i < row.size(); i++)
	{
		row[i].setPosition(row[i - 1].getPosition().x + row[i - 1].getGlobalBounds().width + colSpacing, row[i - 1].getPosition().y);
	}
}

void Row::setCharacterSize(const unsigned int& size)
{
	for (sf::Text& text : row)
	{
		text.setCharacterSize(size);
	}
}

void Row::setColor(const sf::Color& color)
{
	for (sf::Text& text : row)
	{
		text.setFillColor(color);
	}
}

void Row::setFont(const sf::Font& font)
{
	for (sf::Text& text : row)
	{
		text.setFont(font);
	}
}

void Row::update()
{
	setPosition(getPosition());
}

void Row::clear()
{
	for (sf::Text& text : row)
	{
		text.setString("");
	}
}

const sf::Vector2f& Row::getPosition() const
{
	return row[0].getPosition();
}

sf::FloatRect Row::getBounds() const
{
	sf::FloatRect rect = row[0].getGlobalBounds();
	for (size_t i = 1; i < row.size(); i++)
	{
		if (rect.height < row[i].getGlobalBounds().height) rect.height = row[i].getGlobalBounds().height;
		rect.width += row[i].getGlobalBounds().width + colSpacing;
	}
	return rect;
}

size_t Row::getSize() const
{
	return row.size();
}

std::vector<sf::Text>::iterator Row::begin()
{
	return row.begin();
}

std::vector<sf::Text>::const_iterator Row::begin() const
{
	return row.begin();
}

std::vector<sf::Text>::iterator Row::end()
{
	return row.end();
}

std::vector<sf::Text>::const_iterator Row::end() const
{
	return row.end();
}

/*

	TEXT BOX

*/

TextBox::TextBox(
	const float& x,
	const float& y,
	const sf::Font& font,
	unsigned int characterSize,
	const sf::Color& color,
	unsigned int lineSpacing,
	unsigned int colSpacing,
	sf::Uint32 rows,
	sf::Uint32 cols
) :
	textBox(rows, Row(colSpacing, cols)),
	lineSpacing(lineSpacing)
{
	for (Row& row : textBox)
	{
		row.setCharacterSize(characterSize);
		row.setFont(font);
		row.setColor(color);
	}
	setPosition(x, y);
}

Row& TextBox::operator[](size_t index)
{
	return textBox[index];
}

void TextBox::onRender(sf::RenderWindow& win)
{
	for (Row& row : textBox)
	{
		row.onRender(win);
	}
}

void TextBox::center(const float& window_size_x, const float& window_size_y)
{
	sf::FloatRect rect = getBounds();
	setPosition(window_size_x / 2.f - rect.width / 2.f, window_size_y / 2.f - rect.height / 2.f);
}

void TextBox::centerX(const float& window_size_x)
{
	sf::FloatRect rect = getBounds();
	setPosition(window_size_x / 2.f - rect.width / 2.f, getPosition().y);
}

void TextBox::centerY(const float& window_size_y)
{
	sf::FloatRect rect = getBounds();
	setPosition(getPosition().x, window_size_y / 2.f - rect.height / 2.f);
}

void TextBox::setPosition(const float& x, const float& y)
{
	textBox[0].setPosition(x, y);
	for (size_t i = 1; i < textBox.size(); i++)
	{
		textBox[i].setPosition(textBox[i - 1].getPosition().x, textBox[i - 1].getPosition().y + textBox[i - 1].getBounds().height + lineSpacing);
	}
}

void TextBox::setPosition(const sf::Vector2f& position)
{
	textBox[0].setPosition(position);
	for (size_t i = 1; i < textBox.size(); i++)
	{
		textBox[i].setPosition(textBox[i - 1].getPosition().x, textBox[i - 1].getPosition().y + textBox[i - 1].getBounds().height + lineSpacing);
	}
}

void TextBox::setCharacterSize(const unsigned int& size)
{
	for (Row& text : textBox)
	{
		text.setCharacterSize(size);
	}
}

void TextBox::setColor(const sf::Color& color)
{
	for (Row& text : textBox)
	{
		text.setColor(color);
	}
}

void TextBox::setFont(const sf::Font& font)
{
	for (Row& text : textBox)
	{
		text.setFont(font);
	}
}

void TextBox::update()
{
	for (Row& text : textBox)
	{
		text.update();
	}
	setPosition(getPosition());
}

void TextBox::clear()
{
	for (Row& text : textBox)
	{
		text.clear();
	}
}

const sf::Vector2f& TextBox::getPosition() const
{
	return textBox[0].getPosition();
}

sf::FloatRect TextBox::getBounds() const
{
	sf::FloatRect rect = textBox[0].getBounds();
	for (size_t i = 1; i < textBox.size(); i++)
	{
		if (rect.width < textBox[i].getBounds().width) rect.width = textBox[i].getBounds().width;
		rect.height += textBox[i].getBounds().height + lineSpacing;
	}
	return rect;
}

size_t TextBox::getSize() const
{
	return textBox.size();
}

std::vector<Row>::iterator TextBox::begin()
{
	return textBox.begin();
}

std::vector<Row>::const_iterator TextBox::begin() const
{
	return textBox.begin();
}

std::vector<Row>::iterator TextBox::end()
{
	return textBox.end();
}

std::vector<Row>::const_iterator TextBox::end() const
{
	return textBox.end();
}