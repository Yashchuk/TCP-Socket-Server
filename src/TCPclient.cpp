/*
 * TCPclient.cpp
 *
 *  Created on: Jul 11, 2013
 *      Author: ivan
 */


#include "TCPclient.h"

#include <sys/socket.h>
#include <iostream>

using namespace std;

TCPClient::TCPClient(clientParams Parameters) :
		Parameters(Parameters),
		Handler(NULL),
		Buffer(NULL),
		Length(0)
{ }


void TCPClient::setClientEventHandler(DataRecivedHandler HandlerData, void *Context)
{
	Handler = HandlerData;
	this->Context = Context;
}


void TCPClient::disconnect()
{
	Parameters.ServerOwner->eraseClient(this);
}



int TCPClient::getSocket()
{
	return Parameters.Socket;
}


bool TCPClient::setData()
{
	do
	{
		Buffer = new char[MAXRECV];
		Length = recv(Parameters.Socket, Buffer, MAXRECV, 0);

		if(Length <= 0)
		{
			delete[] static_cast<char*>(Buffer);
			return Length;
		}

		if(Handler)
		{
			Handler(this, Context);
			if(Length)
				delete[] static_cast<char*>(Buffer);
			Length = 0;
		}
		else
		{
			delete[] static_cast<char*>(Buffer);
		}
	}
	while(Length == MAXRECV);

	return true;
}

int TCPClient::sendData(const void* Data, unsigned int Length)
{
	return send(Parameters.Socket, Data, Length, 0);
}


void* TCPClient::getData(int *Length)
{
	if(Length)
		*Length = this->Length;

	this->Length = 0;
	return Buffer;
}


TCPClient::~TCPClient()
{
	close(Parameters.Socket);
}
