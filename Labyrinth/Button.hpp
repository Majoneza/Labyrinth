#pragma once
#ifndef BUTTON_H
#define BUTTON_H

#include <SFML\Graphics.hpp>

class Button
{
public:
	//Button constructor
	Button(
		const float& x,
		const float& y,
		const float& sizeX,
		const float& sizeY,
		const sf::Color& color,
		const sf::Color& hitColor,
		const sf::String& text,
		const sf::Color& textColor,
		const sf::Font& font
		);

	//Return true if Button is pressed
	bool isPressed();

	//Button onEvent handler
	void onEvent(const sf::Event& evn);

	//Button onRender handler
	void onRender(sf::RenderWindow& win);

	//Button set position
	void setPosition(const sf::Vector2f& position);

	//Button set position
	void setPosition(const float& x, const float& y);
	
	//Button set size
	void setSize(const sf::Vector2f& size);

	//Button set size
	void setSize(const float& sizeX, const float& sizeY);

	//Button set color
	void setColor(const sf::Color& color);

	//Button set hit color
	void setHitColor(const sf::Color& color);

	//Button set text
	void setText(const sf::String& text);

	//Button set text color
	void setTextColor(const sf::Color& color);

	//Button set text size
	void setTextSize(const sf::Uint16& size);

	//Button set text font
	void setFont(const sf::Font& font);

	//Button get default size
	sf::Uint16 getDefaultTextSize() const;
private:
	//Button color
	sf::Color buttonColor;

	//OnClick Button color
	sf::Color hitButtonColor;

	//Button shape
	sf::RectangleShape button;

	//Button text
	sf::Text text;

	//Button disabled state
	bool disabled;

	//Is Button pressed
	bool clicked;

	//Is mouse on Button
	bool inBounds;
};

#endif