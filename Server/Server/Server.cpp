// Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Server.h"

#include <string.h>

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;

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

	mServer.send(inHandle, &(newPlayer->GetReplicatedAttributes()), sizeof(newPlayer->GetReplicatedAttributes()), websocketpp::frame::opcode::binary);
	//mServer.send(inHandle,"this is a string", websocketpp::frame::opcode::text);
}

void Server::OnClose(connection_hdl inHandle)
{
	mPlayers.erase(inHandle);
}

void Server::OnMessageReceived(connection_hdl inHandle, WSServer::message_ptr inMessage)
{
	if (inMessage->get_opcode() == websocketpp::frame::opcode::binary)
	{
		ProcessBinaryMessage(inHandle, inMessage);
	}
	else if (inMessage->get_opcode() == websocketpp::frame::opcode::text)
	{
		ProcessTextMessage(inHandle, inMessage);
	}
	else
	{
		std::cout << "Invalid message type received from client connection " << inHandle << std::endl;
	}
}

void Server::ProcessBinaryMessage(connection_hdl inFromConnection, WSServer::message_ptr inMessage)
{
	void* byteArray = (void*)(inMessage->get_payload().data());
	int x = ((int*)byteArray)[0];
	int y = ((int*)byteArray)[1];

	for (auto player : mPlayers)
	{
		mServer.send(player.first, inMessage->get_payload(), websocketpp::frame::opcode::binary);
	}
}

void Server::ProcessTextMessage(connection_hdl inFromConnection, WSServer::message_ptr inMessage)
{

}

int main()
{
	const uint16_t port = (uint16_t)9001;
	Server server;
	server.Run(port);
    return 0;
}
