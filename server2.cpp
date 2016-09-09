#undef UNICODE

#define _WIN32_WINNT 0x501
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "31337"

DWORD __stdcall Threadrec(void *);

DWORD __stdcall Threadsend(void *);

int __cdecl main(void) 
{
    WSADATA wsaData;
    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    struct addrinfo *result = NULL;
    struct addrinfo hints;

    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;
    int i=0;
    while(i<512){recvbuf[i]='\0';i++;}
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

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // Accept a client socket
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // No longer need server socket
    //closesocket(ListenSocket);
     HANDLE h[2] = {
    //CreateThread(0,0,Threadrec,LPVOID(ClientSocket),0,0),

    CreateThread(0,0,Threadsend,LPVOID(ClientSocket),0,0)
    };

    while((iResult = recv(SOCKET(ClientSocket), recvbuf, recvbuflen, 0))!=0){
            recvbuf[strlen(recvbuf)]='\0';
            if (iResult > 0) {
                printf("Bytes received: %d\n", iResult);
                printf("Message: %s\n", recvbuf);
            }
            i=0;
            while(i<512){recvbuf[i]='\0';i++;}
            /*else if (iResult == 0)
                printf("Connection closing...\n");
            else  {
                printf("recv failed with error: %d\n", WSAGetLastError());
                closesocket(SOCKET(ClientSocket));
                WSACleanup();
                return 1;
            }
            */
           }
        
   

    WaitForSingleObject(h[0], INFINITE);
   // WaitForSingleObject(h[1], INFINITE);

    // shutdown the connection since we're done
    /*
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        return 1;
    }
    */
    // cleanup
    //closesocket(ClientSocket);
    //WSACleanup();

    return 0;
}

DWORD __stdcall Threadsend(void * ClientSocket)
{           int iSendResult;
            int iResult;
    do {
        // Echo the buffer back to the sender
            char sendbuf[20];
            scanf("%20s",sendbuf);
            iSendResult = send( SOCKET(ClientSocket),sendbuf ,strlen(sendbuf), 0 );
            Sleep(1000);
            printf("Bytes sent: %d\n", iSendResult);
            if (iSendResult == SOCKET_ERROR) {
                printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(SOCKET(ClientSocket));
                WSACleanup();
                return 1;
            }
            
        /*
        else if (iResult == 0)
            printf("Connection closing...\n");
        else  {
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(SOCKET(ClientSocket));
            WSACleanup();
            return 1;
        }*/
    } while (true);
}

DWORD __stdcall Threadrec(void * ClientSocket)
{           
            int iSendResult;
            int iResult;
            char recvbuf[20];

}