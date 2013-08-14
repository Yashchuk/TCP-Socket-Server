/*
 * client.cpp
 *
 *  Created on: Jul 11, 2013
 *      Author: ivan
 */

#include "client.h"
#include "TCPclient.h"

#include <iostream>

using namespace std;

Client* Client::GetInstance(clientParams Parameters)
{
	return new TCPClient(Parameters);
}

void Client::RemoveInstance(Client* client)
{
	delete client;
}
