#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
#include <errno.h>
#include <ios>
#include <iostream>
#include <math.h>
#include <netdb.h>
#include <netinet/in.h>
#include <Poco/Exception.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>


using namespace std;
 
#define IP "localhost" //"192.168.56.1"

/*
// This assumes buffer is at least x bytes long,
// and that the socket is blocking.
void ReadXBytes(int socket, unsigned int x, void* buffer){
  int bytesRead = 0;
  int result;
  while (bytesRead < x){
    result = read(socket, buffer + bytesRead, x - bytesRead);
    if (result < 1 ){
        // Throw your error.
    }

    bytesRead += result;
  }
}
*/


int main(int argc, char* argv[]){

  int client, server;
  int portNum;
  if (argc < 2) portNum = atoi("8888");  
  else portNum = atoi(argv[1]);        

  int bufsize;
  char* buffer;     // char *ip= "192.168.1.101";
  struct sockaddr_in server_addr;
  client = socket(AF_INET, SOCK_STREAM, 0);

  if(client < 0){
    cout << "Error creating socket." << endl;
    exit(1);
  }

  cout << "Client Socket created." << endl;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY; //localhost or inet_addr(IP);
  server_addr.sin_port = htons(portNum);

  if (connect(client,(struct sockaddr*) &server_addr, sizeof(server_addr)) == 0)
    cout << "=> Connection to the server " << inet_ntoa(server_addr.sin_addr) << " with port number: " << portNum << endl; 

  recv(client, buffer, bufsize, 0);
  cout << "Connection confirmed." << endl;
        
  int iterations = 0;
  while (iterations < 10000) {   
    cout << "<=== CLIENT SENDING... " << endl; 

    char* fn = "1,2,3,4,5,6,7,8,9,0";    
    char buffer[19];
    strcpy(buffer, fn);
    cout << buffer << " - size: " << sizeof(buffer) << endl;
    send(client, (const char*) buffer, sizeof(buffer), 0); 
    
    cout << "Iteration: " << iterations << endl;
    cout << "Message: " << buffer << "\n\n";

    cout<< "===> RECEIVING FROM SERVER: " << endl;
    char buf[28];
    recv(client, buf, sizeof(buf), 0); 
    cout << "Message: " << buf << endl;
    cout << "N. Bytes: " << sizeof(buf) << "\n\n";

    // ReadXBytes(client, sizeof(length), (void*)(&length));
    // ReadXBytes(client, length, (void*)buf);

    ++iterations;
  } 

  cout<< "Connection terminated." <<endl; 
  close(client);

  return 0;
}