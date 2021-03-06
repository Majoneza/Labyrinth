// LabyrinthServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include "Server.hpp"

/*
	ARGS {
		MAP_SIZE, 2
		DIFFICULTY, 1
		MIN_CLIENT_NUMBER, 1
		MAX_CLIENT_NUMBER, 1
		PLAYER_WAIT, 1
		PORT, 1
		IP 1
	}
*/
/*
BOOL ctrl_handler(DWORD evn)
{
	if (evn == CTRL_CLOSE_EVENT)
	{
		
		return TRUE;
	}
	return FALSE;
}*/

void write()
{
	std::ofstream s;
	s.open("Start.bat", std::ofstream::out);
	s << "@ECHO OFF\nTITLE LabyrinthServer\nSET server_name=\"Server.exe\"\nECHO Make sure the server is called : Server.exe\nSET /P map_size=\"Map size: \"\nSET /P difficulty=\"Map difficulty: \"\nSET /P min_clients=\"Minimun number of clients: \"\nSET /P max_clients=\"Maximum number of clients: \"\nSET /P player_wait=\"Time to wait for players to join: \"\nSET /P port=\"Port: \"\nSET \"ip=\"\nSET /P ip=\"IP (optional): \"\n:START\nCLS\nIF EXIST %server_name% (\nIF DEFINED ip %server_name% %map_size% %map_size% %difficulty% %min_clients% %max_clients% %player_wait% %port% %ip%\nIF NOT DEFINED ip %server_name% %map_size% %map_size% %difficulty% %min_clients% %max_clients% %player_wait% %port%\n)\nIF NOT EXIST %server_name% (\nSET /P server_name=\"Enter server name(with .exe): \"\nGOTO START\n)\nPAUSE\nEXIT / B 0\n";
	s.close();
}

int main(int argc, char* argv[])
{
	if (argc == 8 || argc == 9)
	{
		printf("Weinachten Industries Corp. LTD LLC Inc.\n");
		if (argc == 8) Server(sf::Vector2u(std::stoi(argv[1]), std::stoi(argv[2])), std::stof(argv[3]), std::stoi(argv[4]), std::stoi(argv[5]), std::stoi(argv[6]), std::stoi(argv[7]));
		else Server(sf::Vector2u(std::stoi(argv[1]), std::stoi(argv[2])), std::stof(argv[3]), std::stoi(argv[4]), std::stoi(argv[5]), std::stoi(argv[6]), std::stoi(argv[7]), argv[8]);
	}
	else write();
    return EXIT_SUCCESS;
}
