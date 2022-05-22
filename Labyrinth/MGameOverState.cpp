#include "stdafx.h"
#include "MGameOverState.hpp"
#include "Game.hpp"
#include "MenuState.hpp"

MGameOverState::MGameOverState(GameStateManager& gsm, const sf::Uint8& client_number) :
	GameState(gsm),
	maxClients(
		client_number
	),
	gameOverText(
		"Game Over",
		game.fonts.digital,
		static_cast<sf::Uint16>(((float(config["Window"]["width"]) + float(config["Window"]["height"])) / 2.f) * 0.08f)
	),
	results(
		0,
		0,
		game.fonts.ailerons,
		static_cast<sf::Uint16>(((float(config["Window"]["width"]) + float(config["Window"]["height"])) / 2.f) * 0.04f),
		sf::Color::Black,
		static_cast<sf::Uint16>((float(config["Window"]["height"]) / 2.f) * 0.04f),
		0,
		client_number,
		3U
	),
	menuButton(
		float(config["Window"]["width"]) / 2.f,
		float(config["Window"]["height"]) * 0.9f,
		float(config["Window"]["width"]) * 0.25f,
		float(config["Window"]["height"]) * 0.065f,
		sf::Color::Color(96, 96, 96),
		sf::Color::Color(255, 229, 204),
		"Menu",
		sf::Color::Color(255, 253, 209),
		game.fonts.ailerons
	)
{
	menuButton.setTextSize(static_cast<sf::Uint16>(((float(config["Window"]["width"]) + float(config["Window"]["height"])) / 2.f) * 0.035f));
	gameOverText.setOrigin(gameOverText.getGlobalBounds().width * 0.5f, gameOverText.getGlobalBounds().height * 0.5f);
	gameOverText.setPosition(float(config["Window"]["width"]) / 2.f, float(config["Window"]["height"]) * 0.1f);
	gameOverText.setFillColor(sf::Color::Black);

	packet << Client::RequestType::ResultRequest;
	switch (client.tcp.send(packet))
	{
	case sf::Socket::Status::Error:
		printf("TCP(S) error\n");
		break;
	case sf::Socket::Status::NotReady:
		printf("TCP(S) not ready\n");
		break;
	case sf::Socket::Status::Partial:
		printf("TCP(S) partial\n");
		break;
	case sf::Socket::Status::Disconnected:
		printf("TCP(S) disconnected\n");
		client.disconnect();
		break;
	case sf::Socket::Status::Done:
		break;
	}
	packet.clear();
}

void MGameOverState::onEvent(const sf::Event& evn)
{
	menuButton.onEvent(evn);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		gsm.setState(new MenuState(gsm));
		client.disconnect();
	}
}

void MGameOverState::onTick()
{
	switch (client.tcp.receive(packet))
	{
	case sf::Socket::Status::Error:
		break;
	case sf::Socket::Status::NotReady:
		break;
	case sf::Socket::Status::Partial:
		break;
	case sf::Socket::Status::Disconnected:
		break;
	case sf::Socket::Status::Done:
		handleTcpReplies();
		break;
	}
	if (menuButton.isPressed())
	{
		gsm.setState(new MenuState(gsm));
		client.disconnect();
	}
}

void MGameOverState::onRender(sf::RenderWindow& win)
{
	win.draw(gameOverText);
	menuButton.onRender(win);
	results.onRender(win);
}

void MGameOverState::handleTcpReplies()
{
	Client::ReplyType flag;
	if (packet >> flag)
	{
		/*

			->Server Result Reply ()

		*/
		if (flag == Client::ReplyType::ResultReply)
		{
			client.disconnect();
			std::string name;
			sf::Uint16 result;
			results.clear();
			for (sf::Uint8 connectedClients = 0; connectedClients < maxClients; connectedClients++)
			{
				if (packet >> name >> result)
				{
					if (name != "")
					{
						results[connectedClients][0].setString(name);
						results[connectedClients][1].setString(" : ");
						results[connectedClients][2].setString(std::to_string(result));
					}
				}
				else printf("LobbyReply(LS): reading packet failed\n");
			}
			results[0].setColor(sf::Color::Red);
			sf::FloatRect rect = results.getBounds();
			results.setPosition(float(config["Window"]["width"]) / 2.f - rect.width / 2.f, float(config["Window"]["height"]) / 3.5f - rect.height / 2.f);
		}
		/*

			->Server Disconnect Reply ()

		*/
		else if (flag == Client::ReplyType::DisconnectReply)
		{
			client.disconnect();
			printf("Disconnected from server\n");
			packet.clear();
		}
		/*

			->Server Shutdown Reply ()

		*/
		else if (flag == Client::ReplyType::ShutdownReply)
		{
			client.disconnect();
			printf("Server shutting down\n");
			packet.clear();
		}
		/*

			->Server Exception ()

		*/
		else
		{
			printf("Unexpected flag(Lobby): %d.\n", flag);
			client.disconnect();
			packet.clear();
		}
	}
	else
	{
		printf("Corrupted packet\n");
		client.disconnect();
		packet.clear();
	}
	if (packet.endOfPacket() == false) handleTcpReplies();
	else packet.clear();
}