#include "NetSock.h"
#include <thread>
#include <vector>
#include <stdlib.h>
void recieveData(NetSock &netsock);
void sendData(NetSock &netsock);
int main(int argc ,char *argv[]){

	uint16_t Port;
	if(argc == 1){
		NetSock server(0);		
	}else if(argc == 2)
	{
		NetSock server(0,argv[1]);
	}else if (argc == 3)
		NetSock server(0,argv[1],atoi(argv[2]));
	else {
		put("Server shutdown \n");
		exit();
	}
    //std::vector <std::thread> threads;

	while(true){
	  	server.Connectuser();
		std::thread send(recieveData,server);
		std::thread recv(sendData,server)
	}
	send.join();
	recv.join();
	FinishConnection(0)
	return 0;
}
void recieveData(!NetSock &netsock){
	char data[buffLength];
		while(netsock.ReceiveData(data)){
			printf("%s\n",data);
		}
}
void sendData(NetSock &netsock){
	char data[buffLength];
	do
	{
	printf("Your mesage:\n");	
	scanf("%512s",data);
	}while(netsock.SendData(data));
}