/*
 * server.h
 *
 *  Created on: Jul 9, 2013
 *      Author: ivan
 */

#ifndef SERVER_H_
#define SERVER_H_

//#include "TCPserver.h"

#include "client.h"


class Server
{
public:
	virtual void eraseClient(Client *client) = 0;
protected:
	virtual ~Server() {}
};

#endif /* SERVER_H_ */
