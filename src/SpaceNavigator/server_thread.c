#include <sys/types.h>   /* socket */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>      /* socket */
#include <string.h>      /* socket */
#include <sys/socket.h>  /* socket */
#include <netdb.h>       /* socket */

#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

#include <spnav.h>

#define BUF_SIZE 500

#define MSG_R 0400
#define MSG_W 0200

typedef struct SpaceNavigator {
  int dev_num ;
  spnav_event evt ;
} SpaceNavigator ;

struct msgbuf {
  long mtype;       /* message type, must be > 0 */
  SpaceNavigator *mdata;    /* message data */
};

int msqid ;

void *thread_spnav (void *port_s)  {
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
  SpaceNavigator spnv ;
  struct msgbuf mbuf ;
  mbuf.mdata = malloc (sizeof(SpaceNavigator)) ;


  printf ("hello routine\n") ;

  printf ("port = %s\n", (char *)port_s) ;

  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
  hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
  hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
  hints.ai_protocol = 0;          /* Any protocol */
  hints.ai_canonname = NULL;
  hints.ai_addr = NULL;
  hints.ai_next = NULL;

  s = getaddrinfo(NULL, port_s, &hints, &result);
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

      /* printf ("Device %d ", device_num) ; */
      /* if(sev.type == SPNAV_EVENT_MOTION) { */
      /*   printf("motion event: t(%d, %d, %d) ", sev.motion.x, sev.motion.y, sev.motion.z); */
      /*   printf("r(%d, %d, %d)\n", sev.motion.rx, sev.motion.ry, sev.motion.rz); */
      /* } else {	/\* SPNAV_EVENT_BUTTON *\/ */
      /*   printf("button %s event b(%d)\n", sev.button.press ? "press" : "release", sev.button.bnum); */
      /* } */

      spnv.dev_num = device_num ;
      spnv.evt = sev ;

      mbuf.mtype = 1 ;
      *(mbuf.mdata) = spnv ;

      if (msgsnd (msqid, &mbuf, sizeof(SpaceNavigator), 0) == -1) {
        printf ("msgsnd : error\n") ;

        switch (errno) {
        case EACCES: printf ("EACCES: The calling process does not have write permission on the message queue, and does not have the CAP_IPC_OWNER capability.\n"); break ;
        case EAGAIN: printf ("EAGAIN: The message can't be sent due to the msg_qbytes limit for the queue and IPC_NOWAIT was specified in msgflg.\n"); break ;
        case EFAULT: printf ("EFAULT: The address pointed to by msgp isn't accessible.\n"); break ;
        case EIDRM:  printf ("EIDRM: The message queue was removed.\n"); break ;
        case EINTR:  printf ("EINTR: Sleeping on a full message queue condition, the process caught a signal.\n"); break ;
        case EINVAL: printf ("EINVAL: Invalid msqid value, or nonpositive mtype value, or invalid msgsz value (less than 0 or greater than the system value MSGMAX).\n"); break ;
        case ENOMEM: printf ("ENOMEM: The system does not have enough memory to make a copy of the message pointed to by msgp.\n"); break ;
        default:
          printf ("Autre erreur !\n") ;
        }
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

  printf ("THREAD : terminated\n") ;

  return NULL ;
}

int main (int argc, char *argv[]) {
  SpaceNavigator spnv ;
  pthread_t thread ;
  /* pthread_attr_t attr ; */

  struct msgbuf mbuf ;
  mbuf.mdata = malloc (sizeof(SpaceNavigator)) ;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s port\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  if ((msqid = msgget(IPC_PRIVATE, MSG_W | MSG_R)) == -1) {
    fprintf (stderr, "Erreur msgget()\n") ;
    exit (1) ;
  }

  if (pthread_create(&thread, NULL, thread_spnav, argv[1]) == 0) {
    printf ("Thread OK\n") ;
  }
  else {
    printf ("Thread PB\n") ;
  }

  while (1) {

    if (msgrcv (msqid, &mbuf, sizeof(SpaceNavigator), 0, IPC_NOWAIT) == sizeof(SpaceNavigator)) {

      spnv = *(mbuf.mdata) ;

      printf ("==== Device %d ", spnv.dev_num) ;

      if (spnv.evt.type == SPNAV_EVENT_MOTION) {
        printf ("motion event: t(%d, %d, %d) ", spnv.evt.motion.x, spnv.evt.motion.y, spnv.evt.motion.z);
        printf ("r(%d, %d, %d)\n", spnv.evt.motion.rx, spnv.evt.motion.ry, spnv.evt.motion.rz);
      } else {	/* SPNAV_EVENT_BUTTON */
        printf ("button %s event b(%d)\n", spnv.evt.button.press ? "press" : "release", spnv.evt.button.bnum);
      }

    }
    else {
      /* printf ("===== rien\n") ; */
    }

  }

  return 0 ;
}
