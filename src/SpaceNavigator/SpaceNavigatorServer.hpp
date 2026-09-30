#ifndef SPACENAVIGATORSERVER_HPP
#define SPACENAVIGATORSERVER_HPP

#include <pthread.h>

#include "Coordinates.hpp"

/* ================== */

/* Problemes de compatibilite avec Qt (definition de Bool dans X11 lib) */
//#include <spnav.h>

/* Recopie des definitions dans /usr/include/spnav.h */


enum {
	SPNAV_EVENT_ANY,	/* used by spnav_remove_events() */
	SPNAV_EVENT_MOTION,
	SPNAV_EVENT_BUTTON	/* includes both press and release */
};

#include "spnav_events.h"
 
struct spnav_event_motion {
	int type;
	int x, y, z;
	int rx, ry, rz;
	unsigned int period;
	int *data;
};

struct spnav_event_button {
	int type;
	int press;
	int bnum;
};

typedef union spnav_event {
	int type;
	struct spnav_event_motion motion;
	struct spnav_event_button button;
} spnav_event;

/* ==================*/

#define BUF_SIZE 500

#define MSG_R 0400
#define MSG_W 0200

enum SPNAV_BUTTON_ID { SPNAV_BUTTON_LEFT, SPNAV_BUTTON_RIGHT } ; // 0 : left, 1: right
enum SPNAV_BUTTON_POSITION { SPNAV_RELEASED, SPNAV_PRESSED } ; // 0 : released, 1 : pressed

typedef struct SpaceNavigator {
  int dev_num ;
  spnav_event evt ;
} SpaceNavigator ;

struct sp_msgbuf {
  long mtype;       /* message type, must be > 0 */
  SpaceNavigator *mdata;    /* message data */
};

class SpaceNavigatorServer {

public:
  static const int MAX_VAL = 350 ; // Higher possible value for Position or Rotation wrote by the device
  static const int MIN_VAL = -MAX_VAL ; // Lower possible value for Position or Rotation wrote by the device
  static const float NORMALIZE_VAL = 1.0f / MAX_VAL  ; // Value to multiply by in order to normalize values

public:

  class SpaceNavigatorDevice {
  public :
    bool ping ; // Becomes true when new Ping has been sent by the client
    bool read ; // Becomes true when new Position or Rotation have been read from the device
    Coordinates coord ; // Position and Rotation values read from the device
    bool leftButtonChanged, rightButtonChanged ; // Becomes true when new button state have been read from the device
    enum SPNAV_BUTTON_POSITION leftButtonPosition, rightButtonPosition ; // Position value of the button read from the device
  };

  SpaceNavigatorDevice sp1 ;
  SpaceNavigatorDevice sp2 ;

public:

  SpaceNavigatorServer (char *port_s) ;

  ~SpaceNavigatorServer () ;

  bool idle () ;

  static void *thread_spnav (void *port_s) ;

  void freeRessources () ;

private:

  static int        msqid ;
  static bool       loop ;

  SpaceNavigator    m_spnv ;
  pthread_t         m_thread ;
  struct sp_msgbuf  m_mbuf ;

  bool m_ressourceAllocated ;
  
} ;

#endif /* SPACENAVIGATORSERVER_HPP */
