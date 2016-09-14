//#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x501
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
//#include <stdint.h>

void customPrint(int fp);


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

DWORD __stdcall ThreadRec(void*);
DWORD __stdcall ThreadSend(void*);

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "31337"

int __cdecl main(int argc, char **argv) 
{
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;
    system("cls");
    char sendbuf[DEFAULT_BUFLEN];
    char recvbuf[DEFAULT_BUFLEN];
    int hghg =0;
    while(hghg<=DEFAULT_BUFLEN){recvbuf[hghg] = '\0';hghg++;}
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;
    // Validate the parameters
    if (argc != 2) {
        printf("usage: %s server-name\n", argv[0]);
        return 1;
    }

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
    //int iResult;
    //char recvbuf[DEFAULT_BUFLEN];
    
     HANDLE h[2]={
    //CreateThread(0,0,ThreadRec,LPVOID(ConnectSocket),0,0),
    CreateThread(0,0,ThreadSend,LPVOID(ConnectSocket),0,0)
    };
    // int k=0;
    COORD BASICSET={0,0};
     do {
     	HANDLE con=GetStdHandle(STD_OUTPUT_HANDLE);
     	// COORD coord={50,k};
        iResult = recv(SOCKET(ConnectSocket), recvbuf, DEFAULT_BUFLEN, 0);

        if ( iResult > 0 )
            recvbuf[strlen(recvbuf)] = '\0';
        	// SetConsoleCursorPosition(con,coord);
        	customPrint(50);
            printf("From Server: %s", recvbuf);
            hghg =0;
            while(hghg<=DEFAULT_BUFLEN){recvbuf[hghg] = '\0';hghg++;}   
            //printf("Blok");
        if(iResult == -1){ printf("Polaczenie przerwane.");exit(3);}
        SetConsoleCursorPosition(con,BASICSET);
        // k++;
    } while( true );
    WaitForSingleObject(h[0],INFINITE);
    //WaitForSingleObject(h[1],INFINITE);
    while (TRUE);

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }
        printf("Blok");

    // Send an initial buffer


    // shutdown the connection since no more data will be sent
    getchar();

    return 0;
}

DWORD __stdcall ThreadRec(void*ConnectSocket){
    int iResult;
    char recvbuf[DEFAULT_BUFLEN];
       /* iResult = shutdown(SOCKET(ConnectSocket), SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(SOCKET(ConnectSocket));
        WSACleanup();
        return 1;
    }
*/
    // Receive until the peer closes the connection
   
       // void *ThreadArgs[2]={LPVOID(ConnectSocket),LPVOID(iResult)};
    // cleanup
    closesocket(SOCKET(ConnectSocket));
    WSACleanup();


}
DWORD __stdcall ThreadSend(void* ConnectSocket){
    int iResult;
    char sendbuf[DEFAULT_BUFLEN];
      
      do {
       HANDLE con=GetStdHandle(STD_OUTPUT_HANDLE);
       COORD coord={0,0};
       DWORD cCharsWritten;
       FillConsoleOutputCharacter(con,0,40,coord,&cCharsWritten);
        SetConsoleCursorPosition(con,coord);

       std::cin.getline(sendbuf,DEFAULT_BUFLEN);

    iResult = send( SOCKET(ConnectSocket), sendbuf, strlen(sendbuf), 0 );
    customPrint(50);
    printf("To Server: %s", sendbuf);
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(SOCKET(ConnectSocket));
        WSACleanup();
        return 1;
    }
    } while( true );
    
}

void customPrint(int fp)
{
	static int k = 0;
	HANDLE con=GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord={fp,k};
    SetConsoleCursorPosition(con,coord);
    k++;
}
