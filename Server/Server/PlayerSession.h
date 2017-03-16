#pragma once

#include <websocketpp/server.hpp>

using websocketpp::connection_hdl;

class PlayerSession
{
private:
	typedef struct
	{
		int id = 0;
		int xPos = 0;
		int yPos = 0;
	} Data;

public:
	PlayerSession(connection_hdl inConnection, int inPlayerId);
	connection_hdl GetConnectionHandle();
	Data GetReplicatedAttributes();

private:
	connection_hdl mConnection;
	Data attrs;
};

typedef std::shared_ptr<PlayerSession> PlayerSessionPtr;