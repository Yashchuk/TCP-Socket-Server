/*
 * ITCPclient.h
 *
 *  Created on: 10 Jul 2013
 *      Author: gordon
 */

#ifndef ITCPCLIENT_H_
#define ITCPCLIENT_H_

//client class-interface for Handlers
class ITCPClient
{
public:
	//this handler must be started if accepted data from client
	typedef void (*DataRecivedHandler)(ITCPClient* client, void* Context);

	//send data for client
	virtual int sendData(const void* Data, unsigned int Length) = 0;

	//give data from client and return length
	virtual void* getData(int *Length) = 0;

	//set new handler with context
	virtual void setClientEventHandler(DataRecivedHandler HandlerData, void *Context) = 0;

	//disconnecting client
	virtual void disconnect() = 0;

protected:
	virtual ~ITCPClient() {}
};


#endif /* ITCPCLIENT_H_ */
