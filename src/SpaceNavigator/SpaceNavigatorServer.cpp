#include <iostream>
#include <cstdio>        /* std::perror() */

#include <sys/types.h>   /* socket */
#include <stdlib.h>
#include <unistd.h>      /* socket */
#include <string.h>      /* socket */
#include <sys/socket.h>  /* socket */
#include <netdb.h>       /* socket */

#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

#include <signal.h>

#include "colormod.hpp"

#include "SpaceNavigatorServer.hpp"

int SpaceNavigatorServer::msqid ;
bool SpaceNavigatorServer::loop = true ;

SpaceNavigatorServer::SpaceNavigatorServer (char *port_s)
  : m_ressourceAllocated (false)
{
  this->sp1.ping = false ;
  this->sp2.ping = false ;
  this->sp1.read = false ;
  this->sp2.read = false ;

  m_mbuf.mdata = (SpaceNavigator*)malloc (sizeof(SpaceNavigator)) ;

  if ((SpaceNavigatorServer::msqid = msgget(IPC_PRIVATE, MSG_W | MSG_R)) == -1) {
    std::cerr << "Erreur msgget()" << std::endl ;
    m_ressourceAllocated = false ;
    exit (1) ;
  }
  else {
    m_ressourceAllocated = true ;
  }

  /* Creating Thread */

  if (pthread_create(&m_thread, NULL, this->thread_spnav, port_s) == 0) {
    std::cerr << "[SPNAV SERVER] Thread OK" << std::endl ;
  }
  else {
    std::cerr << "[SPNAV SERVER] Thread PB" << std::endl ;
  }

}

SpaceNavigatorServer::~SpaceNavigatorServer ()
{
  std::cerr << "SpaceNavigatorServer::~SpaceNavigatorServer() BEGIN" << std::endl ;

  SpaceNavigatorServer::loop = false ;

  if (m_mbuf.mdata != NULL) {
    free (m_mbuf.mdata) ;
    m_mbuf.mdata = NULL ;
  }

  this->freeRessources () ;

  std::cerr << "SpaceNavigatorServer::~SpaceNavigatorServer() END" << std::endl ;
}

/**
 * Lire dans la file de messages les evenements envoyes par thread_spnav()
 * Appelee par Controller::readCoordsFromSpaceNavigators(), elle-meme appelee depuis Controller::idle().
 */
bool SpaceNavigatorServer::idle ()
{
  //std::cout << "SpaceNavigatorServer::idle() BEGIN" << std::endl ;

  if (msgrcv (SpaceNavigatorServer::msqid, &m_mbuf, sizeof(SpaceNavigator), 0, IPC_NOWAIT) == sizeof(SpaceNavigator)) {

    m_spnv = *(m_mbuf.mdata) ;

    //std::cerr << "==== Device " << m_spnv.dev_num << std::endl ;

    if (m_spnv.evt.type == SPNAV_EVENT_MOTION) {

      if (m_spnv.dev_num == 1) {
        this->sp1.read = true ;
        /* Read values and normalize in [-1.0 ; +1.0] */
        this->sp1.coord.px = m_spnv.evt.motion.x * NORMALIZE_VAL ;
        this->sp1.coord.py = m_spnv.evt.motion.y * NORMALIZE_VAL ;
        this->sp1.coord.pz = m_spnv.evt.motion.z * NORMALIZE_VAL ;
        this->sp1.coord.rx = m_spnv.evt.motion.rx * NORMALIZE_VAL ;
        this->sp1.coord.ry = m_spnv.evt.motion.ry * NORMALIZE_VAL ;
        this->sp1.coord.rz = m_spnv.evt.motion.rz * NORMALIZE_VAL ;
      }
      else if (m_spnv.dev_num == 2) {
        this->sp2.read = true ;
        /* Read values and normalize in [-1.0 ; +1.0] */
        this->sp2.coord.px = m_spnv.evt.motion.x * NORMALIZE_VAL ;
        this->sp2.coord.py = m_spnv.evt.motion.y * NORMALIZE_VAL ;
        this->sp2.coord.pz = m_spnv.evt.motion.z * NORMALIZE_VAL ;
        this->sp2.coord.rx = m_spnv.evt.motion.rx * NORMALIZE_VAL ;
        this->sp2.coord.ry = m_spnv.evt.motion.ry * NORMALIZE_VAL ;
        this->sp2.coord.rz = m_spnv.evt.motion.rz * NORMALIZE_VAL ;          
      }

      //std::cerr << "motion event: t(" << m_spnv.evt.motion.x << ", " << m_spnv.evt.motion.y << ", " << m_spnv.evt.motion.z << ") " ;
      //std::cerr << "r(" << m_spnv.evt.motion.rx << ", " << m_spnv.evt.motion.ry << ", " << m_spnv.evt.motion.rz << ")" <<  std::endl ;
    }
    else if (m_spnv.evt.type == SPNAV_EVENT_BUTTON) {
      std::cerr << "[SPNAV SERVER] button " << (m_spnv.evt.button.press ? "press" : "release") << " event b(" << m_spnv.evt.button.bnum << ")" << std::endl  ;

      if (m_spnv.dev_num == 1) {
        if (m_spnv.evt.button.bnum == SPNAV_BUTTON_LEFT) {
          this->sp1.leftButtonChanged = true ;
          this->sp1.leftButtonPosition = (m_spnv.evt.button.press ? SPNAV_PRESSED : SPNAV_RELEASED) ;
        }
        else {
          this->sp1.rightButtonChanged = true ;
          this->sp1.rightButtonPosition = (m_spnv.evt.button.press ? SPNAV_PRESSED : SPNAV_RELEASED) ;
        }
      }
      else if (m_spnv.dev_num == 2) {
        if (m_spnv.evt.button.bnum == SPNAV_BUTTON_LEFT) {
          this->sp2.leftButtonChanged = true ;
          this->sp2.leftButtonPosition = (m_spnv.evt.button.press ? SPNAV_PRESSED : SPNAV_RELEASED) ;
        }
        else {
          this->sp2.rightButtonChanged = true ;
          this->sp2.rightButtonPosition = (m_spnv.evt.button.press ? SPNAV_PRESSED : SPNAV_RELEASED) ;
        }
      }
    }
    else {  /* SPNAV_EVENT_PING */
      if (m_spnv.dev_num == 1) {
        this->sp1.ping = true ;
      }
      else if (m_spnv.dev_num == 2) {
        this->sp2.ping = true ;
      }
    }

    //std::cout << "SpaceNavigatorServer::idle() END" << std::endl ;
    return true ;
  }
  else {
    /* std::cerr << "===== rien" << std::endl ; */
    //std::cout << "SpaceNavigatorServer::idle() END" << std::endl ;
    return false ;
  }
}

/**
 * Ecouter la connexion socket pour recevoir les evenements envoyes par le client qui ecoute le SpaceNavigator.
 */
void *SpaceNavigatorServer::thread_spnav (void *port_s)  {
  int sfd, s;
  struct addrinfo hints;
  struct addrinfo *result, *rp;
  char host[NI_MAXHOST], service[NI_MAXSERV];
  struct sockaddr_storage peer_addr;
  socklen_t peer_addr_len;
  ssize_t nread;
  /*
  char buf[BUF_SIZE];
  */

  spnav_event sev;
  char device_num ;
  SpaceNavigator spnv ;
  struct sp_msgbuf mbuf ;
  mbuf.mdata = (SpaceNavigator*)malloc (sizeof(SpaceNavigator)) ;


  std::cerr << "[SPNAV THREAD] hello routine" << std::endl ;

  std::cerr << "[SPNAV THREAD] port = " << (const char *)port_s << std::endl ;
  std::flush(std::cerr);

  /* Opening Socket */

  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
  hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
  hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
  hints.ai_protocol = 0;          /* Any protocol */
  hints.ai_canonname = NULL;
  hints.ai_addr = NULL;
  hints.ai_next = NULL;

  s = getaddrinfo(NULL, (const char *)port_s, &hints, &result);
  if (s != 0) {
    std::cerr << "[SPNAV THREAD] getaddrinfo: " << gai_strerror(s) << std::endl ;
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
    std::cerr << "[SPNAV THREAD] Could not bind" << std::endl ;
    exit(EXIT_FAILURE);
  }

  freeaddrinfo(result);           /* No longer needed */

  std::cerr << "[SPNAV THREAD] Ready. Waiting for connexions..." << std::endl ;

  /* Read datagrams and echo them back to sender */
  while (SpaceNavigatorServer::loop) {
    peer_addr_len = sizeof(struct sockaddr_storage);

    /* Read device num */
    nread = recvfrom(sfd, &device_num, sizeof(char), 0, (struct sockaddr *) &peer_addr, &peer_addr_len);

    if (device_num > 10) { // recu test PING "is alive" (1 ou 2 => donnees, 11 ou 12 => ping, -1 => quit)

      device_num -= 10 ;

      spnv.dev_num = device_num ;
      spnv.evt.type = SPNAV_EVENT_PING ;

      mbuf.mtype = 1 ;
      *(mbuf.mdata) = spnv ;

      /* Ajouter l'evenement dans la file d'attente de messages */
        if (msgsnd (SpaceNavigatorServer::msqid, &mbuf, sizeof(SpaceNavigator), 0) == -1) {
          std::cerr << "[SPNAV THREAD] msgsnd : error" << std::endl ;

          switch (errno) {
          case EACCES: std::cerr << "[SPNAV THREAD] EACCES: The calling process does not have write permission on the message queue, and does not have the CAP_IPC_OWNER capability." << std::endl ; break ;
          case EAGAIN: std::cerr << "[SPNAV THREAD] EAGAIN: The message can't be sent due to the msg_qbytes limit for the queue and IPC_NOWAIT was specified in msgflg." << std::endl ; break ;
          case EFAULT: std::cerr << "[SPNAV THREAD] EFAULT: The address pointed to by msgp isn't accessible." << std::endl ; break ;
        case EIDRM:  std::cerr << "[SPNAV THREAD] EIDRM: The message queue was removed." << std::endl ; break ;
          case EINTR:  std::cerr << "[SPNAV THREAD] EINTR: Sleeping on a full message queue condition, the process caught a signal." << std::endl ; break ;
          case EINVAL: std::cerr << "[SPNAV THREAD] EINVAL: Invalid msqid value, or nonpositive mtype value, or invalid msgsz value (less than 0 or greater than the system value MSGMAX)." << std::endl ; break ;
          case ENOMEM: std::cerr << "[SPNAV THREAD] ENOMEM: The system does not have enough memory to make a copy of the message pointed to by msgp." << std::endl ; break ;
          default:
            std::cerr << "[SPNAV THREAD] Autre erreur !" << std::endl ;
          }
        }


    }
    else if (device_num > 0) { // Donnees recues

      /* Read SpaceNavigator event */
      nread = recvfrom(sfd, &sev, sizeof(spnav_event), 0, (struct sockaddr *) &peer_addr, &peer_addr_len);
      
      if (nread == -1)
        continue;               /* Ignore failed request */

      s = getnameinfo((struct sockaddr *) &peer_addr, peer_addr_len, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICSERV);

      if (s == 0) {
        /* printf("Received %ld bytes from %s:%s\n", (long) nread, host, service); */

        /* std::cerr << "Device " << device_num << std::endl ; */
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

        /* Ajouter l'evenement dans la file d'attente de messages */
        if (msgsnd (SpaceNavigatorServer::msqid, &mbuf, sizeof(SpaceNavigator), 0) == -1) {
          std::cerr << "[SPNAV THREAD] msgsnd : error" << std::endl ;

          switch (errno) {
          case EACCES: std::cerr << "[SPNAV THREAD] EACCES: The calling process does not have write permission on the message queue, and does not have the CAP_IPC_OWNER capability." << std::endl ; break ;
          case EAGAIN: std::cerr << "[SPNAV THREAD] EAGAIN: The message can't be sent due to the msg_qbytes limit for the queue and IPC_NOWAIT was specified in msgflg." << std::endl ; break ;
          case EFAULT: std::cerr << "[SPNAV THREAD] EFAULT: The address pointed to by msgp isn't accessible." << std::endl ; break ;
        case EIDRM:  std::cerr << "[SPNAV THREAD] EIDRM: The message queue was removed." << std::endl ; break ;
          case EINTR:  std::cerr << "[SPNAV THREAD] EINTR: Sleeping on a full message queue condition, the process caught a signal." << std::endl ; break ;
          case EINVAL: std::cerr << "[SPNAV THREAD] EINVAL: Invalid msqid value, or nonpositive mtype value, or invalid msgsz value (less than 0 or greater than the system value MSGMAX)." << std::endl ; break ;
          case ENOMEM: std::cerr << "[SPNAV THREAD] ENOMEM: The system does not have enough memory to make a copy of the message pointed to by msgp." << std::endl ; break ;
          default:
            std::cerr << "[SPNAV THREAD] Autre erreur !" << std::endl ;
          }
        }
      }
      else {
        std::cerr << "[SPNAV THREAD] getnameinfo: " << gai_strerror(s) << std::endl ;
      }

    }
    else { /* device_num == -1  */

      std::cerr << Color::FG_BLUE << "[SPNAV THREAD] Received -1 " << std::endl ;      

      SpaceNavigatorServer::loop = false ;
    }

    /* Send confirmation mesage */
    /*
    if (sendto(sfd, buf, nread, 0, (struct sockaddr *) &peer_addr, peer_addr_len) != nread) {
      std::cerr << "Error sending response" << std::endl ;
    }
    */
  } /* end while */

  if (mbuf.mdata != NULL) {
    free(mbuf.mdata) ;
    mbuf.mdata = NULL ;
  }

  std::cerr << "[SPNAV THREAD] Thread terminated." << std::endl ;

  pthread_exit (0) ;

  return NULL ;
}

void SpaceNavigatorServer::freeRessources () {

    /* Killing thread */
    // pthread_kill (m_thread, SIGTERM) ;
    //pthread_cancel(m_thread) ;
    //close(sfd); // Closing socket resource opened and used by thread_spnav()

  //std::cerr << "[SPNAV SERVER] Sending -1 to thread." << std::endl ;

  if (m_ressourceAllocated) {
    m_ressourceAllocated = false ;
    std::cerr << "[SPNAV SERVER] Free ressources." << std::endl ;

    if (msgctl(SpaceNavigatorServer::msqid, IPC_RMID, NULL) < 0) {
      std::perror( strerror(errno) );
      std::cerr << "[SPNAV SERVER] msgctl (return queue) failed to remove queue " << SpaceNavigatorServer::msqid << std::endl ;
    }

  }
}
