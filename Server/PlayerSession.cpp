#include "PlayerSession.h"
#include "Packet.h"

void PlayerPacket::Write(Buffer& inBuffer)
{
	inBuffer.WriteInt(id);
	inBuffer.WriteInt(xPos);
	inBuffer.WriteInt(yPos);
}

void PlayerPacket::Read(Buffer& inBuffer)
{
	id = inBuffer.ReadInt();
	xPos = inBuffer.ReadInt();
	yPos = inBuffer.ReadInt();
}

PlayerSession::PlayerSession(connection_hdl inConnection, int inPlayerId)
	: mConnection(inConnection)
{
	attrs.id = inPlayerId;
}

connection_hdl PlayerSession::GetConnectionHandle()
{
	return mConnection;
}

PlayerPacket PlayerSession::GetReplicatedAttributes()
{
	return attrs;
}
