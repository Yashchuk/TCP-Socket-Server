/*
 * TCPserver.cpp
 *
 *  Created on: 11 Jul 2013
 *      Author: gordon
 */

#include "TCPserver.h"

#include <iostream>
#include <algorithm>

#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <arpa/inet.h>

#include <string.h>

using namespace std;

TCPServer::TCPServer(serverParams Parameters) :Parameters(Parameters), Handler(NULL)
{ }

void TCPServer::setServerEventHandler(ServerEventHandler NewHandler, void* Context)
{
	Handler = NewHandler;
	this->Context = Context;
}


TCPServer::initEvents TCPServer::init()
{
	listenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket < 0)
	{
		return INIT_EVENT_ERROR_SOCKET;
	}

	int Result, OptVal = 1;


	Result = setsockopt(listenSocket, SOL_SOCKET,  SO_REUSEADDR, (char *)&OptVal, sizeof(OptVal));
	if (Result < 0)
	{
		close(listenSocket);
		listenSocket = -1;
		return INIT_EVENT_ERROR_SETSOCKOPT;
	}


	Result = ioctl(listenSocket, FIONBIO, (char *)&OptVal);
	if (Result < 0)
	{
		close(listenSocket);
		listenSocket = -1;
		return INIT_EVENT_ERROR_IOCTL;
	}


	sockaddr_in Addr;
	memset(&Addr, 0, sizeof(Addr));

	if(strcmp(Parameters.IP, "(null)"))
	{
		uint32_t IP;
		IP = inet_addr(Parameters.IP);
		if(IP != INADDR_NONE)
		{
			Addr.sin_addr.s_addr = IP;
		}
		else
		{
			close(listenSocket);
			listenSocket = -1;
			return INIT_EVENT_ERROR_INET_ADDR;
		}
	}
	else
		Addr.sin_addr.s_addr = INADDR_ANY;


	Addr.sin_port = Parameters.Port;
	Addr.sin_family = AF_INET;


	Result = bind(listenSocket, (sockaddr *)&Addr, sizeof(Addr));
	if (Result < 0)
	{
		close(listenSocket);
		listenSocket = -1;
		return INIT_EVENT_ERROR_BIND;
	}

	Result = listen(listenSocket, Parameters.MaxClients);
	if (Result < 0)
	{
		close(listenSocket);
		listenSocket = -1;
		return INIT_EVENT_ERROR_LISTEN;
	}

	FD_ZERO(&MasterSet);
	FD_SET(listenSocket, &MasterSet);

	MaxSD = listenSocket;

	return INIT_EVENT_SUCCESSFULL;
}


bool TCPServer::loop()
{
	fd_set WorkingSet;
	int DescReady;

	if(listenSocket == -1)
		return false;

	memcpy(&WorkingSet, &MasterSet, sizeof(MasterSet));

	timeval t = {0, 0};
	DescReady = select(MaxSD + 1, &WorkingSet, NULL, NULL, &t);

	if (DescReady < 0)
		return false;

	if (!DescReady)
		return true;

	for (int FD = 0; FD <= MaxSD  && DescReady > 0; ++FD)
	{
		if (FD_ISSET(FD, &WorkingSet))
		{
			DescReady -= 1;

            if (FD == listenSocket)
            {
               int clientSocket;

               do
               {
            	  clientSocket = accept(listenSocket, NULL, NULL);
                  if (clientSocket < 0)
                  {
                	  break;
                  }

                  Client::clientParams PClient;
                  PClient.ServerOwner = this;
                  PClient.Socket = clientSocket;
                  Client *NewClient = Client::GetInstance(PClient);
                  Pool.push_back(NewClient);

                  FD_SET(NewClient->getSocket(), &MasterSet);
                  if (NewClient->getSocket() > MaxSD)
                	  MaxSD = NewClient->getSocket();

                  if(Handler)
                  {
                	  Handler(NewClient, SERVER_EVENT_CLIENT_CONNECT, Context);
                	  if( find(Pool.begin(),Pool.end(), NewClient) == Pool.end() )
                		  Client::RemoveInstance(NewClient);
                  }

               } while (clientSocket != -1);
            }
            else
            {

            	for(vector<Client*>::iterator it = Pool.begin(); it != Pool.end(); ++it)
            	{
                   	if( ((*it)->getSocket() == FD) && !(*it)->setData() )
                   	{
                        FD_CLR((*it)->getSocket(), &MasterSet);
                        if ((*it)->getSocket() == MaxSD)
                        {
                           while (!FD_ISSET(MaxSD, &MasterSet))
                              MaxSD -= 1;
                        }
               			Pool.erase(it);

                        if(Handler)
                        {
                        	Handler(*it, SERVER_EVENT_CLIENT_DISCONNECT, Context);
                        }
                        Client::RemoveInstance(*it);
                        break;
                   	}
            	}
           	}

		}
	}

	return true;
}

void TCPServer::eraseClient(Client *client)
{
	FD_CLR(client->getSocket(), &MasterSet);
   	if (client->getSocket() == MaxSD)
   	{
   		while (!FD_ISSET(MaxSD, &MasterSet))
   			MaxSD -= 1;
   	}

   	Pool.erase(find(Pool.begin(),Pool.end(), client));

	if(Handler)
	{
    	Handler(client, SERVER_EVENT_CLIENT_DISCONNECT, Context);
 	}
}


TCPServer::~TCPServer()
{
   	for(vector<Client*>::iterator it = Pool.begin(); it != Pool.end(); it++)
   	{
   	    FD_CLR((*it)->getSocket(), &MasterSet);
        if (FD_ISSET((*it)->getSocket(), &MasterSet))
        {
   			Client::RemoveInstance(*it);
        }
   	}
   	Pool.clear();
}
