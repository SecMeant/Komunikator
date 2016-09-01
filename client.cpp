#include "NetSock.h"
#include <thread>
#include <vector>
#include <stdlib.h>
void receiveData(NetSock &netsock);
void sendData(NetSock &netsock);
int main(int argc ,char *argv[]){

	uint16_t Port;
	if(argc < 3 || argc >3){
		exit(1);		
	else if (argc == 3)
		NetSock client(1,argv[1],atoi(argv[2]));
    //std::vector <std::thread> threads;

	while(true){
	  	client.Connectuser();
		std::thread send(receiveData,client);
		std::thread recv(sendData,client)
	}
	send.join();
	recv.join();
	FinishConnection(1)
	return 0;
}
void receiveData(NetSock &netsock){
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