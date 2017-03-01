void send_req_for_conn2_server(char *ip_address){
	int tcpSocket,sock,nByte,count=0;
	char ip[SIZE_OF_IPV4_ADDRESS],port[6];
	int intport;
	struct sockaddr_in tcpStruct,tcpStruct1;
	tcpSocket=socket(AF_INET,SOCK_STREAM,0);

	tcpStruct.sin_family=AF_INET;
	tcpStruct.sin_port=htons(5001);
	tcpStruct.sin_addr.s_addr=inet_addr(ip_address);
	
	connect(tcpSocket,(struct sockaddr *)&tcpStruct,sizeof(tcpStruct));

	write(tcpSocket,"b8:76:3f:87:c1:a",17);
	
	nByte=read(tcpSocket,ip,sizeof(ip));
	nByte=read(tcpSocket,port,6);

	cout<<"IP Address :"<<ip<<endl<<"Port  :"<<port<<endl;

	close(tcpSocket);

	//send tcp connection request to other client
	sock=socket(AF_INET,SOCK_STREAM,0);
	sscanf(port,"%d",&intport);
	tcpStruct1.sin_family=AF_INET;
	tcpStruct1.sin_port=htons(5003);
	tcpStruct1.sin_addr.s_addr=inet_addr(ip);

	if(connect(sock,(struct sockaddr*)&tcpStruct1,sizeof(tcpStruct1))<0)
	{
		cout<<"Not able to connectto CLIENT R\n";
	}
	else
	{
		cout<<"connected to R"<<endl;
	}

	while(1){
		cout<<"Enter a message to send"<<endl;
		char buffer[1024];
		cin>>buffer;
		write(sock,buffer,sizeof(buffer));
	}

}
