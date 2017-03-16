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

	mServer.clear_access_channels(websocketpp::log::alevel::all);
	mServer.set_access_channels(websocketpp::log::alevel::connect);
	mServer.set_access_channels(websocketpp::log::alevel::disconnect);
	mServer.set_access_channels(websocketpp::log::alevel::app);
}

void Server::Run(uint16_t port)
{
	mServer.listen(port);
	mServer.start_accept();
	mServer.run();
}

void Server::OnOpen(connection_hdl inHandle)
{
	static int playerId = 0;

	mServer.send(inHandle, "current_connections:" + std::to_string(mPlayers.size()), websocketpp::frame::opcode::text);
	mServer.send(inHandle, "player_id:" + std::to_string(playerId), websocketpp::frame::opcode::text);

	PlayerSessionPtr newPlayer(new PlayerSession(inHandle, playerId));
	mPlayers[inHandle] = newPlayer;

	for (auto player : mPlayers)
	{
		if (player.first.lock().get() == inHandle.lock().get())
		{
			continue;
		}
		mServer.send(player.first, "new_connection:" + std::to_string(playerId), websocketpp::frame::opcode::text);
	}
	playerId++;
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
		std::cout << "Invalid message type received from client connection: " << inHandle.lock().get() << std::endl;
		//Kill connection?
	}
}

void Server::ProcessBinaryMessage(connection_hdl inFromConnection, WSServer::message_ptr inMessage)
{
	// Right now there's only one type of binary message - player position
	// For now just relay it to all other clients, don't worry about security

	for (auto player : mPlayers)
	{
		if (player.first.lock().get() == inFromConnection.lock().get())
		{
			//clients track their own position
			continue;
		}
		mServer.send(player.first, inMessage->get_payload(), websocketpp::frame::opcode::binary);
	}

	//void* byteArray = (void*)(inMessage->get_payload().data());
	//int x = ((int*)byteArray)[0];
	//int y = ((int*)byteArray)[1];
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
