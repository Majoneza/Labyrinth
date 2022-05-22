#include "stdafx.h"
#include "Client.hpp"
#include "Server.hpp"
#include <random>

const sf::String Client::Unknown = "";

Client::Client() :
	ready(false),
	server(nullptr),
	running(false),
	verified(false),
	address(sf::IpAddress::None),
	port(0U),
	name(Unknown),
	id()
{
	if (udp.bind(sf::Socket::AnyPort) != sf::Socket::Done) printf("Client %u bind failed\n", id);
	tcp.setBlocking(false);
	udp.setBlocking(false);
	generateID();
}

void Client::start()
{
	stop();
	thread = std::thread(&Client::run, this);
}

void Client::stop()
{
	if (thread.joinable())
		thread.join();
}

void Client::shutdown()
{
	running = false;
	stop();
}

void Client::disconnect()
{
	address = sf::IpAddress::None;
	port = 0;
	running = false;
	ready = false;
	verified = false;
	tcp.disconnect();
	udp.bind(sf::Socket::AnyPort);
	generateID();
	sf::Lock lock(nameLock);
	printf("%s disconnected\n", name.toAnsiString().c_str());
	name = Unknown;
}

void Client::run()
{
	const std::chrono::milliseconds ms(1);
	running = true;
	while (running)
	{
		switch (tcp.receive(requestPacket))
		{
		case sf::Socket::Status::Error:
			printf("TCP(R) Packet received failed on %s, %u: %s\n", tcp.getRemoteAddress().toString().c_str(), tcp.getRemotePort(), getName().c_str());
			disconnect();
			break;
		case sf::Socket::Status::Disconnected:
			printf("TCP(R) Socket disconnected on %s, %u: %s\n", tcp.getRemoteAddress().toString().c_str(), tcp.getRemotePort(), getName().c_str());
			disconnect();
			break;
		case sf::Socket::Status::Partial:
			printf("TCP(R) Partial packet received on %s, %u: %s\n", tcp.getRemoteAddress().toString().c_str(), tcp.getRemotePort(), getName().c_str());
			break;
		case sf::Socket::Status::Done:
			handleTcpRequest();
			break;
		case sf::Socket::Status::NotReady:
			break;
		default:
			break;
		}
		switch (udp.receive(requestPacket, address, port))
		{
		case sf::Socket::Status::Error:
			printf("UDP(R) Packet received failed on %s, %u: %s\n", address.toString().c_str(), port, getName().c_str());
			break;
		case sf::Socket::Status::Disconnected:
			printf("UDP(R) Socket disconnected on %s, %u: %s\n", address.toString().c_str(), port, getName().c_str());
			break;
		case sf::Socket::Status::Partial:
			printf("UDP(R) Partial packet received on %s, %u: %s\n", address.toString().c_str(), port, getName().c_str());
			break;
		case sf::Socket::Status::Done:
			handleUdpRequest();
			break;
		case sf::Socket::Status::NotReady:
			break;
		default:
			break;
		}
		std::this_thread::sleep_for(ms);
	}
}

void Client::handleTcpRequest()
{
	Server::RequestType flag;
	if (requestPacket >> flag && flag >= 0 && flag < Server::RequestAmount)
	{
		/*
		
			->Client Verify Request(NAME, CLIENT_PASSWORD)

				- Verify client version to server version

				- Can return:
					1. Unverified Reply()
					2. Verified Reply(ID, CLIENT_NUMBER)
					3. Password Required Reply()

		*/
		if (flag == Server::RequestType::VerifyRequest)
		{
			sf::String clientName;
			sf::String clientPassword;
			if (requestPacket >> clientName && clientName.getSize() > 0U)
			{
				{
					sf::Lock lock(nameLock);
					while (server->nameExists(clientName, this)) clientName += '#';
					name = clientName;
				}
				sf::Lock lock(server->passwordLock);
				if (requestPacket >> clientPassword)
				{
					if (server->password == clientPassword)
					{
						printf("%s connected\n", std::string(name).c_str());
						verified = true;
						replyPacket << Server::ReplyType::VerifiedReply << id << udp.getLocalPort() << sf::Uint8(server->clients.size()) << name;
					}
					else
					{
						replyPacket << Server::ReplyType::UnverifiedReply;
						printf("Unverified connection: %s, %u: %s\n", tcp.getRemoteAddress().toString().c_str(), tcp.getRemotePort(), getName().c_str());
						flag = Server::RequestType::DisconnectRequest;
					}
				}
				else if (server->password.empty())
				{
					printf("%s connected\n", std::string(name).c_str());
					verified = true;
					replyPacket << Server::ReplyType::VerifiedReply << id << udp.getLocalPort() << sf::Uint8(server->clients.size()) << name;
				}
				else
				{
					replyPacket << Server::ReplyType::PasswordRequiredReply;
					printf("Password required: %s, %u: %s\n", tcp.getRemoteAddress().toString().c_str(), tcp.getRemotePort(), getName().c_str());
				}
			}
			else
			{
				replyPacket << Server::ReplyType::UnverifiedReply;
				printf("Unverified connection: %s, %u: %s\n", tcp.getRemoteAddress().toString().c_str(), tcp.getRemotePort(), getName().c_str());
				flag = Server::RequestType::DisconnectRequest;
			}
			requestPacket.clear();
		}
		/*

			->Client Result Request()

				- Get player results sorted

				- Can return:
					1. Unverified Reply()
					2. Result Reply([NAME, RESULT], ...)

		*/
		else if (flag == Server::RequestType::ResultRequest)
		{
			if (verified)
			{
				replyPacket << Server::ReplyType::ResultReply;
				std::vector<std::pair<sf::Uint16, std::string>> results;
				{
					sf::Lock lock(server->clientsLock);
					for (const Client& client : server->clients)
					{
						if (client.verified)
							results.push_back({ client.player.mapNumber, client.getName() });
						else results.push_back({ 0, "" });
					}
				}
				std::sort(results.begin(), results.end());
				std::reverse(results.begin(), results.end());
				for (const auto& pair : results)
					replyPacket << pair.second << pair.first;
			}
			else
			{
				replyPacket << Server::ReplyType::UnverifiedReply;
				printf("Unverified connection: %s, %u: %s\n", tcp.getRemoteAddress().toString().c_str(), tcp.getRemotePort(), getName().c_str());
				flag = Server::RequestType::DisconnectRequest;
			}
		}
		/*

				->Client Map Request (ID)

					-Get server map size

					-Can return:
						1. Map Reply (MAP SIZE X, MAP SIZE Y)

		*/
		else if (flag == Server::RequestType::MapRequest)
		{
			if (verified)
			{
				replyPacket << Server::ReplyType::MapReply << server->mapSize;
				printf("%s map request\n", std::string(name).c_str());
			}
			else
			{
				replyPacket << Server::ReplyType::UnverifiedReply;
				printf("Unverified connection: %s, %u: %s\n", tcp.getRemoteAddress().toString().c_str(), tcp.getRemotePort(), getName().c_str());
				flag = Server::RequestType::DisconnectRequest;
			}
		}
		/*

			->Client Exception ()

				-Exception

				-Can return:
					1. Disconnect reply ()

		*/
		else
		{
			replyPacket << Server::DisconnectReply;
			printf("Unknown flag TCP(%d): %s, %u\n", flag, tcp.getRemoteAddress().toString().c_str(), tcp.getRemotePort());
			flag = Server::RequestType::DisconnectRequest;
			requestPacket.clear();
		}
	}
	/*

		->Client Exception ()

			-Exception

			-Can return:
				1. Disconnect reply ()

	*/
	else
	{
		replyPacket << Server::DisconnectReply;
		printf("Unknown flag TCP(%d): %s, %u\n", flag, tcp.getRemoteAddress().toString().c_str(), tcp.getRemotePort());
		flag = Server::RequestType::DisconnectRequest;
		requestPacket.clear();
	}
	if (requestPacket.endOfPacket() == false) handleTcpRequest();
	else
	{
		switch (tcp.send(replyPacket))
		{
		case sf::Socket::Status::Error:
			printf("TCP(S) Packet send failed on %s, %u: %s\n", tcp.getRemoteAddress().toString().c_str(), tcp.getRemotePort(), getName().c_str());
			break;
		case sf::Socket::Status::Disconnected:
			printf("TCP(S) Socket disconnected on %s, %u: %s\n", tcp.getRemoteAddress().toString().c_str(), tcp.getRemotePort(), getName().c_str());			
			break;
		case sf::Socket::Status::NotReady:
			printf("TCP(S) Socket not ready on %s, %u: %s\n", tcp.getRemoteAddress().toString().c_str(), tcp.getRemotePort(), getName().c_str());
			break;
		case sf::Socket::Status::Partial:
			printf("TCP(S) Partial packet send on %s, %u: %s\n", tcp.getRemoteAddress().toString().c_str(), tcp.getRemotePort(), getName().c_str());
			break;
		case sf::Socket::Status::Done:
			break;
		default:
			break;
		}
		if (flag == Server::RequestType::DisconnectRequest) disconnect();
		requestPacket.clear();
		replyPacket.clear();
	}
}

void Client::handleUdpRequest()
{
	Server::RequestType flag;
	if (requestPacket >> flag && flag >= 0 && flag < Server::RequestAmount)
	{
		sf::Uint16 client_id;
		if (requestPacket >> client_id && client_id == id && verified)
		{
			/*

				->Client Lobby Request (ID)

					-Get server lobby

					-Can return:
						1. Lobby Reply (PLAYERS NAMES)

			*/
			if (flag == Server::RequestType::LobbyRequest)
			{
				replyPacket << Server::ReplyType::LobbyReply;
				sf::Lock(server->clientsLock);
				for (Client& client : server->clients)
				{
					//printf("Name(%s): %s, %d\n", std::string(name).c_str(), std::string(client.name).c_str(), client.isUdpConnected());
					if (client.verified)
					{
						replyPacket << client.name << client.ready;
					}
					else replyPacket << sf::String("") << false;
				}
			}
			/*

				->Client Ready Request (ID)

					-Client set ready

					-Can return:
						1. Ready Reply (Ready Status)

			*/
			else if (flag == Server::RequestType::ReadyRequest)
			{
				ready = !ready;
				replyPacket << Server::ReplyType::ReadyReply << ready;
				if (ready) printf("%s ready\n", std::string(name).c_str());
				else printf("%s unready\n", std::string(name).c_str());
			}
			/*

				->Tiles Request ()

					-Get server map

					-Can return:
						1. Tiles Reply (START, END, [TILES])

			*/
			else if (flag == Server::RequestType::TilesRequest)
			{
				player.timeLeft.update();
				replyPacket << Server::ReplyType::TilesReply;
				//ERROR KADA SE NAPRAVI NOVA MAPA ADRESA STARE SE POREMETI TREBA NAPRAVITI std::vector<Map*> ili std::vector<std::unique_ptr<Map*>> TAKO DA SE ADRESA NE MIJENJA
				if (player.getPosition() == player.getMap()->endPos && player.timeLeft.get() > 0.f)
				{
					player.mapNumber++;
					while (player.mapNumber >= server->maps.size()) server->createMap();
					player.timeLeft.add((server->mapSize.x + server->mapSize.y) / server->difficulty);
					sf::Lock lock(server->mapsLock);
					player.setMap(server->maps[player.mapNumber].get());
				}
				{
					//Netreba lock zato sto pristupim adresi mape koja se ne mijenja preko playera
					replyPacket << player.getMap()->startPos << player.getMap()->endPos;
					for (size_t i = 0; i < player.getMap()->size.x * player.getMap()->size.y; i++)
					{
						const Tile& tile = player.getMap()->getTile(i);
						replyPacket << tile;
					}
				}
				player.timeLeft.reset();
			}
			/*

				->Position Request (PLAYER_POSITION)

					-Get player position

					-Can return:
						1. Position Reply (PLAYER_POSITION)

			*/
			else if (flag == Server::RequestType::PositionRequest)
			{
				sf::Vector2u position;
				if (requestPacket >> position) player.setPosition(position);
				replyPacket << Server::ReplyType::PositionReply << player.getPosition();
			}
			/*

				->Players Request ()

					-Get players info

					-Can return:
						1. Players Reply (PLAYER_NAME, PLAYER_SCORE, PLAYER_ALIVE, PLAYER_POSITION)

			*/
			else if (flag == Server::RequestType::PlayersRequest)
			{
				replyPacket << Server::ReplyType::PlayersReply;
				sf::Lock lock(server->clientsLock);
				for (Client& client : server->clients)
				{
					if (client.verified)
					{
						replyPacket << client.name << client.player.mapNumber << (client.player.timeLeft.get() > 0.F);
						if (client.player.mapNumber == player.mapNumber) replyPacket << client.player.getPosition();
						else replyPacket << sf::Vector2u(0, 0);
					}
					else replyPacket << sf::String("") << 0 << false << sf::Vector2u(0, 0);
				}
			}
			/*

				->Client Time Request ()

					-Disconnect client from server

					-Can return:
						1. Time reply (TIME_LEFT)

			*/
			else if (flag == Server::RequestType::TimeRequest)
			{
				replyPacket << Server::ReplyType::TimeReply << player.timeLeft.get();
			}
			/*

				->Client Disconnect Request (ID)

					-Disconnect client from server

					-Can return:
						1. Disconnect reply ()

			*/
			else if (flag == Server::RequestType::DisconnectRequest)
			{
				replyPacket << Server::ReplyType::DisconnectReply;
				requestPacket.clear();
			}
			/*

				->Client Exception ()

					-Exception

					-Can return:
						1. Disconnect reply ()

			*/
			else
			{
				replyPacket << Server::DisconnectReply;
				printf("Unexpected flag UDP(%d): %s\n", flag, getName().c_str());
				flag = Server::RequestType::DisconnectRequest;
				requestPacket.clear();
			}
		}
		/*

			->Client Exception ()

				-Exception

				-Can return:
					1. Disconnect reply ()

		*/
		else
		{
			replyPacket << Server::DisconnectReply;
			printf("Unknown id(%u == %u)(Flag: %d): %s, %u: %s\n", id, client_id, flag, address.toString().c_str(), port, getName().c_str());
			flag = Server::RequestType::DisconnectRequest;
			requestPacket.clear();
		}
	}
	/*

		->Client Exception ()

			-Exception

			-Can return:
				1. Disconnect reply ()

	*/
	else
	{
		replyPacket << Server::DisconnectReply;
		printf("Unknown flag UDP(%d): %s, %u: %s\n", flag, address.toString().c_str(), port, getName().c_str());
		flag = Server::RequestType::DisconnectRequest;
		requestPacket.clear();
	}
	if (requestPacket.endOfPacket() == false) handleUdpRequest();
	else
	{
		switch (udpSend(address, port, replyPacket))
		{
		case sf::Socket::Status::Error:
			printf("UDP(S) Packet send failed on %s, %u: %s\n", address.toString().c_str(), port, getName().c_str());
			break;
		case sf::Socket::Status::Disconnected:
			printf("UDP(S) Socket disconnected on %s, %u: %s\n", address.toString().c_str(), port, getName().c_str());
			break;
		case sf::Socket::Status::NotReady:
			printf("UDP(S) Socket not ready on %s, %u: %s\n", address.toString().c_str(), port, getName().c_str());
			break;
		case sf::Socket::Status::Partial:
			printf("UDP(S) Partial packet send on %s, %u: %s\n", address.toString().c_str(), port, getName().c_str());
			break;
		case sf::Socket::Status::Done:
			break;
		default:
			break;
		}
		if (flag == Server::RequestType::DisconnectRequest) disconnect();
		requestPacket.clear();
		replyPacket.clear();
	}
}

bool Client::isTcpConnected() const
{
	return tcp.getLocalPort() == 0 || tcp.getRemoteAddress() == sf::IpAddress::None || tcp.getRemotePort() == 0 ? false : true;
}

sf::Socket::Status Client::udpSend(const sf::IpAddress& address, const unsigned short& port, sf::Packet& packet)
{
	if (packet.getDataSize() >= sf::UdpSocket::MaxDatagramSize) printf("%s %u: %s over datagram size\n", address.toString().c_str(), port, getName().c_str());
	return udp.send(packet, address, port);
}

void Client::generateID()
{
	std::random_device rd;
	std::mt19937 engine(rd());
	const std::uniform_int_distribution<sf::Uint16> distribution(1000, 9999);
	id = distribution(engine);
}

std::string Client::getName() const
{
	sf::Lock lock(nameLock);
	return name;
}