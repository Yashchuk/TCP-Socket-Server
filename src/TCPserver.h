/*
 * tcp_server.h
 *
 *  Created on: Jul 10, 2013
 *      Author: ivan
 */

#ifndef TCP_SERVER_H_
#define TCP_SERVER_H_


#include "server.h"
#include "ITCPserver.h"

#include <sys/socket.h>
#include <vector>
using namespace std;

class TCPServer : public ITCPServer, public Server
{
	friend class ITCPServer;

protected:

	typedef vector<Client*> vec_clients;

	virtual void setServerEventHandler(ServerEventHandler NewHandler, void* Context);
	virtual bool loop();
	virtual initEvents init();

	virtual void eraseClient(Client *client);

	TCPServer(serverParams Parameters);
	virtual ~TCPServer();

private:
	int listenSocket;
	int MaxSD;

	vec_clients Pool;
	serverParams Parameters;

	ServerEventHandler Handler;
	void* Context;
	ServerEvents Event;

	fd_set MasterSet;
};


#endif /* TCP_SERVER_H_ */
