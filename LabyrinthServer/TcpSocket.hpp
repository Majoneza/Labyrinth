#pragma once
#ifndef TCP_SOCKET_H
#define TCP_SOCKET_H

#include <SFML\Network\TcpSocket.hpp>

class TcpSocket : public sf::TcpSocket
{
public:
	void recreate() {
		close();
		create();
	}
};

#endif