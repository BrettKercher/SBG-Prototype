#include "PlayerSession.h"



PlayerSession::PlayerSession(connection_hdl inConnection, int inPlayerId)
	: mConnection(inConnection)
{
	attrs.id = inPlayerId;
}

connection_hdl PlayerSession::GetConnectionHandle()
{
	return mConnection;
}

PlayerSession::Data PlayerSession::GetReplicatedAttributes()
{
	return attrs;
}
