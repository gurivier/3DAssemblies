/* gcc client.c -DBUILD_AF_UNIX -lpthread -lspnav -o client_af_unix */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <signal.h>
#include <X11/Xlib.h>
#include <spnav.h>

#include <pthread.h>

#include "spnav_events.h"

#define BUF_SIZE 500

static char thread_continue = 1 ;
static char device_num ;
static int sfd ;
static pthread_mutex_t mutex ;

void sig (int s) {
  printf ("Free ressources.\n") ;
  thread_continue = 0 ;
  spnav_close();
  printf ("Bye.\n") ;
  exit(0);
}

/**
 * Envoyer un message ping toutes les 2 secondes au serveur sur la connexion socket.
 */
void *thread_ping (void *param) {
  spnav_event sev;  
  int nw ;
  char ping_num = device_num + 10 ;

  sev.type = SPNAV_EVENT_PING ;

  while (thread_continue) {

    pthread_mutex_lock (&mutex) ;

    /* Send device num to server */
    if ((nw = write(sfd, &ping_num, sizeof(char))) != sizeof(char)) {
      fprintf(stderr, "Dev num ") ;
      if (nw == -1) {
        perror("write error");
      }
      else {
        fprintf(stderr, "partial/failed write (%d/%lu)\n", nw, sizeof(char));
      }
      exit(EXIT_FAILURE);
    }

    /* Send ping event to server */
    //if ((nw = write(sfd, &sev, sizeof(spnav_event))) != sizeof(spnav_event)) {
    //  fprintf(stderr, "SP event ") ;
    //  if (nw == -1) {
    //    perror("write error");
    //  }
    //  else {
    //    fprintf(stderr, "partial/failed write (%d/%lu)\n", nw, sizeof(char));
    //  }
    //  exit(EXIT_FAILURE);
    //}

    pthread_mutex_unlock (&mutex) ;

    sleep (2) ; // seconds
  }

  pthread_exit (0) ;

  return NULL ;
}

/**
 * Attendre les evenements du SpaceNavigator et envoyer au serveur sur la connexion socket.
 */
int main (int argc, char *argv[]) {
  char default_host[10] = "127.0.0.1" ;
  char default_port[10] = "10222" ;
  char *host_s, *port_s, *device_num_s ;
  pthread_t thread ;

  /* For socket connection to server */

  struct addrinfo hints;
  struct addrinfo *result, *rp;
  int s;
  int nw ;
  /*
  ssize_t nread;
  char buf[BUF_SIZE];
  */

  /* For SpaceNavigator device communication */

#if defined(BUILD_X11)
  Display *dpy;
  Window win;
  unsigned long bpix;
#endif

  spnav_event sev;

  /* Check the number of arguments */

  if (argc == 2) {
    fprintf(stderr, "Usage: %s [host] [port] <device_num>\n", argv[0]);
    fprintf(stderr, "Taking default host %s and port %s\n", default_host, default_port);
    host_s = default_host ;
    port_s = default_port ;
    device_num_s = argv[1] ;
  }
  else if (argc == 4) {
    host_s = argv[1] ;
    port_s = argv[2] ;
    device_num_s = argv[3] ;
  }
  else if (argc != 4) {
    fprintf(stderr, "Usage: %s <host> [port] <device_num>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  device_num = atoi (device_num_s) ;

  /* Free resources if interrupted */

  signal(SIGINT, sig);

  /*== SOCKET CONNECTION TO SERVER ==*/

  printf ("Connecting socket to server at %s:%s...\n", host_s, port_s) ;

  /* Obtain address(es) matching host/port */

  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
  hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
  hints.ai_flags = 0;
  hints.ai_protocol = 0;          /* Any protocol */

  s = getaddrinfo(host_s, port_s, &hints, &result);
  if (s != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
    exit(EXIT_FAILURE);
  }

  /* getaddrinfo() returns a list of address structures.
     Try each address until we successfully connect(2).
     If socket(2) (or connect(2)) fails, we (close the socket
     and) try the next address. */

  for (rp = result; rp != NULL; rp = rp->ai_next) {
    sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if (sfd == -1)
      continue;

    if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
      break;                  /* Success */

    close(sfd);
  }

  if (rp == NULL) {               /* No address succeeded */
    fprintf(stderr, "Could not connect\n");
    exit(EXIT_FAILURE);
  }

  freeaddrinfo(result);           /* No longer needed */


  /*== SPACENAVIGATOR DEVICE_NUM COMMUNICATION  ==*/

  printf ("Opening SpaceNavigator device communication...\n") ;

#if defined(BUILD_X11)

  if(!(dpy = XOpenDisplay(0))) {
    fprintf(stderr, "failed to connect to the X server\n");
    return 1;
  }
  bpix = BlackPixel(dpy, DefaultScreen(dpy));
  win = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 0, 0, 1, 1, 0, bpix, bpix);
  
  /* This actually registers our window with the driver for receiving
   * motion/button events through the 3dxsrv-compatible X11 protocol.
   */
  if(spnav_x11_open(dpy, win) == -1) {
    fprintf(stderr, "failed to connect to the space navigator daemon\n");
    return 1;
  }

#elif defined(BUILD_AF_UNIX)
  if(spnav_open()==-1) {
    fprintf(stderr, "failed to connect to the space navigator daemon\n");
    return 1;
  }
#else
#error Unknown build type!
#endif

  pthread_mutex_init (&mutex, NULL) ;
  pthread_create (&thread, NULL, thread_ping, NULL) ;

  printf ("Ready to receive SPNav events and send on socket to the server.\n") ;

  /* spnav_wait_event() and spnav_poll_event(), will silently ignore any non-spnav X11 events.
   *
   * If you need to handle other X11 events you will have to use a regular XNextEvent() loop,
   * and pass any ClientMessage events to spnav_x11_event, which will return the event type or
   * zero if it's not an spnav event (see spnav.h).
   */
  while(spnav_wait_event(&sev)) {

    pthread_mutex_lock (&mutex) ;

    /* Send device num to server */
    if ((nw = write(sfd, &device_num, sizeof(char))) != sizeof(char)) {
      fprintf(stderr, "Dev num ") ;
      if (nw == -1) {
        perror("write error");
      }
      else {
        fprintf(stderr, "partial/failed write (%d/%lu)\n", nw, sizeof(char));
      }
      exit(EXIT_FAILURE);
    }

    /* Send SpaceNavigator event to server */
    if ((nw = write(sfd, &sev, sizeof(spnav_event))) != sizeof(spnav_event)) {
      fprintf(stderr, "SP event ") ;
      if (nw == -1) {
        perror("write error");
      }
      else {
        fprintf(stderr, "partial/failed write (%d/%lu)\n", nw, sizeof(char));
      }
      exit(EXIT_FAILURE);
    }

    pthread_mutex_unlock (&mutex) ;

    /* Read confirmation message */
    /*
    nread = read(sfd, buf, BUF_SIZE);
    if (nread == -1) {
      perror("read");
      exit(EXIT_FAILURE);
    }
    printf("%ld bytes were received: %s\n", (long) nread, buf);
    */
  }

  printf ("Free ressources.\n") ;
  thread_continue = 0 ;

  pthread_join (thread, NULL) ;

  spnav_close();

  printf ("Bye.\n") ;

  exit(EXIT_SUCCESS);
}
