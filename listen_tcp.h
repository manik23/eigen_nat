int listen_for_tcp_conn_req(int udpSocket){

        int tcpSocket,tcpSocket1,nBytes;
        char mac_address[MAC_ADDRESS_SIZE];
        struct sockaddr_in tcpStruct,tcpStruct1,clientStruct,client1;
        tcpSocket=socket(AF_INET,SOCK_STREAM,0);

        tcpStruct.sin_family=AF_INET;
        tcpStruct.sin_port=htons(5001);
        tcpStruct.sin_addr.s_addr=htonl(INADDR_ANY);

        if(bind(tcpSocket,(struct sockaddr *)&tcpStruct,sizeof(tcpStruct))<0){
                cout<<"Bind unsuccessfull"<<endl;
                exit(1);
        }
        else
                cout<<"Bind successfull"<<endl;

        // Definition of binding the socket on port 5002

        tcpSocket1=socket(AF_INET,SOCK_STREAM,0);

        tcpStruct1.sin_family=AF_INET;
        tcpStruct1.sin_port=htons(5002);
        tcpStruct1.sin_addr.s_addr=htonl(INADDR_ANY);


        if(bind(tcpSocket1,(struct sockaddr*)&tcpStruct1,sizeof(tcpStruct1))<0)
            cout<<"Error in Binding on port 5002"<<endl;
        else
            cout<<"Bind Successful of tcp on 5002"<<endl;

            while(1){
                socklen_t len=sizeof(clientStruct);
                int confd;

                listen(tcpSocket,10);
                listen(tcpSocket1,10);

                confd=accept(tcpSocket,(struct sockaddr *)&clientStruct,&len);

                read(confd,mac_address,MAC_ADDRESS_SIZE);

                int i_port;
                char i_ip_address[SIZE_OF_IPV4_ADDRESS],port[6];

                i_port=ntohs(clientStruct.sin_port);
                sprintf(port,"%d",i_port);
                inet_ntop(AF_INET,&clientStruct.sin_addr,i_ip_address,SIZE_OF_IPV4_ADDRESS);
                cout<<"IP Address : "<<i_ip_address<<"\n Port : "<<i_port<<"\n MAC address : "<<mac_address<<endl;
                // Now send udp connection request to the r client
                send_udp_conn_request_tor(i_port,i_ip_address,mac_address,udpSocket);

                // End of step 4 
                close(udpSocket);
                //listen for tcp on 5002 from r client

                socklen_t len1=sizeof(client1);
                int confd1=accept(tcpSocket1,(struct sockaddr*)&client1,&len1);

                if(confd1<0)
                    cout<<"Error in connecting on port 5002"<<endl;
                else
                    cout<<"tcp connection established from r to server"<<endl;

                write(confd1,i_ip_address,sizeof(i_ip_address));
                write(confd1,port,sizeof(i_port));

                // end of step 6

                //start of step 9

                char ip[SIZE_OF_IPV4_ADDRESS],port2i[6];
                memset(ip,0,sizeof(ip));
                inet_ntop(AF_INET,&client1.sin_addr,ip,sizeof(ip));
                int intport1=ntohs(client1.sin_port);
                memset(port2i,0,sizeof(port2i));
                sprintf(port2i,"%d",intport1);

                close(confd1);

                confd1=accept(tcpSocket1,(struct sockaddr *)NULL,NULL);

                write(confd,ip,sizeof(ip));
                write(confd,port2i,sizeof(port2i));

                close(confd);
                close(confd1);

        }
}