/*
 * TCPClient.h
 *
 *  Created on: 10 Jul 2013
 *      Author: gordon
 */

#ifndef TCPCLIENT_H_
#define TCPCLIENT_H_

#include "ITCPclient.h"
#include "client.h"
#include "server.h"

#include <vector>

using namespace std;


class TCPClient : public Client
{
	friend class Client;
protected:
	virtual int sendData(const void* Data, unsigned int Length);
	virtual void* getData(int *Length);
	virtual void setClientEventHandler(DataRecivedHandler HandlerData, void *Context);
	virtual void disconnect();

	virtual bool setData();
	virtual int getSocket();

	TCPClient(clientParams Parameters);
	virtual ~TCPClient();

private:
	clientParams Parameters;
	unsigned short Length;

	DataRecivedHandler Handler;
	void *Context;

	static const unsigned short MAXRECV = 1000;
	void *Buffer;
};


#endif /* TCPCLIENT_H_ */
