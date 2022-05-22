#include "stdafx.h"
#include "Button.hpp"

Button::Button(
	const float& x,
	const float& y,
	const float& sizeX,
	const float& sizeY,
	const sf::Color& color,
	const sf::Color& hitColor,
	const sf::String& text,
	const sf::Color& textColor,
	const sf::Font& font
	) :
	buttonColor(color),
	hitButtonColor(hitColor),
	text(text, font, 0U),
	inBounds(false),
	clicked(false),
	button(sf::Vector2f(sizeX, sizeY)),
	disabled(false)
{
	button.setOrigin(button.getLocalBounds().width / 2.f, button.getLocalBounds().height / 2.f);
	button.setPosition(x, y);
	setTextSize(getDefaultTextSize());
	this->text.setFillColor(textColor);
}

bool Button::isPressed()
{
	if (inBounds  && clicked == true && disabled == false) { clicked = false; return true; }
	return false;
}

void Button::onEvent(const sf::Event& evn)
{
	if (evn.type == sf::Event::MouseButtonPressed) clicked = true;
	if (evn.type == sf::Event::MouseButtonReleased) clicked = false;
	const sf::Vector2i mPos = { clicked ? evn.mouseButton.x : evn.mouseMove.x, clicked ? evn.mouseButton.y : evn.mouseMove.y };
	if (mPos.x != 0 && mPos.y != 0) inBounds = mPos.x >= button.getPosition().x - button.getSize().x / 2.f && mPos.x <= button.getPosition().x + button.getSize().x / 2.f && mPos.y >= button.getPosition().y - button.getSize().y / 2.f && mPos.y <= button.getPosition().y + button.getSize().y / 2.f;
}

void Button::onRender(sf::RenderWindow& win)
{
	if (inBounds || disabled) button.setFillColor(hitButtonColor);
	else button.setFillColor(buttonColor);
	win.draw(button);
	win.draw(text);
}

void Button::setPosition(const sf::Vector2f& position)
{
	button.setPosition(position);
	text.setPosition(button.getPosition());
}

void Button::setPosition(const float& x, const float& y)
{
	button.setPosition(x, y);
	text.setPosition(button.getPosition());
}

void Button::setSize(const sf::Vector2f& size)
{
	button.setSize(size);
	button.setOrigin(button.getGlobalBounds().width / 2.f, button.getGlobalBounds().height / 2.f);
}

void Button::setSize(const float& sizeX, const float& sizeY)
{
	button.setSize(sf::Vector2f(sizeX, sizeY));
	button.setOrigin(button.getGlobalBounds().width / 2.f, button.getGlobalBounds().height / 2.f);
}

void Button::setColor(const sf::Color& color)
{
	buttonColor = color;
}

void Button::setHitColor(const sf::Color& color)
{
	hitButtonColor = color;
}

void Button::setText(const sf::String& text)
{
	this->text.setString(text);
	this->text.setOrigin(this->text.getLocalBounds().left + this->text.getLocalBounds().width / 2.f, this->text.getLocalBounds().top + this->text.getLocalBounds().height / 2.f);
	this->text.setPosition(button.getPosition());
}

void Button::setTextColor(const sf::Color& color)
{
	this->text.setFillColor(color);
}

void Button::setTextSize(const sf::Uint16& size)
{
	this->text.setCharacterSize(size);
	this->text.setOrigin(this->text.getLocalBounds().left + this->text.getLocalBounds().width / 2.f, this->text.getLocalBounds().top + this->text.getLocalBounds().height / 2.f);
	this->text.setPosition(button.getPosition());
}

void Button::setFont(const sf::Font& font)
{
	this->text.setFont(font);
}

sf::Uint16 Button::getDefaultTextSize() const
{
	return button.getSize().y < button.getSize().x / text.getString().getSize() ? sf::Uint16(button.getSize().y) : sf::Uint16(button.getSize().x / text.getString().getSize());
}