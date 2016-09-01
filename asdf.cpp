#include "NetSock.h"
#include <stdint.h>
const size_t buffLength = 512;
const uint16_t defaultPort = 31337;
//eloszka kometarz od holza

NetSock::NetSock(
		int n, // n == 0 SERVER , n == 1 CLIENT 
		const char* ip,
		uint16_t port
	 )
{
	if( n == 0 ) // SERVER
	{
		SOCKET ListenSocket = INVALID_SOCKET;
	    SOCKET ClientSocket = INVALID_SOCKET;

	    struct addrinfo *result = NULL;
	    struct addrinfo hints;

	    int iSendResult;

	    // Initialize Winsock
    	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    	if (iResult != 0) {
       		printf("WSAStartup failed with error: %d\n", iResult);
        	return 1;
    	}

	    ZeroMemory(&hints, sizeof(hints));
	    hints.ai_family = AF_INET;
	    hints.ai_socktype = SOCK_STREAM;
	    hints.ai_protocol = IPPROTO_TCP;
	    hints.ai_flags = AI_PASSIVE;

	    // Resolve the server address and port
	    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    	if ( iResult != 0 ) {
        	printf("getaddrinfo failed with error: %d\n", iResult);
        	WSACleanup();
        	return 1;
 	    }

 	    // Create a SOCKET for connecting to server
    	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    	if (ListenSocket == INVALID_SOCKET) {
        	printf("socket failed with error: %ld\n", WSAGetLastError());
        	freeaddrinfo(result);
        	WSACleanup();
        	return 1;
    	}

    	// Setup the TCP listening socket
    	iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    	if (iResult == SOCKET_ERROR) {
        	printf("bind failed with error: %d\n", WSAGetLastError());
        	freeaddrinfo(result);
        	closesocket(ListenSocket);
        	WSACleanup();
        	return 1;
    	}

    	freeaddrinfo(result);
	}

	else if( n == 1 ) // CLIENT
	{

	    SOCKET ConnectSocket = INVALID_SOCKET;
	    struct addrinfo *result = NULL,
	                    *ptr = NULL,
	                    hints;

	    int iResult;
	 
	    // Initialize Winsock
	    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	    if (iResult != 0) {
	        printf("WSAStartup failed with error: %d\n", iResult);
	        return 1;
	    }

	    ZeroMemory( &hints, sizeof(hints) );
	    hints.ai_family = AF_UNSPEC;
	    hints.ai_socktype = SOCK_STREAM;
	    hints.ai_protocol = IPPROTO_TCP;

	    // Resolve the server address and port
	    iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
	    if ( iResult != 0 ) {
	        printf("getaddrinfo failed with error: %d\n", iResult);
	        WSACleanup();
	        return 1;
	    }

	    // Attempt to connect to an address until one succeeds
	    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

	        // Create a SOCKET for connecting to server
	        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
	            ptr->ai_protocol);
	        if (ConnectSocket == INVALID_SOCKET) {
	            printf("socket failed with error: %ld\n", WSAGetLastError());
	            WSACleanup();
	            return 1;
	        }

	        // Connect to server.
	        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
	        if (iResult == SOCKET_ERROR) {
	            closesocket(ConnectSocket);
	            ConnectSocket = INVALID_SOCKET;
	            continue;
	        }
	        break;
	    }

	    freeaddrinfo(result);

	    if (ConnectSocket == INVALID_SOCKET) {
	        printf("Unable to connect to server!\n");
	        WSACleanup();
	        return 1;
	    }

	    // Send an initial buffer
	    iResult = send( ConnectSocket, sendbuf, (int)strlen(sendbuf), 0 );
	    if (iResult == SOCKET_ERROR) {
	        printf("send failed with error: %d\n", WSAGetLastError());
	        closesocket(ConnectSocket);
	        WSACleanup();
	        return 1;
	    	}
		}
	}

	NetSock::ConnectUser()
	{
		iResult = listen(ListenSocket, SOMAXCONN);
	    if (iResult == SOCKET_ERROR) {
	        printf("listen failed with error: %d\n", WSAGetLastError());
	        closesocket(ListenSocket);
	        WSACleanup();
	        return 1;
	    }

	    ClientSocket = accept(ListenSocket, NULL, NULL);
	    if (ClientSocket == INVALID_SOCKET) {
	        printf("accept failed with error: %d\n", WSAGetLastError());
	        closesocket(ListenSocket);
	        WSACleanup();
	        return 1;
	    }
	    closesocket(ListenSocket);
}

	bool NetSock::SendData(char *string)
	{

		iResult = send( ConnectSocket, string, (int)strlen(string), 0 );
	    if (iResult == SOCKET_ERROR) {
	        printf("send failed with error: %d\n", WSAGetLastError());
	        closesocket(ConnectSocket);
	        WSACleanup();
	        return 1;
	    }

}

bool NetSock::ReceiveData(char *string)
{

	int recvbuflen = recvBufferSize; 
        iResult = recv(ConnectSocket, string, buffLength, 0);
        if ( iResult > 0 ){
            printf("Bytes received: %d\n", iResult);
            return 0
        }
        else if ( iResult == 0 ){
            printf("Connection closed\n");
      		return 1;
        }
        else
            printf("recv failed with error: %d\n", WSAGetLastError());
        return 1;
}
void FinishConnection(int n)
{
	if( n == 0 )
	{
		closesocket(ClientSocket);
    	WSACleanup();
		return 0;
	}
	else if( n == 1 )
	{
		closesocket(ConnectSocket);
    	WSACleanup();
    	return 0;
	}
	//to moj koncowy komentarz
}