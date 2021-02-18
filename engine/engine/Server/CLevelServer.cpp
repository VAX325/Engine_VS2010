#include "Base_include.h"
#include "CLevelServer.h"

CLevelServer::CLevelServer()
{
	LF = new LevelFileServer();
}

CLevelServer::~CLevelServer()
{
}

CLevelServer::CLevelServer(char* LevelName)
{
	LF = new LevelFileServer(LevelName);
}

void CLevelServer::AddEntity(CBaseEntity* Entity)
{
	LF->AddEntity(Entity);
}

void CLevelServer::DeleteEntity(CBaseEntity* Entity)
{
	LF->DeleteEntity(Entity);
}

void CLevelServer::Close()
{

}