#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "TargetManager.h"
#include "v4l2grab.h"
#include "AssemblyWindow.h"
#include "ConfigurationWindow.h"

using namespace std ;

//=========================
//== CONSTRUCTORS public
//=========================

TargetManager::TargetManager (unsigned int capacity, int imageWidth, int imageHeight)
  : m_size (0),    
    m_capacity (capacity),
    m_targets_position (0),
    m_targets_state (0),
    m_cartouches (0),
    m_imageWidth (imageWidth),
    m_imageHeight (imageHeight),
    m_contrast (25),
    m_luminosity (25),
    m_applyContrast (false),
    m_applyLuminosity (false),
    m_grayScaleDetection (true),
    m_grayScaleThreshold (128),
    m_grayScaleInverted (false),
    m_whiteThreshold (28),
    m_targetSize (16),
    m_targetAreaSizeRatio (50),
    m_areaTargetDetection (true)
{
  int dx = m_targetSize + 32 ;
  int dy = m_targetSize + 4 ;
  int x = dx ;
  int y = dy ;
  for (unsigned int i=0 ; i<m_capacity ; i++) {
    m_targets_position.push_back(QPoint(x, y)) ;

    if ((x += dx) > m_imageWidth - dx) {
      x = dx ;
      y += dy ;
    }
  }
}

TargetManager::TargetManager (const char *filename, int imageWidth, int imageHeight)
  : m_imageWidth (imageWidth),
    m_imageHeight (imageHeight)
{
  unserialize (filename) ;
}

//=========================
//== METHODS public
//=========================

void TargetManager::quit ()
{
  saveConfiguration ("setup") ;

  m_assemblyWindow->close() ;
  m_configurationWindow->close() ;
}

void TargetManager::loadConfiguration (const char *filename)
{
  cout << "Loading..." << flush ;
  unserialize (filename) ;
  m_configurationWindow->initialization () ;
  cout << " done." << endl ;
}

void TargetManager::saveConfiguration (const char *filename)
{
  cout << "Saving..." << flush ;

  /* Copy file */
  char cmd[128] ;
  sprintf (cmd, "cp %s %s.bak", filename, filename) ;
  system (cmd) ;

  serialize (filename) ;

  cout << " done." << endl ;
}

void TargetManager::idleFunction ()
{
  // -- read a frame with V4L2
  while (!frameRead()) ; // cout << "ConfigurationWindow::timerFunction() : Failed to read frame with V4L2" << endl ;

  // -- pre-treatment on the image
  if (m_applyContrast)
    applyContrast(v4l2_frame_RGB888) ;
  if (m_applyLuminosity)
    applyLuminosity(v4l2_frame_RGB888) ;

  // -- treat the image
  detectTargets(v4l2_frame_RGB888, m_areaTargetDetection) ;  
}

void TargetManager::serialize (const char *filename)
{
  ofstream out (filename) ;

  if (!out) {
    cerr << "** TargetManager::serialize() Error: failed to open file " << filename << "in write mode." << endl ;
    exit (1) ;
  }

  out << m_capacity << endl ;

  for (unsigned int i=0 ; i<m_capacity ; i++)
    out << m_targets_position[i].x() << " " << m_targets_position[i].y() << endl ;

  out << m_contrast << endl ;
  out << m_luminosity << endl ;
  out << m_applyContrast << endl ;
  out << m_applyLuminosity << endl ;
  out << m_grayScaleDetection << endl ;
  out << m_grayScaleThreshold << endl ;
  out << m_grayScaleInverted << endl ;
  out << m_whiteThreshold << endl ;
  out << m_targetSize << endl ;
  out << m_targetAreaSizeRatio << endl ;
  out << m_areaTargetDetection << endl ;
}

void TargetManager::unserialize (const char *filename)
{
  ifstream in (filename, ios::in) ;

  if (!in) {
    cerr << "** TargetManager::unserialize() Error: failed to open file " << filename << "in read mode." << endl ;
    exit (1) ;
  }

  in >> m_capacity ;

  m_targets_position.clear () ;
  m_targets_state.clear () ;

  int x, y ;
  for (unsigned int i=0 ; i<m_capacity ; i++) {
    in >> x ;
    in >> y ;
    m_targets_position.push_back(QPoint(x, y)) ;
    m_targets_state.push_back(false) ;
  }

  in >> m_contrast ;
  in >> m_luminosity ;
  in >> m_applyContrast ;
  in >> m_applyLuminosity ;
  in >> m_grayScaleDetection ;
  in >> m_grayScaleThreshold ;
  in >> m_grayScaleInverted ;
  in >> m_whiteThreshold ;
  in >> m_targetSize ;
  in >> m_targetAreaSizeRatio ;
  in >> m_areaTargetDetection ;
}

bool TargetManager::addCartouche (Cartouche *cartouche)
{
  if (m_size < m_capacity) {
    //    m_targets_position.push_back(QPoint(0, 0)) ;
    //    m_targets_state.push_back(false) ;
    m_cartouches.push_back(cartouche) ;
    m_size++ ;
    return true ;
  }
  else {
    cerr << "** TargetManager::addCartouche() Warning: number of cartouches goes over capacity" << endl ;
    return false ;
  }
}

void TargetManager::applyContrast (unsigned char* raw_image)
{
  unsigned char *p=raw_image ;
  unsigned int i = m_imageWidth*m_imageHeight ;
  int val ;
  while (i-->0) {

    val = 128 - ((128 - *p) * m_contrast/10.0) ;
    *p = (val>255) ? 255 : ((val<0) ? 0 : val ) ;

    val = 128 - ((128 - *(p+1)) * m_contrast/10.0) ;
    *(p+1) = (val>255) ? 255 : ((val<0) ? 0 : val ) ;

    val = 128 - ((128 - *(p+2)) * m_contrast/10.0) ;
    *(p+2) = (val>255) ? 255 : ((val<0) ? 0 : val ) ;

    p+=3 ;
  }
}

void TargetManager::applyLuminosity (unsigned char* raw_image)
{
  unsigned char *p=raw_image ;
  unsigned int i = m_imageWidth*m_imageHeight ;
  int val ;
  while (i-->0) {

    val = *p * m_luminosity/10.0 ;
    *p = (val>255) ? 255 : val ;

    val = *(p+1) * m_luminosity/10.0 ;
    *(p+1) = (val>255) ? 255 : val ;

    val = *(p+2) * m_luminosity/10.0 ;
    *(p+2) = (val>255) ? 255 : val ;

    p+=3 ;
  }
}


void TargetManager::detectTargets (unsigned char* raw_image, bool detectArea)
{
  int xMin, xMax, yMin, yMax ;
  unsigned char *p, *pA, *pB, *pC, *pD ;
  int minNumPixels = (m_targetSize*m_targetSize)*m_targetAreaSizeRatio*.01 ;
  int numPixels ;
  bool detected ;

  for (unsigned int i=0 ; i<m_size ; i++) {

    int x = m_targets_position[i].x() ;
    int y = m_targets_position[i].y() ;

    if ((xMin = x - m_targetSize*.5) < 0) xMin=0 ;
    if ((xMax = x + m_targetSize*.5) > m_imageWidth-1) xMax=m_imageWidth-1 ;
    if ((yMin = y - m_targetSize*.5) < 0) yMin=0 ;
    if ((yMax = y + m_targetSize*.5) > m_imageHeight-1) yMax=m_imageHeight-1 ;

    p = raw_image+3*(m_targets_position[i].y()*m_imageWidth+m_targets_position[i].x()) ;
    pA = raw_image+3*(yMin*m_imageWidth+xMin) ;
    pB = raw_image+3*(yMax*m_imageWidth+xMin) ;
    pC = raw_image+3*(yMin*m_imageWidth+xMax) ;
    pD = raw_image+3*(yMax*m_imageWidth+xMax) ;
      
    // r = *R ;
    // g = *G ;
    // b = *B ;
    //cout << "(" << r << ", " << g << ", " << b << ")  " ;
    // printf("%d=(%3d, %3d, %3d)  ", i, r, g, b) ;

    if (detectArea) { // Check Target Area
      numPixels=0 ;
      for (x=xMin ; x<=xMax && numPixels<minNumPixels ; x++)
	for (y=yMin ; y<=yMax && numPixels<minNumPixels ; y++)
	  if (checkPixel(p=raw_image+3*(y*m_imageWidth+x)))
	    numPixels++ ;
	
      detected = numPixels>=minNumPixels ;
      if (!m_targets_state[i] && detected)
	m_cartouches[i]->appear() ;
      else if (m_targets_state[i] && !detected)
	m_cartouches[i]->disappear() ;
      m_targets_state[i] = detected ;
    }
    else { // Check five pixels

      if (checkPixel(p) || checkPixel(pA) || checkPixel(pB) || checkPixel(pC) || checkPixel(pD)) {
	if (!m_targets_state[i]) {
	  m_targets_state[i] = true ;
	  cout << "i=" << i << " " ;
	  m_cartouches[i]->appear() ;
	}
      }
      else {
	if (m_targets_state[i]) {
	  m_targets_state[i] = false ;
	  cout << "i=" << i << " " ;
	  m_cartouches[i]->disappear() ;
	}
      }
    }
  }  
}

void TargetManager::processTreatedImageTarget(unsigned char* raw_image, int x, int y)
{
  int xMin, xMax, yMin, yMax ;
  unsigned char *p ;

  if ((xMin = x - m_targetSize*.5 - 2) < 0) xMin=0 ;
  if ((xMax = x + m_targetSize*.5 + 3) > m_imageWidth-1) xMax=m_imageWidth-1 ;
  if ((yMin = y - m_targetSize*.5 - 2) < 0) yMin=0 ;
  if ((yMax = y + m_targetSize*.5 + 3) > m_imageHeight-1) yMax=m_imageHeight-1 ;

  for (x=xMin ; x<=xMax ; x++)
    for (y=yMin ; y<=yMax ; y++)
      if (checkPixel(p=raw_image+3*(y*m_imageWidth+x)))
	*p = *(p+1) = *(p+2) = 0 ;
      else
	*p = *(p+1) = *(p+2) = 200 ;
}

void TargetManager::processTreatedImage(unsigned char* raw_image, bool fullTreatedImage)
{
  if (fullTreatedImage) {
    unsigned char *p=raw_image ;
    unsigned int i = m_imageWidth*m_imageHeight ;
    while (i-->0) {
      if (checkPixel(p)) { // Pixel detected
	*p = *(p+1) = *(p+2) = 0 ;
      }
      else { // Pixel not detected
	*p = *(p+1) = *(p+2) = 200 ;
      }
      p+=3 ;
    }
  }
  else
    for (unsigned int i=0 ; i<m_size ; i++)
      processTreatedImageTarget(raw_image, m_targets_position[i].x(), m_targets_position[i].y()) ;
}

void TargetManager::display ()
{
  for (unsigned int i=0 ; i < m_size ; i++) {
    cout << "(" << m_targets_position[i].x() << ", " << m_targets_position[i].y() << ") " ;
  }
  cout << endl ;
}

