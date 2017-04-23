/* The port number is passed as an argument */
#define _BSD_SOURCE
#include <algorithm>
#include <arpa/inet.h>
#include <math.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <iostream>
#include "LWPRandPC_DCN.hh"

#include <sstream>

#include <iterator>  
#include <vector>
#include <string>
#include <cstdlib>
#include <iostream>


void error(const char *msg){
  perror(msg);
  exit(1);
}

void display_vector_double(const char *name, std::vector<double> v){
  std::cout << name << ": ";
  for (std::vector<double>::const_iterator i = v.begin(); i != v.end(); ++i)
    std::cout << *i << ' ';
  std::cout << std::endl;
}

void display_vector_double(std::vector<double> v){
  for (std::vector<double>::const_iterator i = v.begin(); i != v.end(); ++i)
    std::cout << *i << ' ';
  std::cout << std::endl;
}

void display_vector_int(const char *name, std::vector<int> v){
  std::cout << name << ": ";
  for (std::vector<int>::const_iterator i = v.begin(); i != v.end(); ++i)
    std::cout << *i << ' ';
  std::cout << std::endl;
}

void display_vector_int(std::vector<int> v){
  for (std::vector<int>::const_iterator i = v.begin(); i != v.end(); ++i)
    std::cout << *i << ' ';
  std::cout << std::endl;
}



int main(int argc, char* argv[]){

  using namespace std;

  // LWPR object parameters
  int nin = 10;
  int nout = 1;
  int njoints = 2;
  LWPR_andPC_DCN ml_dcn_1(nin, nout, njoints);

  // Server variables
  int sockfd, newsockfd, portno;
  socklen_t clilen;
  char* buffer; //[256]
  struct sockaddr_in serv_addr, cli_addr;
  int n;

  // Server setup
  if (argc < 2)  portno = atoi("8888"); 
  // fprintf(stderr,"ERROR, no port provided\n");  // exit(1);
  else  portno = atoi(argv[1]);  // atoi: string to integer - printf("%s\n", argv[1]);

  // Create a socket: socket(int domain, int type, int protocol)
  sockfd =  socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) error("ERROR opening socket");

  // Clear address structure: bzero() copies zeros into string
  bzero((char*) &serv_addr, sizeof(serv_addr)); 

  /* Setup the host_addr structure for use in bind call */
  // Server byte order
  serv_addr.sin_family = AF_INET;

  // Automatically be filled with current host's IP address
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  printf("%c\n", INADDR_ANY);

  // Convert short integer value for port must be converted into network byte order
  serv_addr.sin_port = htons(portno);

  /* Bind(int fd, struct sockaddr *local_addr, socklen_t addr_length)
   * bind() passes file descriptor, the address structure,
   * and the length of the address structure
   * This bind() call will bind  the socket to the current IP address on port, portno */
  if (bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
    error("ERROR on binding");

  /* This listen() call tells the socket to listen to the incoming connections.
   * The listen() function places all incoming connection into a backlog queue
   * until accept() call accepts the connection.
   * Here, we set the maximum size for the backlog queue to 5. */
  listen(sockfd, 1);

  // The accept() call actually accepts an incoming connection
  clilen = sizeof(cli_addr);

  /* This accept() function will write the connecting client's address info
   * into the the address structure and the size of that structure is clilen.
   * The accept() returns a new socket file descriptor for the accepted connection.
   * So, the original socket file descriptor can continue to be used
   * for accepting new connections while the new socker file descriptor is used for
   * communicating with the connected client.*/
  newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
  if (newsockfd < 0) error("ERROR on accept");

  printf("Server: got connection from %s port %d\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));

  // Data interchange
  int iterations = 0;
  while (iterations < 10000 && newsockfd > 0) {
    cout << "===> SERVER SENDING..." << endl;

    vector<int> v {0, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    v.insert(v.begin(), (int) sizeof(v));
    cout << "The true message 'v': ";
    display_vector_int(v);

    char buf[12] {};
    transform(begin(v), end(v), begin(buf), 
      [](int i) {
        return (i < 12) ? ('0' + i) : ('\n'); 
      }
    );
    buf[16] = '\n';
    cout << "After transforming 'v' to char: ";
    cout << buf << endl; 
  
    send(newsockfd, (const char*) buf, sizeof(buf), 0); //buffer.size()
    cout << "Server sends: " << buf << endl;

    // Send vector
    // stringstream result;        
    // copy(v.begin(), v.end(), ostream_iterator<int>(result, " "));
    // const string tmp = result.str();
    // const char* buf = tmp.c_str();

    // int nbytes =0;
    // vector<unsigned char> buffer;
    // buffer.resize(5000);
    // nbytes = recv(socket, &buffer[0], buffer.size(),0);
    // //since I want to use buffer.size() to know data length in buffer I do
    // buffer.resize(nbytes);

    /* LWPR cycle */
    /* doubleVec fb_signal_torque;
     * fb_signal_torque.push_back(2.3);
     * doubleVec efferent_copy_torque;
     * efferent_copy_torque.push_back(2.3);
     * doubleVec input_lwpr;
     * input_lwpr.push_back(32.0); 
     * ml_dcn_1.ML_prediction(input_lwpr, fb_signal_torque);
     * ml_dcn_1.ML_update(input_lwpr, efferent_copy_torque); 
     */
 
    cout << "\n<=== SERVER READING..." << endl;

    char buffer[20];// bzero(buffer, sizeof(buffer));
    
    recv(newsockfd, buffer, sizeof(buffer), 0);  //bytes2read.size());
    if (n < 0) 
      error("ERROR reading from socket");
    
    /* Parse string to Vector<int>
     * string myString = "10 15 20 23";
     * stringstream iss( myString );

     * int number;
     * vector<int> myNumbers;
     * while ( iss >> number )
     *   myNumbers.push_back( number );
     * display_vector_int( myNumbers ); */
    cout << "Iteration: " << iterations << endl;
    cout << "Received message: " << buffer << endl;
    cout << "N. Bytes: " << sizeof(buffer) << "\n\n";
    
    ++iterations;
  }

  close(newsockfd);
  cout<< "Connection with Client terminated." <<endl;   
  close(sockfd);
  cout<< "Server terminated." <<endl; 

  return 0;
}
