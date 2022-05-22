#include "stdafx.h"
#include "LobbyState.hpp"
#include "MPlayState.hpp"
#include "GameStateManager.hpp"
#include "MenuState.hpp"
#include "Game.hpp"

LobbyState::LobbyState(GameStateManager& gsm, const sf::Uint8& client_number) :
	GameState(
		gsm
	),
	maxClients(
		client_number
	),
	lobbyText(
		"Lobby",
		game.fonts.digital,
		static_cast<sf::Uint16>(((float(config["Window"]["width"]) + float(config["Window"]["height"])) / 2.f) * 0.08f)
	),
	lobby(
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
	readyButton(
		float(config["Window"]["width"]) / 2.f,
		float(config["Window"]["height"]) * 0.8f,
		float(config["Window"]["width"]) * 0.25f,
		float(config["Window"]["height"]) * 0.065f,
		sf::Color::Color(96, 96, 96),
		sf::Color::Color(255, 229, 204),
		"  Ready  ",
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
	lobbyText.setOrigin(lobbyText.getGlobalBounds().width * 0.5f, lobbyText.getGlobalBounds().height * 0.5f);
	lobbyText.setPosition(float(config["Window"]["width"]) / 2.f, float(config["Window"]["height"]) * 0.1f);
	lobbyText.setFillColor(sf::Color::Black);

	packet << Client::RequestType::LobbyRequest << client.id;
	switch (client.udpSend(client.serverAddress, client.serverPort, packet))
	{
	case sf::Socket::Status::Error:
		printf("UDP(S) error\n");
		break;
	case sf::Socket::Status::NotReady:
		printf("UDP(S) not ready\n");
		break;
	case sf::Socket::Status::Partial:
		printf("UDP(S) partial\n");
		break;
	case sf::Socket::Status::Disconnected:
		printf("UDP(S) disconnected\n");
		client.disconnect();
		gsm.setState(new MenuState(gsm));
		break;
	case sf::Socket::Status::Done:
		break;
	}
	packet.clear();

	packet << Client::RequestType::MapRequest;
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
		gsm.setState(new MenuState(gsm));
		break;
	case sf::Socket::Status::Done:
		break;
	}
	packet.clear();
}

void LobbyState::onEvent(const sf::Event& evn)
{
	readyButton.onEvent(evn);
	disconnectButton.onEvent(evn);

	if (evn.type == sf::Event::KeyPressed)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
		{
			packet << Client::RequestType::ReadyRequest << client.id;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			packet << Client::RequestType::DisconnectRequest << client.id;
		}
	}
}

void LobbyState::onTick()
{
	packet << Client::RequestType::LobbyRequest << client.id;
	if (readyButton.isPressed())
	{
		packet << Client::RequestType::ReadyRequest << client.id;
	}
	else if (disconnectButton.isPressed())
	{
		packet << Client::RequestType::DisconnectRequest << client.id;
	}
	switch (client.udpSend(client.serverAddress, client.serverPort, packet))
	{
	case sf::Socket::Status::Error:
		printf("UDP(S) error\n");
		break;
	case sf::Socket::Status::NotReady:
		printf("UDP(S) not ready\n");
		break;
	case sf::Socket::Status::Partial:
		printf("UDP(S) partial\n");
		break;
	case sf::Socket::Status::Disconnected:
		printf("UDP(S) disconnected\n");
		client.disconnect();
		break;
	case sf::Socket::Status::Done:
		break;
	}
	switch (client.tcp.receive(packet))
	{
	case sf::Socket::Status::NotReady:
		break;
	case sf::Socket::Status::Partial:
		printf("TCP(R) partial\n");
		break;
	case sf::Socket::Status::Disconnected:
		printf("TCP(R) disconnected\n");
		client.disconnect();
		gsm.setState(new MenuState(gsm));
		break;
	case sf::Socket::Status::Error:
		printf("TCP(R) error\n");
		break;
	case sf::Socket::Status::Done:
		handleTcpReplies();
		break;
	}
	switch (client.udp.receive(packet, client.serverAddress, client.serverPort))
	{
	case sf::Socket::Status::Error:
		printf("UDP(R) error\n");
		break;
	case sf::Socket::Status::NotReady:
		break;
	case sf::Socket::Status::Partial:
		printf("UDP(R) partial\n");
		break;
	case sf::Socket::Status::Disconnected:
		printf("UDP(R) disconnected\n");
		client.disconnect();
		gsm.setState(new MenuState(gsm));
		break;
	case sf::Socket::Status::Done:
		handleUdpReplies();
		break;
	}
	packet.clear();
}

void LobbyState::onRender(sf::RenderWindow& win)
{
	win.draw(lobbyText);
	lobby.onRender(win);
	readyButton.onRender(win);
	disconnectButton.onRender(win);
}

void LobbyState::handleTcpReplies()
{
	Client::ReplyType flag;
	if (packet >> flag)
	{
		/*

			->Server Start Reply ()

		*/
		if (flag == Client::ReplyType::StartReply)
		{
			//Start game /*/*/*/*/*/*/*/*/*/*/*/*/*/*/*
			gsm.setState(new MPlayState(gsm, mapSize, maxClients));
		}
		/*

			->Server Map Reply (SIZE_X, SIZE_Y)

		*/
		else if (flag == Client::ReplyType::MapReply)
		{
			if (packet >> mapSize) printf("Map size: %u, %u\n", mapSize.x, mapSize.y);
			else printf("MapReply(LS): reading packet failed");
		}
		/*

			->Server Disconnect Reply ()

		*/
		else if (flag == Client::ReplyType::DisconnectReply)
		{
			client.disconnect();
			printf("Disconnected from server\n");
			gsm.setState(new MenuState(gsm));
			packet.clear();
		}
		/*

			->Server Shutdown Reply ()

		*/
		else if (flag == Client::ReplyType::ShutdownReply)
		{
			client.disconnect();
			printf("Server shutting down\n");
			gsm.setState(new MenuState(gsm));
			packet.clear();
		}
		/*

			->Server Exception ()

		*/
		else
		{
			printf("Unexpected flag(Lobby): %d.\n", flag);
			gsm.setState(new MenuState(gsm));
			client.disconnect();
			packet.clear();
		}
	}
	else
	{
		printf("Corrupted packet\n");
		gsm.setState(new MenuState(gsm));
		client.disconnect();
		packet.clear();
	}
	if (packet.endOfPacket() == false) handleTcpReplies();
	else packet.clear();
}

void LobbyState::handleUdpReplies()
{
	Client::ReplyType flag;
	if (packet >> flag)
	{
		/*

			->Server Lobby Reply (PLAYERS)

		*/
		if (flag == Client::ReplyType::LobbyReply)
		{
			sf::String name;
			bool ready;
			lobby.clear();
			for (sf::Uint8 connectedClients = 0; connectedClients < maxClients; connectedClients++)
			{
				if (packet >> name >> ready)
				{
					if (name != "")
					{
						lobby[connectedClients][0].setString(name);
						lobby[connectedClients][1].setString(" : ");
						if (ready)
						{
							lobby[connectedClients][2].setString("Ready");
							lobby[connectedClients][2].setFillColor(sf::Color(87, 187, 130));
						}
						else
						{
							lobby[connectedClients][2].setString("Unready");
							lobby[connectedClients][2].setFillColor(sf::Color::Red);
						}
					}
				}
				else printf("LobbyReply(LS): reading packet failed\n");
			}
			sf::FloatRect rect = lobby.getBounds();
			lobby.setPosition(float(config["Window"]["width"]) / 2.f - rect.width / 2.f, float(config["Window"]["height"]) / 3.5f - rect.height / 2.f);
		}
		/*

			->Server Ready Reply ()

		*/
		else if (flag == Client::ReplyType::ReadyReply)
		{
			bool ready;
			if (packet >> ready)
			{
				if (ready) readyButton.setText("Unready");
				else readyButton.setText("Ready");
				printf("Confirmed ready\n");
			}
			else printf("ReadyReply(LS): reading packet failed\n");
		}
		/*

			->Server Disconnect Reply ()

		*/
		else if (flag == Client::ReplyType::DisconnectReply)
		{
			client.disconnect();
			printf("Disconnected from server\n");
			gsm.setState(new MenuState(gsm));
			packet.clear();
		}
		/*

			->Server Shutdown Reply ()

		*/
		else if (flag == Client::ReplyType::ShutdownReply)
		{
			client.disconnect();
			printf("Server shutting down\n");
			gsm.setState(new MenuState(gsm));
			packet.clear();
		}
		/*

			->Server Exception ()

		*/
		else
		{
			printf("Unexpected flag(Lobby): %d.\n", flag);
			gsm.setState(new MenuState(gsm));
			client.disconnect();
			packet.clear();
		}
	}
	else
	{
		printf("Corrupted packet\n");
		gsm.setState(new MenuState(gsm));
		client.disconnect();
		packet.clear();
	}
	if (packet.endOfPacket() == false) handleUdpReplies();
	else packet.clear();
}