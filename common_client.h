#include <iostream>
#include <pthread.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>
#include <net/if.h>   
#include <unistd.h>  
#include <arpa/inet.h> 
#include <stdio.h>
#include <iostream>

using namespace std;

#define SIZE_OF_IPV4_ADDRESS 16
#define MAC_ADDRESS_SIZE 17

struct client{
	int port;
	char ip_address[SIZE_OF_IPV4_ADDRESS];
	char mac_address[MAC_ADDRESS_SIZE];
	struct client *next;
};

char *get_mac_address(){
	struct ifreq ifr;
	char *iface = "wlan0";
	unsigned char *mac;
	static char buff[MAC_ADDRESS_SIZE];
	int fd = socket(AF_INET, SOCK_DGRAM, 0);
	ifr.ifr_addr.sa_family = AF_INET;
	strncpy(ifr.ifr_name , iface , IFNAMSIZ-1);
	ioctl(fd, SIOCGIFHWADDR, &ifr);
	close(fd);
	mac = (unsigned char *)ifr.ifr_hwaddr.sa_data;  
	snprintf(buff, 17, "%02x:%02x:%02x:%02x:%02x:%02x",mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	return buff;
}
