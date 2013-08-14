/*
 * ITCPserver.cpp
 *
 *  Created on: Jul 10, 2013
 *      Author: ivan
 */


#include "ITCPserver.h"
#include "TCPserver.h"


ITCPServer* ITCPServer::getInstance(serverParams Parameters)
{
	return new TCPServer(Parameters);
}

void ITCPServer::removeInstance(ITCPServer *server)
{
	delete server;
}
