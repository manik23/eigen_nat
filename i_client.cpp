#include "common_client.h"
#include "i_client.h"

int main(int argc, char **argv){

	pid_t pid;
	pid=fork();

	if(pid==0){	
		// Client process


	}
	else{			// Parent process

		// Parent process will send TCP requet to server to make connection with other client
		// this client will send a tcp request on server ip and port number 3001


		// Function to send for connection request to relay server
		send_req_for_conn2_server(argv[1]);
						//step 3
	}

	return 0;
}