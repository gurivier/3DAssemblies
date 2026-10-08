
#include <iostream>
#include <string.h>
#include <math.h>

#include "PolhemusReader.hpp"

const int PolhemusReader::BUFFER_SIZE = 1000 ;
const int PolhemusReader::MAX_CNX_ATTEMPS = 10 ;

PolhemusReader::PolhemusDevice::PolhemusDevice ()
  : read (false),
    coord ()
{
  // Nothing
}

PolhemusReader::PolhemusDevice::~PolhemusDevice ()
{
  // Nothing
}

PolhemusReader::PolhemusReader ()
  : m_pTrak (NULL),
    m_connected (false)
{

}

PolhemusReader::~PolhemusReader ()
{
  stop() ;
}

bool PolhemusReader::init ()
{ 
  const char trackerName[] = "Liberty" ;
  const int vid            = 0x0f44 ;      // Liberty Vendor Id
  const int pid            = 0xff12 ;      // Liberty Product Id
  const int writeEp        = 0x02 ;        // Liberty Write Endpoint
  const int readEp         = 0x82 ;        // Liberty Read Endpoint
  int cnxSuccess, attempts=0 ;

  // The communication with the tracker
  m_pTrak = new PiTracker ;
  if (!m_pTrak){
    std::cerr << "Memory Allocation Error creating tracker communications module" << std::endl ;
    return false;
  }

  /* USB CONNEXION */
  while ((cnxSuccess = m_pTrak->UsbConnect(vid, pid, writeEp, readEp)) != 0 && attempts < MAX_CNX_ATTEMPS) {
    attempts++ ;
  }

  if (cnxSuccess == 0) {
    std::cout << "Connected to " << trackerName << " over USB" << std::endl ;
    m_connected = true ;
  }
  else {
    std::cerr << "Connexion to " << trackerName << " over USB failed" << std::endl ;
  }

  return m_connected ;
}

void PolhemusReader::stop ()
{
  m_connected = false ;
  if (m_pTrak != NULL) {
    delete m_pTrak ;
    m_pTrak = NULL ;
  }
}

bool PolhemusReader::idle ()
{
  char buf[BUFFER_SIZE] ;
  int len ;
  struct sensor_datagram *psensor1, *psensor2 ;
  
  //-- PUT TRACKER INTO BINARY MODE
  m_pTrak->WriteTrkData("F1\r", 3);

  //-- SETUP OUTPUT DATA LIST
  // 2: X, Y, Z Cartesian coordinates of position    => 3x float    => 12 bytes
  // 7: Orientation Quaternion                       => 4x float    => 16 bytes
  // 1: ASCII carriage return, linefeed              => 2x char     =>  2 bytes
  m_pTrak->WriteTrkData("O*,2,7,1\r", 9); 

  //-- ASK FOR DATA
  m_pTrak->WriteTrkData("P", 1);  

  //-- READ DATA
  len = m_pTrak->ReadTrkData(buf, BUFFER_SIZE);

  //-- PUT TRACKER BACK TO ASCII MODE
  m_pTrak->WriteTrkData("F0\r", 3);

  if (len > 0 && len < BUFFER_SIZE){
    buf[len] = 0 ;  // null terminate
  }
  else {
    std::cout << "nBytesRead: " << len << std::endl ;
    return false ;
  }

  psensor1 = (struct sensor_datagram *)(buf) ;
  psensor2 = (struct sensor_datagram *)(buf + 8 + psensor1->ResponseSize) ; // Response Header Size (8 bytes) + Response Body Size (28 + 2 bytes = 30)

  //-- ANALYSE DATA
    
  // check for proper format LY for Liberty
  if (strncmp(psensor1->FrameTag, "LY", 2) || strncmp(psensor2->FrameTag, "LY", 2)) {
    std::cerr << "Corrupted data received" << std::endl ;
    //std::cerr << buf << std::endl ;
    return false ;
  }

  static const float POS_PRECISION = 0.01f ;
  static const float POS_PRETRUNC = 1.0f / POS_PRECISION ;
 
  // Read Polhemus 1
  pol1.coord.px = trunc (-psensor1->x * POS_PRETRUNC) * POS_PRECISION ;
  pol1.coord.py = trunc ( psensor1->y * POS_PRETRUNC) * POS_PRECISION ;
  pol1.coord.pz = trunc ( psensor1->z * POS_PRETRUNC) * POS_PRECISION ;
  pol1.coord.qr = psensor1->q0 ;
  pol1.coord.qi = psensor1->q1 ;
  pol1.coord.qj = psensor1->q2 ;
  pol1.coord.qk = psensor1->q3 ;
  pol1.read = true ;

  // Read Polhemus 2
  pol2.coord.px = trunc (-psensor2->x * POS_PRETRUNC) * POS_PRECISION ;
  pol2.coord.py = trunc ( psensor2->y * POS_PRETRUNC) * POS_PRECISION ;
  pol2.coord.pz = trunc ( psensor2->z * POS_PRETRUNC) * POS_PRECISION ;
  pol2.coord.qr = psensor2->q0 ;
  pol2.coord.qi = psensor2->q1 ;
  pol2.coord.qj = psensor2->q2 ;
  pol2.coord.qk = psensor2->q3 ;
  pol2.read = true ;

  return true ;
}

bool PolhemusReader::isConnected ()
{
  return m_connected ;
}
