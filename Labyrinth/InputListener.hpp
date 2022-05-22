#pragma once
#ifndef INPUTLISTENER_H
#define INPUTLISTENER_H

#include "InputHandler.hpp"
#include <array>
#include <tuple>
#include <initializer_list>

namespace helper
{
	template <int... Is>
	struct index {};

	template <int N, int... Is>
	struct gen_seq : gen_seq<N - 1, N - 1, Is...> {};

	template <int... Is>
	struct gen_seq<0, Is...> : index<Is...> {};
}

template<typename Return, typename Member, typename ... Args>
class function
{
public:
	function() : func(), args() { }

	function(Return(Member::*func)(Args...), Args&& ... args) : func(func), args(std::forward<Args>(args)...) {}

	Return call(Member& member) {
		return callMe(member, helper::gen_seq<sizeof...(Args)>{});
	}
private:
	template<int ... Is>
	Return callMe(Member& member, helper::index<Is...>) {
		return (member.*func)(std::get<Is>(args)...);
	}

	Return(Member::*func)(Args...);

	std::tuple<Args...> args;
};

template<typename Return, typename ... Args>
class function<Return, void, Args...>
{
public:
	function() : func(), args() { }

	function(Return(*func)(Args...), Args&& ... args) : func(func), args(std::forward<Args>(args)...) {}

	Return call() {
		return callMe(helper::gen_seq<sizeof...(Args)>{});
	}
private:
	template<int ... Is>
	Return callMe(helper::index<Is...>) {
		return (*func)(std::get<Is>(args)...);
	}

	Return(*func)(Args...);

	std::tuple<Args...> args;
};

template<typename Member_, size_t Size_, typename ... Args>
class InputListener
{
public:
	//InputListener constructor
	InputListener(Member_ & member, std::initializer_list<std::pair<std::string, function<void, Member_, Args...>>> arr) :
		inputs(),
		member(member),
		pressed(false),
		allowKeyHold(false)
	{
		int i = 0;
		for (auto & e : arr)
		{
			if (InputHandler::isKeyboardInput(e.first))
			{
				inputs[i].first = InputHandler::toKey<sf::Keyboard::Key>(e.first);
			}
			else if (InputHandler::isMouseInput(e.first))
			{
				inputs[i].first = InputHandler::toKey<sf::Mouse::Button>(e.first) + sf::Keyboard::KeyCount;
			}
			else
			{
				printf("Key %s doesn't exist\n", e.first.c_str());
				inputs[i].first = -1;
			}
			inputs[i].second = e.second;
			i++;
		}
	}

	//InputListener constructor
	InputListener(Member_ & member, bool block_key_hold, std::initializer_list<std::pair<std::string, function<void, Member_, Args...>>> arr) :
		inputs(),
		member(member),
		pressed(false),
		allowKeyHold(!block_key_hold)
	{
		int i = 0;
		for (auto & e : arr)
		{
			if (InputHandler::isKeyboardInput(e.first))
			{
				inputs[i].first = InputHandler::toKey<sf::Keyboard::Key>(e.first);
			}
			else if (InputHandler::isMouseInput(e.first))
			{
				inputs[i].first = InputHandler::toKey<sf::Mouse::Button>(e.first) + sf::Keyboard::KeyCount;
			}
			else
			{
				printf("Key %s doesn't exist\n", e.first.c_str());
				inputs[i].first = -1;
			}
			inputs[i].second = e.second;
			i++;
		}
	}

	//InputListener onEvent handler
	void onEvent(const sf::Event& evn)
	{
		for (size_t i = 0; i < inputs.size(); i++)
		{
			if (inputs[i].first >= sf::Keyboard::KeyCount && evn.type == sf::Event::MouseButtonPressed && !pressed
				&& InputHandler::isPressed(static_cast<sf::Mouse::Button>(inputs[i].first - sf::Keyboard::KeyCount))) inputs[i].second.call(member);
			else if (evn.type == sf::Event::KeyPressed && !pressed 
				&& InputHandler::isPressed(static_cast<sf::Keyboard::Key>(inputs[i].first))) inputs[i].second.call(member);
		}
		if ((evn.type == sf::Event::KeyReleased || evn.type == sf::Event::MouseButtonReleased) && pressed) pressed = false;
		else if ((evn.type == sf::Event::KeyPressed || evn.type == sf::Event::MouseButtonPressed) && !pressed && !allowKeyHold) pressed = true;
	}

	//InputListener block key hold
	void blockKeyHold(const bool& blockKeyHold) 
	{ 
		allowKeyHold = !blockKeyHold;
	}
private:
	//InputListener inputs
	std::array<std::pair<int, function<void, Member_, Args...>>, Size_> inputs;

	//InputListener member
	Member_& member;

	//InputListener pressed state
	bool pressed;

	//InputListener register key hold
	bool allowKeyHold;
};

template<size_t Size_, typename ... Args>
class InputListener<void, Size_, Args...>
{
public:
	//InputListener constructor
	InputListener(std::initializer_list<std::pair<std::string, function<void, void, Args...>>> arr) :
		inputs(),
		pressed(false),
		allowKeyHold(false)
	{
		int i = 0;
		for (auto & e : arr)
		{
			if (InputHandler::isKeyboardInput(e.first))
			{
				inputs[i].first = InputHandler::toKey<sf::Keyboard::Key>(e.first);
			}
			else if (InputHandler::isMouseInput(e.first))
			{
				inputs[i].first = InputHandler::toKey<sf::Mouse::Button>(e.first) + sf::Keyboard::KeyCount;
			}
			else
			{
				printf("Key %s doesn't exist\n", e.first.c_str());
				inputs[i].first = -1;
			}
			inputs[i].second = e.second;
			i++;
		}
	}

	//InputListener constructor
	InputListener(bool block_key_hold, std::initializer_list<std::pair<std::string, function<void, void, Args...>>> arr) :
		inputs(),
		pressed(false),
		allowKeyHold(!block_key_hold)
	{
		int i = 0;
		for (auto & e : arr)
		{
			if (InputHandler::isKeyboardInput(e.first))
			{
				inputs[i].first = InputHandler::toKey<sf::Keyboard::Key>(e.first);
			}
			else if (InputHandler::isMouseInput(e.first))
			{
				inputs[i].first = InputHandler::toKey<sf::Mouse::Button>(e.first) + sf::Keyboard::KeyCount;
			}
			else
			{
				printf("Key %s doesn't exist\n", e.first.c_str());
				inputs[i].first = -1;
			}
			inputs[i].second = e.second;
			i++;
		}
	}

	//InputListener onEvent handler
	void onEvent(const sf::Event& evn)
	{
		for (size_t i = 0; i < inputs.size(); i++)
		{
			if (inputs[i].first >= sf::Keyboard::KeyCount && evn.type == sf::Event::MouseButtonPressed && !pressed
				&& InputHandler::isPressed(static_cast<sf::Mouse::Button>(inputs[i].first - sf::Keyboard::KeyCount))) inputs[i].second.call();
			else if (evn.type == sf::Event::KeyPressed && !pressed 
				&& InputHandler::isPressed(static_cast<sf::Keyboard::Key>(inputs[i].first))) inputs[i].second.call();
		}
		if ((evn.type == sf::Event::KeyReleased || evn.type == sf::Event::MouseButtonReleased) && pressed) pressed = false;
		else if ((evn.type == sf::Event::KeyPressed || evn.type == sf::Event::MouseButtonPressed) && !pressed && !allowKeyHold) pressed = true;
	}

	//InputListener block key hold
	void blockKeyHold(const bool& blockKeyHold)
	{
		allowKeyHold = !blockKeyHold;
	}
private:
	//InputListener inputs
	std::array<std::pair<int, function<void, void, Args...>>, Size_> inputs;

	//InputListener pressed state
	bool pressed;

	//InputListener register key hold
	bool allowKeyHold;
};

#endif