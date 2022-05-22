#include "stdafx.h"
#include "InputBox.hpp"

const std::string InputBox::LETTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
const std::string InputBox::NUMBERS = "0123456789";
const std::string InputBox::LETTERS_NUMBERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
const std::string InputBox::IP = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789.";
const std::string InputBox::ALL = "_<>?|+:{}\"~)!@#$%^&*(-,./\\=;[]\'`ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

InputBox::InputBox(
	const float& x, 
	const float& y, 
	const float& sX, 
	const float& sY,
	const sf::Color& textColor, 
	const sf::Color& backgroundColor, 
	const sf::Color& outlineColor, 
	const float& outlineThickness, 
	const sf::Font& font, 
	const sf::Uint8& maxlength, 
	const sf::String& restricted, 
	const sf::String& prompt, 
	const sf::String& text
	) :
	text("", font, unsigned int(sY)),
	prompt("", font, unsigned int(sY)),
	box(sf::Vector2f(sX, sY)),
	maxlength(maxlength),
	restricted(restricted),
	outlineColor(outlineColor),
	pressedOutlineColor(sf::Color(192, 192, 192)),
	inBounds(false),
	focused(false),
	clicked(false)
{
	this->text.setFillColor(textColor);
	this->prompt.setFillColor(sf::Color(224, 224, 224));
	box.setOrigin(box.getSize().x * 0.5f, box.getSize().y * 0.5f);
	box.setFillColor(backgroundColor);
	box.setOutlineColor(outlineColor);
	box.setOutlineThickness(outlineThickness);
	box.setPosition(x, y);
	this->text.setOrigin(this->text.getGlobalBounds().width * 0.5f, this->text.getGlobalBounds().height * 0.5f);
	this->text.setString(text);
	this->text.setPosition(x - box.getSize().x * 0.49f, y - sY * 0.6f);
	this->prompt.setOrigin(this->prompt.getGlobalBounds().width * 0.5f, this->prompt.getGlobalBounds().height * 0.5f);
	this->prompt.setString(prompt);
	this->prompt.setPosition(x - box.getSize().x * 0.49f, y - sY * 0.6f);
}

void InputBox::onEvent(const sf::Event& evn)
{
	if (evn.type == sf::Event::MouseButtonPressed)
	{
		if (inBounds)
		{
			focused = true;
			box.setOutlineColor(pressedOutlineColor);
		}
		else
		{
			focused = false;
			box.setOutlineColor(outlineColor);
		}
		clicked = true;
	}
	else if (evn.type == sf::Event::MouseButtonReleased)
	{
		clicked = false;
	}
	else if (evn.type == sf::Event::TextEntered && focused)
	{
		if (evn.text.unicode < 128)
		{
			std::string s = text.getString();
			const char letter = evn.text.unicode;
			if (letter == 8 && s.size() > 0) s.pop_back();
			else if (letter == 32) s += ' ';
			else if (restricted.find(letter) != std::string::npos) s += letter;
			if (s.size() > maxlength) s.erase(maxlength, s.size() - maxlength);
			text.setString(s);
		}

	}
	const sf::Vector2i mPos = { clicked ? evn.mouseButton.x : evn.mouseMove.x, clicked ? evn.mouseButton.y : evn.mouseMove.y };
	inBounds = mPos.x >= box.getPosition().x - box.getSize().x / 2.f && mPos.x <= box.getPosition().x + box.getSize().x / 2.f && mPos.y >= box.getPosition().y - box.getSize().y / 2.f && mPos.y <= box.getPosition().y + box.getSize().y / 2.f;
}

void InputBox::onRender(sf::RenderWindow& win) const
{
	if (text.getString().getSize() == 0U) win.draw(prompt);
	win.draw(text);
	win.draw(box);
}

template<typename T>
T InputBox::getInput() const
{
	return NULL;
}

template<>
std::string InputBox::getInput() const
{
	return text.getString();
}

template <>
sf::String InputBox::getInput() const
{
	return text.getString();
}

template <>
sf::Int32 InputBox::getInput() const
{
	return std::stoi(getInput<std::string>());
}

template <>
float InputBox::getInput() const
{
	return std::stof(getInput<std::string>());
}

template <>
double InputBox::getInput() const
{
	return std::stod(getInput<std::string>());
}

bool InputBox::isFocused() const
{
	return focused;
}

void InputBox::focus()
{
	focused = true;
	box.setOutlineColor(pressedOutlineColor);
}

void InputBox::unfocus()
{
	focused = false;
	box.setOutlineColor(outlineColor);
}

size_t InputBox::getInputLength() const
{
	return text.getString().getSize();
}

void InputBox::setPosition(const sf::Vector2f& position)
{
	box.setPosition(position);
	text.setPosition(position.x - box.getSize().x * 0.49f, position.y - text.getCharacterSize() * 0.5f);
	prompt.setPosition(position.x - box.getSize().x * 0.49f, position.y - text.getCharacterSize() * 0.5f);
}

void InputBox::setPosition(const float& x, const float& y)
{
	box.setPosition(x, y);
	text.setPosition(x - box.getSize().x * 0.49f, y - text.getCharacterSize() * 0.5f);
	prompt.setPosition(x - box.getSize().x * 0.49f, y - text.getCharacterSize() * 0.5f);
}

void InputBox::setSize(const sf::Vector2f& size)
{
	box.setSize(size);
	box.setOrigin(box.getSize().x * 0.5f, box.getSize().y * 0.5f);
	text.setPosition(box.getPosition().x - box.getSize().x * 0.49f, box.getPosition().y - box.getSize().y * 0.6f);
	text.setCharacterSize(unsigned int(size.y));
	prompt.setPosition(box.getPosition().x - box.getSize().x * 0.49f, box.getPosition().y - box.getSize().y * 0.6f);
	prompt.setCharacterSize(unsigned int(size.y));
}

void InputBox::setSize(const float& sizeX, const float& sizeY)
{
	box.setSize(sf::Vector2f(sizeX, sizeY));
	box.setOrigin(box.getSize().x * 0.5f, box.getSize().y * 0.5f);
	text.setPosition(box.getPosition().x - box.getSize().x * 0.49f, box.getPosition().y - box.getSize().y * 0.6f);
	text.setCharacterSize(unsigned int(sizeY));
	prompt.setPosition(box.getPosition().x - box.getSize().x * 0.49f, box.getPosition().y - box.getSize().y * 0.6f);
	prompt.setCharacterSize(unsigned int(sizeY));
}

void InputBox::setText(const sf::String& text)
{
	this->text.setString(text);
}

void InputBox::setTextColor(const sf::Color& color)
{
	text.setFillColor(color);
}

void InputBox::setTextSize(const sf::Uint16& size)
{
	this->text.setCharacterSize(size);
	this->prompt.setCharacterSize(size);
	this->text.setOrigin(this->text.getGlobalBounds().width * 0.5f, this->text.getGlobalBounds().height * 0.5f);
	this->prompt.setOrigin(this->prompt.getGlobalBounds().width * 0.5f, this->prompt.getGlobalBounds().height * 0.5f);
}

void InputBox::setBackgroundColor(const sf::Color& color)
{
	box.setFillColor(color);
}

void InputBox::setTemporaryOutlineColor(const sf::Color& color)
{
	box.setOutlineColor(color);
}

void InputBox::setOutlineColor(const sf::Color& color)
{
	outlineColor = color;
}

void InputBox::setPressedOutlineColor(const sf::Color& color)
{
	pressedOutlineColor = color;
}

void InputBox::setOutlineThickness(const float& thickness)
{
	box.setOutlineThickness(thickness);
}

void InputBox::setFont(const sf::Font& font)
{
	text.setFont(font);
	prompt.setFont(font);
}

void InputBox::setMaxLength(const sf::Uint8& maxLength)
{
	maxlength = maxLength;
}

void InputBox::setRestricted(const sf::String& restricted)
{
	this->restricted = restricted;
}

void InputBox::setPrompt(const sf::String& prompt)
{
	this->prompt.setString(prompt);
}

void InputBox::setPromptColor(const sf::Color& color)
{
	this->prompt.setFillColor(color);
}