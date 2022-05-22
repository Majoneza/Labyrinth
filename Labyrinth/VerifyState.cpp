#include "stdafx.h"
#include "VerifyState.hpp"
#include "Game.hpp"
#include "JoinState.hpp"
#include "LobbyState.hpp"
#include "MenuState.hpp"

VerifyState::VerifyState(GameStateManager& gsm, const sf::String& username) :
	GameState(gsm),
	text(
		"Verifying...", 
		game.fonts.digital,
		static_cast<sf::Uint16>(((float(config["Window"]["width"]) + float(config["Window"]["height"])) * 0.5f) * 0.1f)
	),
	title(
		"Verify",
		game.fonts.digital,
		static_cast<sf::Uint16>(((float(config["Window"]["width"]) + float(config["Window"]["height"])) * 0.5f) * 0.08f)
	),
	hidePassword(
		true
	),
	username(
		username
	),
	password(
		float(config["Window"]["width"]) * 0.5f,
		float(config["Window"]["height"]) * 0.48f,
		float(config["Window"]["width"]) * 0.3f,
		float(config["Window"]["height"]) * 0.065f,
		sf::Color::Black,
		sf::Color::Transparent,
		sf::Color::Black,
		1.f,
		game.fonts.jura,
		16U,
		InputBox::ALL,
		"Password"
	),
	confirmButton(
		float(config["Window"]["width"]) / 2.f,
		float(config["Window"]["height"]) * 0.8f,
		float(config["Window"]["width"]) * 0.25f,
		float(config["Window"]["height"]) * 0.065f,
		sf::Color::Color(96, 96, 96),
		sf::Color::Color(255, 229, 204),
		"Confirm",
		sf::Color::Color(255, 253, 209),
		game.fonts.ailerons
	),
	disconnectButton(
		float(config["Window"]["width"]) / 2.f,
		float(config["Window"]["height"]) * 0.9f,
		float(config["Window"]["width"]) * 0.25f,
		float(config["Window"]["height"]) * 0.065f,
		sf::Color::Color(96, 96, 96),
		sf::Color::Color(255, 229, 204),
		"Disconnect",
		sf::Color::Color(255, 253, 209),
		game.fonts.ailerons
	)
{
	title.setFillColor(sf::Color::Black);
	title.setOrigin(title.getGlobalBounds().width * 0.5f, title.getGlobalBounds().height * 0.5f);
	title.setPosition(float(config["Window"]["width"]) * 0.5f, float(config["Window"]["height"]) * 0.1f);
	text.setFillColor(sf::Color::Black);
	text.setOrigin(text.getGlobalBounds().width * 0.5f, text.getGlobalBounds().height * 0.5f);
	text.setPosition(float(config["Window"]["width"]) * 0.5f, float(config["Window"]["height"]) * 0.3f);
	confirmButton.setTextSize(static_cast<sf::Uint16>(((float(config["Window"]["width"]) + float(config["Window"]["height"])) / 2.f) * 0.03f));
	packet.clear();
	packet << Client::RequestType::VerifyRequest << username;
	switch (client.tcp.send(packet))
	{
	case sf::Socket::Status::Error:
		printf("TCP(S) Packet send failed\n");
		client.disconnect();
		gsm.setState(new JoinState(gsm));
		break;
	case sf::Socket::Status::Disconnected:
		printf("TCP(S) Server disconnected\n");
		client.disconnect();
		gsm.setState(new JoinState(gsm));
		break;
	case sf::Socket::Status::NotReady:
		printf("TCP(S) Socket not ready\n");
		client.disconnect();
		gsm.setState(new JoinState(gsm));
		break;
	case sf::Socket::Status::Partial:
		printf("TCP(S) Partial packet send\n");
		client.disconnect();
		gsm.setState(new JoinState(gsm));
		break;
	case sf::Socket::Status::Done:
		break;
	}
	printf("Verifying\n");
}

void VerifyState::onEvent(const sf::Event& evn)
{
	if (!hidePassword)
	{
		password.onEvent(evn);
		confirmButton.onEvent(evn);
		disconnectButton.onEvent(evn);

		if (evn.type == sf::Event::KeyPressed)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
			{
				confirmPassword();
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				gsm.setState(new MenuState(gsm));
				client.disconnect();
			}
		}
	}
}

void VerifyState::onTick()
{
	switch (client.tcp.receive(packet))
	{
	case sf::Socket::Status::Error:
		printf("TCP(R) Packet received failed\n");
		client.disconnect();
		gsm.setState(new JoinState(gsm));
		break;
	case sf::Socket::Status::Disconnected:
		printf("TCP(R) Server disconnected\n");
		client.disconnect();
		gsm.setState(new JoinState(gsm));
		break;
	case sf::Socket::Status::Partial:
		printf("TCP(R) Partial packet received\n");
		client.disconnect();
		gsm.setState(new JoinState(gsm));
		break;
	case sf::Socket::Status::Done:
		handleTcpReplies();
		break;
	case sf::Socket::Status::NotReady:
		break;
	}
	if (!hidePassword)
	{
		if (confirmButton.isPressed())
		{
			confirmPassword();
		}
		else if (disconnectButton.isPressed())
		{
			gsm.setState(new MenuState(gsm));
			client.disconnect();
		}
	}
}

void VerifyState::onRender(sf::RenderWindow& win)
{
	if (hidePassword)
	{
		win.draw(text);
	}
	else
	{
		win.draw(title);
		password.onRender(win);
		confirmButton.onRender(win);
		disconnectButton.onRender(win);
	}
}

void VerifyState::handleTcpReplies()
{
	Client::ReplyType flag;
	if (packet >> flag)
	{
		if (flag == Client::ReplyType::VerifiedReply)
		{
			sf::Uint8 client_number;
			if (packet >> client.id && packet >> client.serverPort && packet >> client_number && packet >> client.name && client_number > 0)
			{
				printf("Verified\n");
				client.udp.bind(sf::Socket::AnyPort);
				gsm.setState(new LobbyState(gsm, client_number));
			}
			else
			{
				client.disconnect();
				printf("Packet corrupted\n");
				gsm.setState(new JoinState(gsm));
			}
		}
		else if (flag == Client::ReplyType::UnverifiedReply)
		{
			client.disconnect();
			printf("Server incorrect password or untrusted!\n");
			gsm.setState(new JoinState(gsm));
		}
		else if (flag == Client::ReplyType::PasswordRequiredReply)
		{
			printf("Server is password protected\n");
			hidePassword = false;
		}
		else if (flag == Client::ReplyType::DisconnectReply)
		{
			client.disconnect();
			printf("Server rejected connection\n");
			gsm.setState(new JoinState(gsm));
		}
		else if (flag == Client::ReplyType::ShutdownReply)
		{
			client.disconnect();
			printf("Server shutting down\n");
			gsm.setState(new JoinState(gsm));
		}
		else
		{
			client.disconnect();
			printf("Unexpected flag(Verify): %d.\n", flag);
			gsm.setState(new JoinState(gsm));
		}
	}
	else
	{
		client.disconnect();
		printf("Packet corrupted\n");
		gsm.setState(new JoinState(gsm));
	}
}

void VerifyState::confirmPassword()
{
	if (password.getInputLength() <= 0U) password.setTemporaryOutlineColor(sf::Color::Red);
	else
	{
		packet << Client::RequestType::VerifyRequest << username << password.getInput<sf::String>();
		switch (client.tcp.send(packet))
		{
		case sf::Socket::Status::Error:
			printf("TCP(S) Packet send failed\n");
			client.disconnect();
			gsm.setState(new JoinState(gsm));
			break;
		case sf::Socket::Status::Disconnected:
			printf("TCP(S) Server disconnected\n");
			client.disconnect();
			gsm.setState(new JoinState(gsm));
			break;
		case sf::Socket::Status::NotReady:
			printf("TCP(S) Socket not ready\n");
			client.disconnect();
			gsm.setState(new JoinState(gsm));
			break;
		case sf::Socket::Status::Partial:
			printf("TCP(S) Partial packet send\n");
			client.disconnect();
			gsm.setState(new JoinState(gsm));
			break;
		case sf::Socket::Status::Done:
			break;
		}
	}
}