#ifndef POLHEMUSREADER_HPP
#define POLHEMUSREADER_HPP

#include "Coordinates.hpp"
#include "PiTracker.hpp"


// SENSOR DATAGRAM ACCORDING TO OUTPUT DATA LIST :
// 2: X, Y, Z Cartesian coordinates of position    => 3x float    => 12 bytes
// 7: Orientation Quaternion                       => 4x float    => 16 bytes
// 1: ASCII carriage return, linefeed              => 2x char     =>  2 bytes
struct sensor_datagram {

  // Header (8 bytes)
  char           FrameTag[2] ; // always 'LY' or 0x5041 for LIBERTY HST
  unsigned char  StationNumber ;
  unsigned char  InitiatingCommand ;
  unsigned char  ErrorIndicator ;
  unsigned char  Reserved ;
  unsigned short ResponseSize ; // number of bytes in the response body

  // Position in X,Y,Z Cartesian coordinates (12 bytes)
  float x ;
  float y ;
  float z ;

  // Orientation in Quaternion representation (16 bytes)
  float q0 ;
  float q1 ;
  float q2 ;
  float q3 ;

  // EndPoint
  //char CR ; // ignore
  //char LF ; // ignore
} ;


class PolhemusReader {

public:

  class PolhemusDevice {
  public :
    PolhemusDevice () ;
    ~PolhemusDevice () ;
    bool read ; // Becomes true when new Position or Rotation have been read from the device
    Coordinates coord ; // Position and Rotation values read from the device
  };

  PolhemusDevice pol1 ; // Position and Rotation values read from the device
  PolhemusDevice pol2 ; // Position and Rotation values read from the device

public:

  PolhemusReader () ;
  ~PolhemusReader () ;

  bool init () ;
  void stop () ;
  bool idle () ;

  bool isConnected () ;

private:
  
  PiTracker *m_pTrak;
  bool m_connected ;

  static const int BUFFER_SIZE ;
  static const int MAX_CNX_ATTEMPS ;

} ;

#endif /* POLHEMUSREADER_HPP */
