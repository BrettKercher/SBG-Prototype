#pragma once

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <map>

typedef websocketpp::server<websocketpp::config::asio> WSServer;
using websocketpp::connection_hdl;

class PlayerSession;
class Buffer;
typedef std::shared_ptr<PlayerSession> PlayerSessionPtr;

struct NewSessionPacket
{
	uint32_t assignedId;
	uint32_t numPlayers;
	std::vector<uint32_t> connectedPlayerIds;
	void Write(Buffer& inBuffer);
	void Read(Buffer& inBuffer);
};

struct PlayerJoinPacket
{
	uint32_t playerId;
	void Write(Buffer& inBuffer);
	void Read(Buffer& inBuffer);
};

class Server
{
public:
	Server();
	void OnOpen(connection_hdl inHandle);
	void OnClose(connection_hdl inHandle);
	void OnMessageReceived(connection_hdl inHandle, WSServer::message_ptr inMessage);
	void Run(uint16_t port);

	void ProcessBinaryMessage(connection_hdl inFromConnection, WSServer::message_ptr inMessage);
	void ProcessTextMessage(connection_hdl inFromConnection, WSServer::message_ptr inMessage);

private:
	WSServer mServer;
	std::map<connection_hdl, PlayerSessionPtr, std::owner_less<connection_hdl>> mPlayers;
};