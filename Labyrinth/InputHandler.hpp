#pragma once
#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <SFML\Window.hpp>
#include <map>

namespace InputHandler
{
	static const std::map <sf::String, sf::Keyboard::Key> keyInputs
	{
	{ "A", sf::Keyboard::Key::A },
	{ "Add", sf::Keyboard::Key::Add },
	{ "B", sf::Keyboard::Key::B },
	{ "BackSlash", sf::Keyboard::Key::BackSlash },
	{ "BackSpace", sf::Keyboard::Key::BackSpace },
	{ "C", sf::Keyboard::Key::C },
	{ "Comma", sf::Keyboard::Key::Comma },
	{ "D", sf::Keyboard::Key::D },
	{ "Dash", sf::Keyboard::Key::Dash },
	{ "Delete", sf::Keyboard::Key::Delete },
	{ "Divide", sf::Keyboard::Key::Divide },
	{ "Down", sf::Keyboard::Key::Down },
	{ "E", sf::Keyboard::Key::E },
	{ "Equal", sf::Keyboard::Key::Equal },
	{ "Escape", sf::Keyboard::Key::Escape },
	{ "F", sf::Keyboard::Key::F },
	{ "F1", sf::Keyboard::Key::F1 },
	{ "F2", sf::Keyboard::Key::F2 },
	{ "F3", sf::Keyboard::Key::F3 },
	{ "F4", sf::Keyboard::Key::F4 },
	{ "F5", sf::Keyboard::Key::F5 },
	{ "F6", sf::Keyboard::Key::F6 },
	{ "F7", sf::Keyboard::Key::F7 },
	{ "F8", sf::Keyboard::Key::F8 },
	{ "F9", sf::Keyboard::Key::F9 },
	{ "F10", sf::Keyboard::Key::F10 },
	{ "F11", sf::Keyboard::Key::F11 },
	{ "F12", sf::Keyboard::Key::F12 },
	{ "F13", sf::Keyboard::Key::F13 },
	{ "F14", sf::Keyboard::Key::F14 },
	{ "F15", sf::Keyboard::Key::F15 },
	{ "G", sf::Keyboard::Key::G },
	{ "H", sf::Keyboard::Key::H },
	{ "Home", sf::Keyboard::Key::Home },
	{ "I", sf::Keyboard::Key::I },
	{ "Insert", sf::Keyboard::Key::Insert },
	{ "J", sf::Keyboard::Key::J },
	{ "K", sf::Keyboard::Key::K },
	{ "L", sf::Keyboard::Key::L },
	{ "LAlt", sf::Keyboard::Key::LAlt },
	{ "LBracket", sf::Keyboard::Key::LBracket },
	{ "LControl", sf::Keyboard::Key::LControl },
	{ "Left", sf::Keyboard::Key::Left },
	{ "LShift", sf::Keyboard::Key::LShift },
	{ "LSystem", sf::Keyboard::Key::LSystem },
	{ "M", sf::Keyboard::Key::M },
	{ "Menu", sf::Keyboard::Key::Menu },
	{ "Multiply", sf::Keyboard::Key::Multiply },
	{ "N", sf::Keyboard::Key::N },
	{ "Num0", sf::Keyboard::Key::Num0 },
	{ "Num1", sf::Keyboard::Key::Num1 },
	{ "Num2", sf::Keyboard::Key::Num2 },
	{ "Num3", sf::Keyboard::Key::Num3 },
	{ "Num4", sf::Keyboard::Key::Num4 },
	{ "Num5", sf::Keyboard::Key::Num5 },
	{ "Num6", sf::Keyboard::Key::Num6 },
	{ "Num7", sf::Keyboard::Key::Num7 },
	{ "Num8", sf::Keyboard::Key::Num8 },
	{ "Num9", sf::Keyboard::Key::Num9 },
	{ "Numpad0", sf::Keyboard::Key::Numpad0 },
	{ "Numpad1", sf::Keyboard::Key::Numpad1 },
	{ "Numpad2", sf::Keyboard::Key::Numpad2 },
	{ "Numpad3", sf::Keyboard::Key::Numpad3 },
	{ "Numpad4", sf::Keyboard::Key::Numpad4 },
	{ "Numpad5", sf::Keyboard::Key::Numpad5 },
	{ "Numpad6", sf::Keyboard::Key::Numpad6 },
	{ "Numpad7", sf::Keyboard::Key::Numpad7 },
	{ "Numpad8", sf::Keyboard::Key::Numpad8 },
	{ "Numpad9", sf::Keyboard::Key::Numpad9 },
	{ "O", sf::Keyboard::Key::O },
	{ "P", sf::Keyboard::Key::P },
	{ "PageDown", sf::Keyboard::Key::PageDown },
	{ "PageUp", sf::Keyboard::Key::PageUp },
	{ "Pause", sf::Keyboard::Key::Pause },
	{ "Period", sf::Keyboard::Key::Period },
	{ "Q", sf::Keyboard::Key::Q },
	{ "Quote", sf::Keyboard::Key::Quote },
	{ "R", sf::Keyboard::Key::R },
	{ "RAlt", sf::Keyboard::Key::RAlt },
	{ "RBracket", sf::Keyboard::Key::RBracket },
	{ "RControl", sf::Keyboard::Key::RControl },
	{ "Return", sf::Keyboard::Key::Return },
	{ "Right", sf::Keyboard::Key::Right },
	{ "RShift", sf::Keyboard::Key::RShift },
	{ "RSystem", sf::Keyboard::Key::RSystem },
	{ "S", sf::Keyboard::Key::S },
	{ "SemiColon", sf::Keyboard::Key::SemiColon },
	{ "Slash", sf::Keyboard::Key::Slash },
	{ "Space", sf::Keyboard::Key::Space },
	{ "Subtract", sf::Keyboard::Key::Subtract },
	{ "T", sf::Keyboard::Key::T },
	{ "Tab", sf::Keyboard::Key::Tab },
	{ "Tilde", sf::Keyboard::Key::Tilde },
	{ "U", sf::Keyboard::Key::U },
	{ "Unknown", sf::Keyboard::Key::Unknown },
	{ "Up", sf::Keyboard::Key::Up },
	{ "V", sf::Keyboard::Key::V },
	{ "W", sf::Keyboard::Key::W },
	{ "X", sf::Keyboard::Key::X },
	{ "Y", sf::Keyboard::Key::Y },
	{ "Z", sf::Keyboard::Key::Z }
	};

	static const std::map < sf::String, sf::Mouse::Button> mouseInputs
	{
	{ "Left", sf::Mouse::Button::Left },
	{ "Middle", sf::Mouse::Button::Middle },
	{ "Right", sf::Mouse::Button::Right },
	{ "Button1", sf::Mouse::Button::XButton1 },
	{ "Button2", sf::Mouse::Button::XButton2 }
	};

	template <typename T>
	T toKey(const sf::String& input);

	bool isKeyboardInput(const sf::String& input);

	bool isMouseInput(const sf::String& input);

	bool hasKey(const sf::String& input);

	bool isKey(const sf::String& input);

	bool isPressed(const sf::String& input);

	bool isPressed(const sf::Mouse::Button& button);

	bool isPressed(const sf::Keyboard::Key& key);

	bool isPressedOnce(const sf::String& input, const sf::Event& evn);

	bool isPressedOnce(const sf::Mouse::Button& input, const sf::Event& evn);

	bool isPressedOnce(const sf::Keyboard::Key& input, const sf::Event& evn);

	std::vector<sf::String> splitString(const sf::String& s, const char& c);
}

#endif