// Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Server.h"

#include <iostream>
#include <string.h>
#include "PlayerSession.h"
#include "Packet.h"

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;

#pragma region NewSessionPacket

void NewSessionPacket::Write(Buffer& inBuffer)
{
	inBuffer.WriteByte(0);
	inBuffer.WriteInt(assignedId);
	inBuffer.WriteInt(numPlayers);
	for (auto& id : connectedPlayerIds)
	{
		inBuffer.WriteInt(id);
	}
}

void NewSessionPacket::Read(Buffer& inBuffer)
{
	//assignedId = inBuffer.ReadInt();
	//numPlayers = inBuffer.ReadInt();
	assert(false);	//Should never have to read a NewSessionPacket
}

#pragma endregion //NewSessionPacket

#pragma region PlayerJoinPacket

void PlayerJoinPacket::Write(Buffer& inBuffer)
{
	std::cout << "-------writing to join packet" << std::endl;
	inBuffer.WriteByte(1);
	inBuffer.WriteInt(playerId);
	std::cout << "-------finished writing to join packet" << std::endl;
}

void PlayerJoinPacket::Read(Buffer& inBuffer)
{
	assert(inBuffer.Index() == 1);	//First byte defines packet type
	playerId = inBuffer.ReadInt();
}

#pragma endregion //PlayerJoinPacket


Server::Server()
{
	mServer.init_asio();
	mServer.set_open_handler(bind(&Server::OnOpen, this, ::_1));
	mServer.set_close_handler(bind(&Server::OnClose, this, ::_1));
	mServer.set_message_handler(bind(&Server::OnMessageReceived, this, ::_1, ::_2));
	mServer.set_http_handler(bind(&Server::OnHTTP, this, ::_1));

	//mServer.clear_access_channels(websocketpp::log::alevel::all);
	//mServer.set_access_channels(websocketpp::log::alevel::connect);
	//mServer.set_access_channels(websocketpp::log::alevel::disconnect);
	//mServer.set_access_channels(websocketpp::log::alevel::app);
}

void Server::Run(uint16_t port)
{
	mServer.listen(port);
	mServer.start_accept();
	mServer.run();
}

void Server::OnHTTP(connection_hdl inHandle)
{
	WSServer::connection_ptr testCon = mServer.get_con_from_hdl(inHandle);

	std::stringstream output;
	output << "<!doctype html><html><body>You requested " << testCon->get_resource() << "</body></html>";

	testCon->set_status(websocketpp::http::status_code::ok);
	testCon->set_body(output.str());
}

void Server::OnOpen(connection_hdl inHandle)
{
	static uint32_t playerId = 0;

	NewSessionPacket sessionPacket;
	sessionPacket.assignedId = playerId;
	sessionPacket.numPlayers = mPlayers.size();

	Buffer joinBuffer(sizeof(uint32_t) + 1);
	PlayerJoinPacket joinPacket;
	uint32_t id = 0;
	for (auto player : mPlayers)
	{
		joinPacket.playerId = playerId;
		joinPacket.Write(joinBuffer);
		mServer.send(player.first, joinBuffer.Data(), joinBuffer.Length(), websocketpp::frame::opcode::binary);
		joinBuffer.Clear();

		sessionPacket.connectedPlayerIds.push_back((player.second)->GetReplicatedAttributes().id);
	}

	Buffer sessionBuffer(sizeof(sessionPacket.assignedId) + sizeof(sessionPacket.numPlayers) + (sessionPacket.connectedPlayerIds.size() * 8) + 1);
	sessionPacket.Write(sessionBuffer);
	mServer.send(inHandle, sessionBuffer.Data(), sessionBuffer.Length(), websocketpp::frame::opcode::binary);

	PlayerSessionPtr newPlayer(new PlayerSession(inHandle, playerId));
	mPlayers[inHandle] = newPlayer;
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
	for (auto player : mPlayers)
	{
		if (player.first.lock().get() == inFromConnection.lock().get())
		{
			//clients track their own position
			continue;
		}
		mServer.send(player.first, inMessage->get_payload(), websocketpp::frame::opcode::binary);
	}

// 	Buffer playerBuffer(inMessage->get_payload().length(), inMessage->get_payload().data());
// 	PlayerPacket playerPacket;
// 	playerBuffer.ReadByte();
// 	playerPacket.Read(playerBuffer);
}

void Server::ProcessTextMessage(connection_hdl inFromConnection, WSServer::message_ptr inMessage)
{

}

int main()
{
	const uint16_t port = (uint16_t)9001;
	Server server;
	std::cout << "Running Server on Port " << port << std::endl;
	server.Run(port);
    return 0;
}
