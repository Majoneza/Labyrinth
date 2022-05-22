#pragma once
#ifndef INPUTBOX_H
#define INPUTBOX_H

#include <SFML\Graphics.hpp>

class InputBox
{
public:
	//Restricted presets
	static const std::string LETTERS;
	static const std::string NUMBERS;
	static const std::string LETTERS_NUMBERS;
	static const std::string IP;
	static const std::string ALL;

	//InputBox constructor
	InputBox(
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
		const sf::String& prompt = "", 
		const sf::String& text = ""
		);

	//InputBox onEvent handler
	void onEvent(const sf::Event& evn);

	//InputBox onRender handler
	void onRender(sf::RenderWindow& win) const;

	//InputBox get input
	template<typename T>
	T getInput() const;

	//InputBox is focused
	bool isFocused() const;

	//InputBox focus
	void focus();

	//InputBox unfocus
	void unfocus();

	//InputBox get input length
	size_t getInputLength() const;

	//InputBox set position
	void setPosition(const sf::Vector2f& position);

	//InputBox set position
	void setPosition(const float& x, const float& y);

	//InputBox set size
	void setSize(const sf::Vector2f& size);

	//InputBox set size
	void setSize(const float& sizeX, const float& sizeY);

	//InputBox set text
	void setText(const sf::String& text);

	//InputBox set text color
	void setTextColor(const sf::Color& color);

	//InputBox set text size
	void setTextSize(const sf::Uint16& size);

	//InputBox set box background color
	void setBackgroundColor(const sf::Color& color);

	//InputBox set box outline thickness
	void setOutlineThickness(const float& thickness);

	//InputBox set text font
	void setFont(const sf::Font& font);

	//InputBox set max text length
	void setMaxLength(const sf::Uint8& maxLength);

	//InputBox set restricted letters
	void setRestricted(const sf::String& restricted);

	//InputBox set prompt text
	void setPrompt(const sf::String& prompt);

	//InputBox set prompt color
	void setPromptColor(const sf::Color& color);

	//InputBox set pressed color
	void setPressedOutlineColor(const sf::Color& color);

	//InputBox set box outline color
	void setTemporaryOutlineColor(const sf::Color& color);

	//InputBox set outline color
	void setOutlineColor(const sf::Color& color);
private:
	//InputBox shape
	sf::RectangleShape box;

	//InputBox prompt
	sf::Text prompt;

	//InputBox text
	sf::Text text;

	//InputBox clicked outline color
	sf::Color pressedOutlineColor;

	//InputBox non-pressed outline color
	sf::Color outlineColor;

	//InputBox max length
	sf::Uint8 maxlength;

	//InputBox restricted characters
	sf::String restricted;
	
	//InputBox mouse in bounds state
	bool inBounds;

	//InputBox focused state
	bool focused;

	//InputBox clicked state
	bool clicked;
};

#endif