/* The port number is passed as an argument */
//#define _BSD_SOURCE
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

// #ifdef WIN32
// #define SEED_RAND()     srand(time(NULL))
// #define URAND()         (((double)rand())/ (double)RAND_MAX)
//
// #else
// #define SEED_RAND()     srand48(time(NULL))
// #define URAND()         drand48()
// #endif


void error(const char *msg){
  perror(msg);
  exit(1);
}

void display_vector(const char *name, std::vector<double> v){
  std::cout << name << ": ";
  for (std::vector<double>::const_iterator i = v.begin(); i != v.end(); ++i)
    std::cout << *i << ' ';
  std::cout << std::endl;
}



int main(int argc, char *argv[]){
  using namespace std;

  // LWPR object
  int nin = 10;
  int nout = 1;
  int njoints = 2;
  LWPR_andPC_DCN ml_dcn_1(nin, nout, njoints);

  // Server variables
  int sockfd, newsockfd, portno;
  socklen_t clilen;
  char buffer[256];
  struct sockaddr_in serv_addr, cli_addr;
  int n;

  // Server setup
  if (argc < 2) {
    fprintf(stderr,"ERROR, no port provided\n");
    exit(1);
  }
  // Create a socket: socket(int domain, int type, int protocol)
  sockfd =  socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
    error("ERROR opening socket");

  // Clear address structure: bzero() copies zeros into string
  bzero((char *) &serv_addr, sizeof(serv_addr));

  portno = atoi(argv[1]);  // atoi: convert a string to an integer
  printf("%s\n", argv[1]);

  /* Setup the host_addr structure for use in bind call */
  // Server byte order
  serv_addr.sin_family = AF_INET;

  // automatically be filled with current host's IP address
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  printf("%c\n", INADDR_ANY);

  // convert short integer value for port must be converted into network byte order
  serv_addr.sin_port = htons(portno);

  // bind(int fd, struct sockaddr *local_addr, socklen_t addr_length)
  // bind() passes file descriptor, the address structure,
  // and the length of the address structure
  // This bind() call will bind  the socket to the current IP address on port, portno
  if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    error("ERROR on binding");

  // This listen() call tells the socket to listen to the incoming connections.
  // The listen() function places all incoming connection into a backlog queue
  // until accept() call accepts the connection.
  // Here, we set the maximum size for the backlog queue to 5.
  listen(sockfd,5);

  // The accept() call actually accepts an incoming connection
  clilen = sizeof(cli_addr);

  // This accept() function will write the connecting client's address info
  // into the the address structure and the size of that structure is clilen.
  // The accept() returns a new socket file descriptor for the accepted connection.
  // So, the original socket file descriptor can continue to be used
  // for accepting new connections while the new socker file descriptor is used for
  // communicating with the connected client.
  newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
  if (newsockfd < 0)
    error("ERROR on accept");

  printf("server: got connection from %s port %d\n",
    inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));

  // Data interchange
  while (1) {
    // This send() function sends the 13 bytes of the string to the new socket
    send(newsockfd, "Hello, world!\n", 13, 0);

    bzero(buffer,256);

    // LWPR cycle
    doubleVec fb_signal_torque;
    fb_signal_torque.push_back(2.3);
    doubleVec efferent_copy_torque;
    efferent_copy_torque.push_back(2.3);
    doubleVec input_lwpr;
    input_lwpr.push_back(32.0);

    ml_dcn_1.ML_prediction(input_lwpr, fb_signal_torque);

    ml_dcn_1.ML_update(input_lwpr, efferent_copy_torque);



    n = read(newsockfd,buffer,255);
    if (n < 0) error("ERROR reading from socket");
    printf("Here is the message: %s\n",buffer);

    // Do stuff
  }

  // End of connection
  close(newsockfd);
  close(sockfd);
  return 0;
  // return C_main(argc, argv);
}
