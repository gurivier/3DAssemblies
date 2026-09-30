#include <sys/types.h>   /* socket */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>      /* socket */
#include <string.h>      /* socket */
#include <sys/socket.h>  /* socket */
#include <netdb.h>       /* socket */

#include <spnav.h>

#define BUF_SIZE 500

int main (int argc, char *argv[]) {
  struct addrinfo hints;
  struct addrinfo *result, *rp;
  int sfd, s;
  struct sockaddr_storage peer_addr;
  socklen_t peer_addr_len;
  char host[NI_MAXHOST], service[NI_MAXSERV];
  ssize_t nread;
  /*
  char buf[BUF_SIZE];
  */

  spnav_event sev;
  char device_num ;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s port\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
  hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
  hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
  hints.ai_protocol = 0;          /* Any protocol */
  hints.ai_canonname = NULL;
  hints.ai_addr = NULL;
  hints.ai_next = NULL;

  s = getaddrinfo(NULL, argv[1], &hints, &result);
  if (s != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
    exit(EXIT_FAILURE);
  }

  /* getaddrinfo() returns a list of address structures.
     Try each address until we successfully bind(2).
     If socket(2) (or bind(2)) fails, we (close the socket
     and) try the next address. */

  for (rp = result; rp != NULL; rp = rp->ai_next) {
    sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);

    if (sfd == -1)
      continue;

    if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
      break;                  /* Success */

    close(sfd);
  }

  if (rp == NULL) {               /* No address succeeded */
    fprintf(stderr, "Could not bind\n");
    exit(EXIT_FAILURE);
  }

  freeaddrinfo(result);           /* No longer needed */

  printf ("Ready. Waiting for connexions...\n") ;

  /* Read datagrams and echo them back to sender */
  for (;;) {
    peer_addr_len = sizeof(struct sockaddr_storage);

    /* Read device num */
    nread = recvfrom(sfd, &device_num, sizeof(char), 0, (struct sockaddr *) &peer_addr, &peer_addr_len);

    /* Read SpaceNavigator event */
    nread = recvfrom(sfd, &sev, sizeof(spnav_event), 0, (struct sockaddr *) &peer_addr, &peer_addr_len);
      
    if (nread == -1)
      continue;               /* Ignore failed request */

    s = getnameinfo((struct sockaddr *) &peer_addr, peer_addr_len, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICSERV);

    if (s == 0) {
      /* printf("Received %ld bytes from %s:%s\n", (long) nread, host, service); */

      printf ("Device %d ", device_num) ;

      if(sev.type == SPNAV_EVENT_MOTION) {
        printf("motion event: t(%d, %d, %d) ", sev.motion.x, sev.motion.y, sev.motion.z);
        printf("r(%d, %d, %d)\n", sev.motion.rx, sev.motion.ry, sev.motion.rz);
      } else {	/* SPNAV_EVENT_BUTTON */
        printf("button %s event b(%d)\n", sev.button.press ? "press" : "release", sev.button.bnum);
      }

    }
    else {
      fprintf(stderr, "getnameinfo: %s\n", gai_strerror(s));
    }

    /* Send confirmation mesage */
    /*
    if (sendto(sfd, buf, nread, 0, (struct sockaddr *) &peer_addr, peer_addr_len) != nread) {
      fprintf(stderr, "Error sending response\n");
    }
    */
  }
}
