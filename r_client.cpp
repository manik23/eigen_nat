#include <fcntl.h>
#include "common_client.h"
#include "r_client.h"



int main(int argc, char **argv){

	if(argc!=2){
		cout<<"Plz Pass the server ip address as command line argument"<<endl;
		exit(1);
	}
	int fd[2];
	pipe(fd);
	pid_t pid;
	pid=fork();

	if(pid==0){		// Client Process	

		//listen on above port and ip to make p2p connection

			// Parent process to send registration packet and receive registration ACK
		// Client will send a packet at server ip address and port number 5000

		int udpRegistrationSocket;
		struct sockaddr_in udpRegistrationStruct;
		udpRegistrationSocket=socket(AF_INET,SOCK_DGRAM,0);

		udpRegistrationStruct.sin_family=AF_INET;
		udpRegistrationStruct.sin_port=htons(5000);
		udpRegistrationStruct.sin_addr.s_addr=inet_addr(argv[1]);
		

		send_registration_packet(udpRegistrationSocket,udpRegistrationStruct);		// for step 1 and 2

		wait_for_udp_conn_req(udpRegistrationSocket,udpRegistrationStruct);

		// For step 4 and 5
		struct sockaddr_in src=tcp_to_server(argv);

		close(udpRegistrationSocket);
		send_tcp_conn_to_o_client(src,i_client);
		

		char ip_address[SIZE_OF_IPV4_ADDRESS];
		char port[6];
		int intport;

		inet_ntop(AF_INET,&src.sin_addr,ip_address,sizeof(ip_address));
		intport=ntohs(src.sin_port);
		sprintf(port,"%d",intport);

		close(fd[0]);
		write(fd[1],port,sizeof(port));

		send_tcp_to_s(argv);       //step 9

	}
	else{

		char port[6];
		int tcpSocket,option;
		struct sockaddr_in tcpStruct;

		close(fd[1]);
		memset(port,0,sizeof(port));
		read(fd[0],port,sizeof(port));

		int intport;
		sscanf(port,"%d",&intport);

		cout<<"My Src port and ip"<<endl;
		cout<<"Port :"<<intport<<endl;

		tcpSocket=socket(AF_INET,SOCK_STREAM,0);
		setsockopt(tcpSocket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));


		tcpStruct.sin_family=AF_INET;
		tcpStruct.sin_port=htons(intport);
		tcpStruct.sin_addr.s_addr=htonl(INADDR_ANY);

		if(bind(tcpSocket,(struct sockaddr *)&tcpStruct,sizeof(tcpStruct))<0)
			cout<<"Bind Unsuccessfull"<<endl;
		else
			cout<<"Bind successfull on port : "<<intport<<endl;

		listen(tcpSocket,10);

		int confd=accept(tcpSocket,(struct sockaddr *)NULL,NULL);

		if(confd>0)
			cout<<"Connection Established with other client"<<endl;
		else{
			cout<<"Connection not Established with other client"<<endl;
			exit(0);
		}
		char buffer[1024];
		while(1){
			read(confd,buffer,sizeof(buffer));
			cout<<buffer<<endl;
		}
		
	}

	return 0;
}
