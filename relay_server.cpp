#include "server.h"
#include "listen_tcp.h"

int main() {
        int udpSocket;
        struct sockaddr_in udpStruct;

        udpSocket=socket(AF_INET,SOCK_DGRAM,0);

        udpStruct.sin_family=AF_INET;
        udpStruct.sin_port=htons(5000);
        udpStruct.sin_addr.s_addr=htonl(INADDR_ANY);

        if(bind(udpSocket,(struct sockaddr *)&udpStruct,sizeof(udpStruct))<0)
        {
                cout<<"Bind unsuccessfull"<<endl;
                exit(1);
        }
        else
        {
                cout<<"Binding Completed"<<endl;
        }

        pid_t pid;
        pid=fork();

        if(pid==0){                                             // Child Process 

                // Child process will listen for tcp connection request on port 5001

                listen_for_tcp_conn_req(udpSocket);  // for step 3 and 4

                //listen_for_res();

        }
        else{                                                   // Parent Process

                //Create a udp socket to receive a registration request from client

                listen_for_registration(udpSocket);             // for step 1 and 2
        }
        return 0;
}