#pragma once
#include "windows.h"
#include <Winsock2.h>
#ifndef netsock
#define netsock
#include <stdint.h>
const size_t buffLength =512;
const uint16_t defaultPort= 31337;

class NetSock{
 private:
 	WSADATA wsadata;
 	int iResult;

 	SOCKET ListenSocket; //socket for listening the client
 	SOCKET ClientSocket; //the socket to keep the client info
 	SOCKET ConnectSocket;	//
 	addrinfo *result,*ptr;
 	addrinfo hints; 
 	int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen;
  public:
  	NetSock(int temp, const char* ip="127.0.0.1" ,uint16_t port=defaultPort);
  	void ConnectUser(); //server function listen+accept
  	bool SendData(char *string); // server + client 
  	bool ReceiveData(char *string);
  	bool FinishConnection(int n);
};
#endif