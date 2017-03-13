#include "PlayerSession.h"



PlayerSession::PlayerSession(connection_hdl inConnection)
	: mConnection(inConnection)
{
}

connection_hdl PlayerSession::GetConnectionHandle()
{
	return mConnection;
}

PlayerSession::Data PlayerSession::GetReplicatedAttributes()
{
	return attrs;
}
