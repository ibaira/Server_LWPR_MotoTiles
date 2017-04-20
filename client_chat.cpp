#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>

#include <arpa/inet.h>
#include <math.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>


using namespace std;
 
#define IP "localhost" //"192.168.56.1"

int main(){

    int client, server;
    int portNum = 8888;
    bool isExit = false;
    // int bufsize = 256;
    int bufsize;
    char* buffer;
 
    // char *ip= "192.168.1.101";
    struct sockaddr_in server_addr;
    client = socket(AF_INET, SOCK_STREAM, 0);

    if(client < 0){
        cout << "Error creating socket." << endl;
        exit(1);
    }

    cout<< "Client Socket created." <<endl;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; //localhost or inet_addr(IP);
    server_addr.sin_port=htons(portNum);
    
    if (connect(client,(struct sockaddr*) &server_addr, sizeof(server_addr)) == 0)
        cout << "=> Connection to the server " << inet_ntoa(server_addr.sin_addr) << " with port number: " << portNum << endl; 

    recv(client, buffer, bufsize, 0);
    cout << "Connection confirmed." << endl;
    cout << "Enter # to end the connection." <<endl;
        
    int iterations = 0;
    while (iterations < 10) {
        
        string buffer("1,2,3,4,5,6,7,8,9,0");
        // bufsize = 20; // sizeof(buffer);
        const void * a = buffer.c_str();
        send(client, a, buffer.size(), 0);
        cout << "Client sent" << "\n" ;
        // if(*buffer == '#'){
        //     send(client, buffer, bufsize, 0);
        //     *buffer = '*';
        //     isExit = true;
        // }
        // } while(*buffer != 42);

        cout<< "Received from Server:  " << "\n" ;
        char b[20];
        recv(client, b, sizeof(b), 0);
        cout << "Bytes: " << sizeof(buffer) << "\n";
        cout << "Sent Text: " << &buffer << "\n\n";

        ++iterations;
    } 

    cout<< "Connection terminated." <<endl; 
    close(client);
    
    return 0;
}