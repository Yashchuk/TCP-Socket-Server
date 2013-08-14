/*
 * client.h
 *
 *  Created on: 10 Jul 2013
 *      Author: gordon
 */

#ifndef CLIENT_H_
#define CLIENT_H_

//class Server;

#include "ITCPclient.h"
//#include "server.h"

class Server;

class Client : public ITCPClient
{
public:
	//structure must be used for client constructor
	typedef struct clientParams
	{
		int Socket;
		Server *ServerOwner; // server that created client
	} clientParams;

	static Client* GetInstance(clientParams Parameters);
	static void RemoveInstance(Client *client);

	virtual bool setData() = 0;

	//gives client socket
	virtual int getSocket() = 0;

protected:
	virtual int sendData(const void* Data, unsigned int Length) = 0;
	virtual void* getData(int *Length) = 0;
	virtual void setClientEventHandler(DataRecivedHandler HandlerData, void *Context) = 0;
	virtual void disconnect() = 0;

	virtual ~Client() {}
};


#endif /* CLIENT_H_ */
