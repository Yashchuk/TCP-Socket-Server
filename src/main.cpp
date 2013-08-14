/*
 * main.cpp
 *
 *  Created on: Jul 9, 2013
 *      Author: ivan
 */


#include <iostream>

using namespace std;

#include <unistd.h>


#include "ITCPserver.h"
#include "ITCPclient.h"



// call this function, when recv data from client
void informClient(ITCPClient* client, void* context)
{
	void *Data;
	int Len;
	cout << "\tDATA:" << endl;
	Data = client->getData(&Len);
	cout << (char*)Data << endl;

	client->sendData(Data, Len);
}


// call this function, when connect and disconnected client
void informServer(ITCPClient *client, ITCPServer::ServerEvents event, void *context)
{
	if(event == ITCPServer::SERVER_EVENT_CLIENT_CONNECT)
	{
		cout << "\t!!!CONNECTED!!!" << endl;
		client->setClientEventHandler(informClient, 0);
	}
	else
	{
		cout << "\t!!!DISCONNECTED!!!" << endl;
	}
}

int main()
{

	ITCPServer::serverParams params;
	params.MaxClients = 5;
	params.Port = 778;

	ITCPServer *server = ITCPServer::getInstance(params);

	server->setServerEventHandler(informServer, 0);
	server->init();

	for(int i = 0; i<100; i++, usleep(3000000))
	{
		server->loop();
	}



	ITCPServer::removeInstance(server);
	return 0;
}

