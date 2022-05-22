#include "stdafx.h"
#include "InputHandler.hpp"

template<typename T>
T InputHandler::toKey(const sf::String& input)
{
	return NULL;
}

template<>
sf::Keyboard::Key InputHandler::toKey(const sf::String& input)
{
	return keyInputs.at(splitString(input, '_')[1]);
}

template<>
sf::Mouse::Button InputHandler::toKey(const sf::String& input)
{
	return mouseInputs.at(splitString(input, '_')[1]);
}

bool InputHandler::isKeyboardInput(const sf::String& input)
{
	auto split = splitString(input, '_');
	return split.size() == 2 && split[0] == "K" && keyInputs.find(split[1]) != keyInputs.end();
}

bool InputHandler::isMouseInput(const sf::String& input)
{
	auto split = splitString(input, '_');
	return split.size() == 2 && split[0] == "M" && mouseInputs.find(split[1]) != mouseInputs.end();
}

bool InputHandler::hasKey(const sf::String& input)
{
	return mouseInputs.find(splitString(input, '_')[1]) != mouseInputs.end() || keyInputs.find(splitString(input, '_')[1]) != keyInputs.end();
}

bool InputHandler::isKey(const sf::String& input)
{
	return splitString(input, '_').size() == 2;
}

bool InputHandler::isPressed(const sf::String& input)
{
	if (isKey(input) && hasKey(input))
	{
		if (isKeyboardInput(input)) return sf::Keyboard::isKeyPressed(toKey<sf::Keyboard::Key>(input));
		if (isMouseInput(input)) return sf::Mouse::isButtonPressed(toKey<sf::Mouse::Button>(input));
	}
	return false;
}

bool InputHandler::isPressed(const sf::Mouse::Button& button)
{
	return sf::Mouse::isButtonPressed(button);
}

bool InputHandler::isPressed(const sf::Keyboard::Key& key)
{
	return sf::Keyboard::isKeyPressed(key);
}

bool InputHandler::isPressedOnce(const sf::String& input, const sf::Event& evn)
{
	if (isKey(input) && hasKey(input))
	{
		if (isKeyboardInput(input)) return sf::Keyboard::isKeyPressed(toKey<sf::Keyboard::Key>(input)) && evn.type == sf::Event::KeyPressed;
		if (isMouseInput(input)) return sf::Mouse::isButtonPressed(toKey<sf::Mouse::Button>(input)) && evn.type == sf::Event::MouseButtonPressed;
	}
	return false;
}

bool InputHandler::isPressedOnce(const sf::Mouse::Button& button, const sf::Event& evn)
{
	return sf::Mouse::isButtonPressed(button) && evn.type == sf::Event::MouseButtonPressed;
}

bool InputHandler::isPressedOnce(const sf::Keyboard::Key& key, const sf::Event& evn)
{
	return sf::Keyboard::isKeyPressed(key) && evn.type == sf::Event::KeyPressed;
}

std::vector<sf::String> InputHandler::splitString(const sf::String& s, const char& c)
{
	sf::String buffer("");
	std::vector<sf::String> v;
	for (const char& ch : s)
	{
		if (ch != c) buffer += ch;
		else if (ch == c && buffer != "") { v.push_back(buffer); buffer = ""; }
	}
	if (buffer != "") v.push_back(buffer);
	return v;
}