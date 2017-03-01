struct client *i_client;
int send_registration_packet(int udpRegistrationSocket, struct sockaddr_in udpRegistrationStruct){
	char mac_address[MAC_ADDRESS_SIZE],buffer[1024];
	memset(mac_address,0,sizeof(mac_address));
	
	socklen_t len=sizeof(udpRegistrationStruct);

	char *str=get_mac_address();
	strcpy(mac_address,str);
	cout<<mac_address<<endl;
	sendto(udpRegistrationSocket,mac_address,MAC_ADDRESS_SIZE,0,(struct sockaddr *)&udpRegistrationStruct,sizeof(udpRegistrationStruct));
	memset(buffer,0,sizeof(buffer));
	recvfrom(udpRegistrationSocket,buffer,sizeof(buffer),0,(struct sockaddr *)&udpRegistrationStruct,&len);
	cout<<buffer<<endl;
}

struct client * wait_for_udp_conn_req(int udpRegistrationSocket,struct sockaddr_in udpRegistrationStruct){
	char buffer[1024];
	struct client *i_client;
	socklen_t len=sizeof(udpRegistrationStruct);
	//i_client=(struct client*)malloc(sizeof(struct client));
	memset(buffer,0,sizeof(buffer));
	recvfrom(udpRegistrationSocket,buffer,sizeof(buffer),0,(struct sockaddr *)&udpRegistrationStruct,&len);
	//sscanf(buffer,"%d",&(i_client->port));
	//memset(buffer,0,sizeof(buffer));
	//recvfrom(udpRegistrationSocket,buffer,sizeof(buffer),0,(struct sockaddr *)&udpRegistrationStruct,&len);
	//strcpy(i_client->ip_address,buffer);
	//sendto(udpRegistrationSocket,"ok",strlen("ok")-1,0,(struct sockaddr *)&udpRegistrationStruct,len);

	//cout<<"IP ADDRESS :"<<i_client->ip_address<<"\n PORT : "<<i_client->port<<endl;
}

int send_tcp_conn_to_o_client(struct sockaddr_in src,struct client *o_client){
	int tcpSocket;
	struct sockaddr_in tcpStruct;
    cout<<"Creating hole"<<endl;
	tcpSocket=socket(AF_INET,SOCK_STREAM,0);

	tcpStruct.sin_family=AF_INET;
	tcpStruct.sin_port=htons(o_client->port);
	tcpStruct.sin_addr.s_addr=inet_addr(o_client->ip_address);

	bind(tcpSocket,(struct sockaddr*)&src,sizeof(src));

	fcntl(tcpSocket, F_SETFL, O_NONBLOCK);

	connect(tcpSocket,(struct sockaddr *)&tcpStruct,sizeof(tcpStruct));
	cout<<"Hole created"<<endl;
	close(tcpSocket);
	return 1;
}

struct sockaddr_in tcp_to_server(char **argv){

	// start of stpe 5
	int tcpSocket,option=1;
	struct sockaddr_in tcpStruct,tcpStruct1,src;

    char ip[SIZE_OF_IPV4_ADDRESS],port[6];
	tcpSocket=socket(AF_INET,SOCK_STREAM,0);
	setsockopt(tcpSocket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

	// Client structure

	socklen_t l=sizeof(src);
	
	// Server structure
	tcpStruct.sin_family=AF_INET;
	tcpStruct.sin_port=htons(5002);
	tcpStruct.sin_addr.s_addr=inet_addr(argv[1]);

	cout<<argv[1]<<endl;

	if((connect(tcpSocket,(struct sockaddr *)&tcpStruct,sizeof(tcpStruct)))<0)
	{

		cout<<"failed to send from R to server"<<endl;
	}
	else{
		cout<<"successfully send from R to server (port and ip binded on 5001)"<<endl;
		}

	i_client=(struct client*)malloc(sizeof(struct client));

	read(tcpSocket,ip,sizeof(ip));
	strcpy(i_client->ip_address,ip);
	read(tcpSocket,port,sizeof(port));
	sscanf(port,"%d",&(i_client->port));

	cout<<"IP ADDRESS :"<<i_client->ip_address<<"\n PORT : "<<i_client->port<<endl;

	getsockname(tcpSocket,(struct sockaddr*)&src,&l);
	return src;
}

void send_tcp_to_s(char **argv){
	int tcpSocket;
	struct sockaddr_in tcpStruct;

	tcpSocket=socket(AF_INET,SOCK_STREAM,0);
	tcpStruct.sin_family=AF_INET;
	tcpStruct.sin_port=htons(5002);
	tcpStruct.sin_addr.s_addr=inet_addr(argv[1]);
	connect(tcpSocket,(struct sockaddr*)&tcpStruct,sizeof(tcpStruct));
}