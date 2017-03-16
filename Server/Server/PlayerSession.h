#pragma once

#include <websocketpp/server.hpp>

class Buffer;

using websocketpp::connection_hdl;

struct PlayerPacket
{
	int id = 0;
	int xPos = 0;
	int yPos = 0;

	void Write(Buffer& inBuffer);
	void Read(Buffer& inBuffer);
};

class PlayerSession
{

public:
	PlayerSession(connection_hdl inConnection, int inPlayerId);
	connection_hdl GetConnectionHandle();
	PlayerPacket GetReplicatedAttributes();

private:
	connection_hdl mConnection;
	PlayerPacket attrs;
};

typedef std::shared_ptr<PlayerSession> PlayerSessionPtr;