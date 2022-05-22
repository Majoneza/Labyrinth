#include "stdafx.h"
#include "MPlayState.hpp"
#include "MenuState.hpp"
#include "Game.hpp"
#include "MGameOverState.hpp"

MPlayState::MPlayState(GameStateManager& gsm, const sf::Vector2u& map_size, const sf::Uint8& client_number) :
	GameState(
		gsm
	),
	maxClients(
		client_number
	),
	timeText(
		"Time: ?",
		game.fonts.digital,
		static_cast<sf::Uint16>(((float(config["Window"]["width"]) + float(config["Window"]["height"])) / 2.f) * 0.04f)
	),
	scores(
		0,
		0,
		game.fonts.ailerons,
		static_cast<sf::Uint16>(((float(config["Window"]["width"]) + float(config["Window"]["height"])) / 2.f) * 0.03f),
		sf::Color::Black,
		static_cast<sf::Uint16>((float(config["Window"]["height"]) / 2.f) * 0.04f),
		0,
		client_number,
		3U
	),
	map(
		map_size.x,
		map_size.y,
		config["Window"]["width"].get<float>(),
		config["Window"]["height"].get<float>()
	),
	player(
		config["Player"]["sprite"].get<std::string>(),
		map.startPos,
		map.getTileSize(),
		&map
	),
	listener(
		*this,
		true,
		{
			{ config["Player"]["up_input"], { &MPlayState::moveUp } },
			{ config["Player"]["down_input"], { &MPlayState::moveDown } },
			{ config["Player"]["left_input"], { &MPlayState::moveLeft } },
			{ config["Player"]["right_input"], { &MPlayState::moveRight } }
		}
	),
	opponents(
		client_number - 1,
		Player(
			config["Player"]["ghostSprite"].get<std::string>(), 
			map.startPos, 
			map.getTileSize(), 
			&map
		)
	)
{
	startTexture.loadFromFile(config["Tile"]["start_sprite"].get<std::string>());
	endTexture.loadFromFile(config["Tile"]["end_sprite"].get<std::string>());
	wallTexture.loadFromFile(config["Tile"]["wall_sprite"].get<std::string>());
	timeText.setFillColor(sf::Color::Red);
	timeText.setOrigin(timeText.getGlobalBounds().width / 2.f, timeText.getGlobalBounds().height / 2.f);
	timeText.setPosition(float(config["Window"]["width"]) / 2.f, float(config["Window"]["height"]) - timeText.getGlobalBounds().height);
	packet << Client::RequestType::TilesRequest << client.id;
	switch (client.udpSend(client.serverAddress, client.serverPort,packet))
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
	map.clear();
}

void MPlayState::onEvent(const sf::Event& evn)
{
	listener.onEvent(evn);
	map.onEvent(evn);
	player.onEvent(evn);
}

void MPlayState::onTick()
{
	map.onTick();
	player.onTick();
	if (player.getPosition() == map.endPos)
	{
		packet << Client::RequestType::TilesRequest << client.id;
		map.endPos = { 0, 0 };
	}
	packet << Client::RequestType::PlayersRequest << client.id << Client::RequestType::TimeRequest << client.id;
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
	switch (client.tcp.receive(packet))
	{
	case sf::Socket::Status::Error:
		printf("TCP(R) error\n");
		break;
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
	case sf::Socket::Status::Done:
		handleTcpReplies();
		break;
	}
}

void MPlayState::onRender(sf::RenderWindow& win)
{
	map.onRender(win);
	for (Player& opponent : opponents)
		opponent.onRender(win);
	player.onRender(win);
	win.draw(timeText);
	scores.onRender(win);
}

void MPlayState::handleUdpReplies()
{
	Client::ReplyType flag;
	if (packet >> flag)
	{
		/*
		
			->Server Tiles Reply (START, END, [TILES])
		
		*/
		if (flag == Client::ReplyType::TilesReply)
		{
			if (packet >> map.startPos >> map.endPos)
			{
				sf::Int32 type;
				sf::Vector2u position;
				map.clear();
				for (size_t i = 0; i < map.getSize().x * map.getSize().y; i++)
				{
					if (packet >> type >> position)
					{
						Tile& tile = map.getTile(position);
						tile.type = static_cast<Tile::Type>(type);
						tile.setPosition(position);
						//Trebao bi pohraniti teksture na pocetku igre i onda ih samo loadat a ne ovako
						if (position == map.startPos) tile.setTexture(startTexture);
						else if (position == map.endPos) tile.setTexture(endTexture);
						else if (tile.type == Tile::Type::Wall) tile.setTexture(wallTexture);
						//else if (tile.type == Tile::Type::Empty);
						else if (tile.type != Tile::Type::Empty) printf("TilesReply(MP): error unknown tile type %d\n", tile.type);
						tile.setSize(map.getTileSize());
					}
					else printf("TilesReply(MP): reading packet failed(2)\n");
				}
				player.forceSetPosition(map.startPos);
			}
			else printf("TilesReply(MP): reading packet failed(1)\n");
		}
		/*
		
			->Server Position Reply (PLAYER POSITION)
		
		*/
		else if (flag == Client::ReplyType::PositionReply)
		{
			sf::Vector2u position;
			if (packet >> position) player.setPosition(position);
			else printf("PositionReply(MP): reading packet failed\n");
		}
		/*
		
			->Server Players Reply (PLAYER_NAME, PLAYER_SCORE, PLAYER_ALIVE, PLAYER_POSITION)
		
		*/
		else if (flag == Client::ReplyType::PlayersReply)
		{
			sf::String name;
			sf::Uint16 score;
			bool alive;
			sf::Vector2u position;
			scores.clear();
			sf::Uint8 counter = 0;
			for (sf::Uint8 connectedClients = 0; connectedClients < maxClients; connectedClients++)
			{
				if (packet >> name >> score >> alive >> position)
				{
					if (name != "")
					{
						scores[connectedClients][0].setString(name);
						scores[connectedClients][1].setString(" : ");
						scores[connectedClients][2].setString(std::to_string(score));
						if (alive) scores[connectedClients].setColor(sf::Color::Black);
						else scores[connectedClients].setColor(sf::Color::Red);
						if (name != client.name)
						{
							opponents[counter].forceSetPosition(position);
							counter++;
						}
					}
				}
				else printf("PlayersReply(MP): reading packet failed %u\n", connectedClients);
			}
			scores.update();
		}
		/*
		
			->Server Time Reply (TIME LEFT)
		
		*/
		else if (flag == Client::ReplyType::TimeReply)
		{
			float timeLeft;
			if (packet >> timeLeft) timeText.setString("Time: " + std::to_string(timeLeft));
			else printf("TimeReply(MP): reading packet failed\n");
		}
		/*
		
			->Server Lobby Reply (LOBBY)
		
		*/
		else if (flag == Client::ReplyType::LobbyReply)
		{
			sf::String name;
			bool ready;
			for (sf::Uint8 connectedClients = 0; connectedClients < maxClients; connectedClients++)
			{
				if (!(packet >> name >> ready)) printf("LobbyReply(MP): reading packet failed\n");
			}
		}
		else if (flag == Client::ReplyType::ReadyReply)
		{
			bool ready;
			if (packet >> ready) printf("Leftover ready reply: Skipping...\n");
			else printf("ReadyReply(MP): reading packet failed\n");
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
		else
		{
			client.disconnect();
			printf("Unexpected flag UDP(MP): %d\n", flag);
			gsm.setState(new MenuState(gsm));
			packet.clear();
		}
	}
	else
	{
		client.disconnect();
		printf("MP: UDP reading packet failed\n");
		gsm.setState(new MenuState(gsm));
		packet.clear();
	}
	if (packet.endOfPacket() == false) handleUdpReplies();
	else packet.clear();
}

void MPlayState::handleTcpReplies()
{
	Client::ReplyType flag;
	if (packet >> flag)
	{
		/*

			->Server Game Over Reply ()

		*/
		if (flag == Client::ReplyType::GameOverReply)
		{
			printf("Game over\n");
			gsm.setState(new MGameOverState(gsm, maxClients));
		}
		else
		{
			client.disconnect();
			printf("Unexpected flag TCP(MP): %d\n", flag);
			gsm.setState(new MenuState(gsm));
			packet.clear();
		}
	}
	else
	{
		client.disconnect();
		printf("MP: TCP reading packet failed\n");
		gsm.setState(new MenuState(gsm));
		packet.clear();
	}
	if (packet.endOfPacket() == false) handleTcpReplies();
	else packet.clear();
}

void MPlayState::moveDown()
{
	player.moveDown();
	packet << Client::RequestType::PositionRequest << client.id << player.getPosition();
}

void MPlayState::moveLeft()
{
	player.moveLeft();
	packet << Client::RequestType::PositionRequest << client.id << player.getPosition();
}

void MPlayState::moveRight()
{
	player.moveRight();
	packet << Client::RequestType::PositionRequest << client.id << player.getPosition();
}

void MPlayState::moveUp()
{
	player.moveUp();
	packet << Client::RequestType::PositionRequest << client.id << player.getPosition();
}