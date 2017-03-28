#include <arpa/inet.h>
// extern "C" {
#include <lwpr.h>
// }
#include <lwpr.hh>
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

#ifdef WIN32
#define SEED_RAND()     srand(time(NULL))
#define URAND()         (((double)rand())/ (double)RAND_MAX)

#else
#define SEED_RAND()     srand48(time(NULL))
#define URAND()         drand48()
#endif

using namespace std;

void error(const char *msg){
  perror(msg);
  exit(1);
}

void display_vector(const char *name, vector<double> v){
  cout << name << ": ";
  for (vector<double>::const_iterator i = v.begin(); i != v.end(); ++i)
    cout << *i << ' ';
  cout << endl;
}


class MLandC{  // inherits from LWPR_Object
  public:
    vector<double> wt;
    vector<double> w;
    vector<double> w_pc_dcn;
    vector<double> w_mf_dcn;

    MLandC(){
      cout << "Child class constructor" << endl;
      wt.push_back(0);
      wt.push_back(0.3);

      display_vector("wt", wt);
      // printf("wt[0] = %lf \n", wt[0]);
    }
};


// extern "C" int C_main(int, char**);
int main(int argc, char *argv[]){
  MLandC ml;

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
