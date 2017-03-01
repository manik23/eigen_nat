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

		struct client *o_client=wait_for_udp_conn_req(udpRegistrationSocket,udpRegistrationStruct);

		close(udpRegistrationSocket);
		tcp_to_server(argv);
		send_tcp_conn_to_o_client(o_client);

		close(fd[0]);
		write(fd[1],"ok",sizeof("ok"));
	}
	else{

		char buff[2];
		int tcpSocket;
		struct sockaddr_in tcpStruct;

		close(fd[1]);
		read(fd[1],buff,sizeof(buff));

		tcpSocket=socket(AF_INET,SOCK_STREAM,0);
		tcpStruct.sin_family=AF_INET;
		tcpStruct.sin_port=htons(5003);
		tcpStruct.sin_addr.s_addr=htonl(INADDR_ANY);

		bind(tcpSocket,(struct sockaddr *)&tcpStruct,sizeof(tcpStruct));

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
