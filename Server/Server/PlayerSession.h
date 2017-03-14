#pragma once

#include <websocketpp/server.hpp>

using websocketpp::connection_hdl;

class PlayerSession
{
private:
	typedef struct
	{
		int xPos = 7;
		int yPos = 1;
	} Data;

public:
	PlayerSession(connection_hdl inConnection);
	connection_hdl GetConnectionHandle();
	Data GetReplicatedAttributes();

private:
	connection_hdl mConnection;
	Data attrs;
};

typedef std::shared_ptr<PlayerSession> PlayerSessionPtr;