/*
 * i_server.h
 *
 *  Created on: Jul 10, 2013
 *      Author: ivan
 */

#ifndef I_SERVER_H_
#define I_SERVER_H_

//#include "server.h"
#include "ITCPclient.h"

// class-interface for creating server
class ITCPServer
{
public:
	//enum for argument ServerEventHandler inform type event (connecting or disconnecting)
	typedef enum serverEvents
	{
		SERVER_EVENT_CLIENT_CONNECT,
		SERVER_EVENT_CLIENT_DISCONNECT
	} ServerEvents;


	typedef enum initEvents
	{
		INIT_EVENT_ERROR_SOCKET,
		INIT_EVENT_ERROR_SETSOCKOPT,
		INIT_EVENT_ERROR_IOCTL,
		INIT_EVENT_ERROR_INET_ADDR,
		INIT_EVENT_ERROR_BIND,
		INIT_EVENT_ERROR_LISTEN,
		INIT_EVENT_SUCCESSFULL
	} initErrors;

	//this handler must be started if connected or disconnected client	
	typedef void (*ServerEventHandler)(ITCPClient* Client, ServerEvents Event, void* Context);

	//structure must be used for server constructor
	typedef struct serverParams
	{
		serverParams() :
			IP("(null)"),
			Port(0),
			MaxClients(1) { }

		unsigned int MaxClients;
		unsigned short Port;
		char *IP;
	} serverParams;

	//give pointer to the new object
	static ITCPServer* getInstance(serverParams Parameters);

	//delete object
	static void removeInstance(ITCPServer *server);

	//set new handler with context
	virtual void setServerEventHandler(ServerEventHandler NewHandler, void* Context) = 0;

	virtual initErrors init() = 0;

	//must be called with periodic for check new events
	virtual bool loop() = 0;

protected:
	virtual ~ITCPServer() {}
};


#endif /* I_SERVER_H_ */
