// Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Server.h"

#include <string.h>

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;

struct Message
{
	byte num1;
	byte num2;
	int num3;
};


Server::Server()
{
	mServer.init_asio();
	mServer.set_open_handler(bind(&Server::OnOpen, this, ::_1));
	mServer.set_close_handler(bind(&Server::OnClose, this, ::_1));
	mServer.set_message_handler(bind(&Server::OnMessageReceived, this, ::_1, ::_2));
}

void Server::Run(uint16_t port)
{
	mServer.listen(port);
	mServer.start_accept();
	mServer.run();
}

void Server::OnOpen(connection_hdl inHandle)
{
	PlayerSessionPtr newPlayer(new PlayerSession(inHandle));
	mPlayers[inHandle] = newPlayer;
}

void Server::OnClose(connection_hdl inHandle)
{
	mPlayers.erase(inHandle);
}

void Server::OnMessageReceived(connection_hdl inHandle, WSServer::message_ptr inMessage)
{
	std::cout << "Received message from client" << std::endl;

	for (auto player : mPlayers)
	{
		mServer.send(player.first, &(player.second->GetReplicatedAttributes()), sizeof(player.second->GetReplicatedAttributes()), websocketpp::frame::opcode::binary);
	}
}

int main()
{
	const uint16_t port = (uint16_t)9001;
	Server server;
	server.Run(port);
    return 0;
}
