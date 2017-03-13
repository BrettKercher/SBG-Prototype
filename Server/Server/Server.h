#pragma once

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <map>
#include "PlayerSession.h"

typedef websocketpp::server<websocketpp::config::asio> WSServer;
using websocketpp::connection_hdl;

class PlayerSession;

class Server
{
public:
	Server();
	void OnOpen(connection_hdl inHandle);
	void OnClose(connection_hdl inHandle);
	void OnMessageReceived(connection_hdl inHandle, WSServer::message_ptr inMessage);
	void Run(uint16_t port);

private:
	WSServer mServer;
	std::map<connection_hdl, PlayerSessionPtr, std::owner_less<connection_hdl>> mPlayers;
};