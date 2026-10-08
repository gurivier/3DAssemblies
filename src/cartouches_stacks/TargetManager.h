#ifndef TARGETMANAGER_H
#define TARGETMANAGER_H

#include <vector>
#include <QPoint>

#include "Cartouche.h"

using namespace std;

class AssemblyWindow ;
class ConfigurationWindow ;

class TargetManager {

 public:

  //=========================
  //== CONSTRUCTORS public
  //=========================

  TargetManager (unsigned int capacity, int imageWidth, int imageHeight) ;

  TargetManager (const char *filename, int imageWidth, int imageHeight) ;

 public:

  //=========================
  //== ACCESSORS public
  //=========================

  inline void setAssemblyWindow (AssemblyWindow *assemblyWindow) { m_assemblyWindow = assemblyWindow ; }
  inline void setConfigurationWindow (ConfigurationWindow *configurationWindow) { m_configurationWindow = configurationWindow ; }

  inline void   setTargetPosition (int pos, QPoint target) { m_targets_position[pos] = target ; }
  inline QPoint getTargetPosition (int pos) { return m_targets_position[pos] ; }
  inline bool   getTargetState (int pos) { return m_targets_state[pos] ; }

  inline int capacity() { return m_capacity ; }
  inline int size() { return m_size ; }

  inline void setGrayScaleDetection (bool enable) { m_grayScaleDetection = enable ; }
  inline bool getGrayScaleDetection () { return m_grayScaleDetection ; }

  inline void setGrayScaleThreshold (int threshold) { m_grayScaleThreshold = threshold ; }
  inline int  getGrayScaleThreshold () { return m_grayScaleThreshold ; }

  inline void setGrayScaleInverted (bool checked) { m_grayScaleInverted = checked ; }
  inline bool getGrayScaleInverted () { return m_grayScaleInverted ; }

  inline void setGreenDetection (bool enable) { m_grayScaleDetection = !enable ; }
  inline bool getGreenDetection () { return !m_grayScaleDetection ; }

  inline void setWhiteThreshold (int threshold) { m_whiteThreshold = threshold - 100 ; }
  inline int  getWhiteThreshold () { return m_whiteThreshold + 100 ; }

  inline void setTargetSize(int size) { m_targetSize = size ; }
  inline int  getTargetSize() { return m_targetSize ; }

  inline int getImageWidth () { return m_imageWidth ; }
  inline int getImageHeight () { return m_imageHeight ; }

  inline void setContrast (int contrast) { m_contrast = (double)contrast ; }
  inline int  getContrast () { return (int)m_contrast ; }

  inline void setLuminosity (int luminosity) { m_luminosity = (double)luminosity ; }
  inline int  getLuminosity () { return (int)m_luminosity ; }

  inline void setApplyContrast (bool apply) { m_applyContrast = apply ; } ;
  inline bool getApplyContrast () { return m_applyContrast ; } ;

  inline void setApplyLuminosity (bool apply) { m_applyLuminosity = apply ; } ;
  inline bool getApplyLuminosity () { return m_applyLuminosity ; } ;

  inline void setTargetDetectionAreaSize (int sizeRatio) { m_targetAreaSizeRatio = sizeRatio ; }
  inline int  getTargetDetectionAreaSize () { return m_targetAreaSizeRatio ; }

  inline void setTargetAreaDetection (bool detectArea) { m_areaTargetDetection = detectArea ; }
  inline bool getTargetAreaDetection () { return m_areaTargetDetection ; }


 public:

  //=========================
  //== METHODS public
  //=========================

  void quit () ;

  void loadConfiguration (const char *filename) ;

  void saveConfiguration (const char *filename) ;

  void idleFunction () ;

  void serialize (const char *filename) ;

  void unserialize (const char *filename) ;

  bool addCartouche (Cartouche *cartouche) ;

  void applyContrast (unsigned char* raw_image) ;

  void applyLuminosity (unsigned char* raw_image) ;

  void detectTargets (unsigned char* raw_image, bool detectArea) ;

  void processTreatedImageTarget(unsigned char* raw_image, int x, int y) ;

  void processTreatedImage(unsigned char* raw_image, bool fullTreatedImage) ;

  void display () ;
  
  inline bool checkPixel(unsigned char *p)
  {

    if (m_grayScaleDetection) {

//--------------
//    if (!m_grayScaleInverted)
//      return *p < m_grayScaleThreshold   // Red
//	&& *(p+1) < *p+10       // Blue
//	&& *(p+2) < *p+20 ;      // Green
//    else
//      return *p > m_grayScaleThreshold   // Red
//	&& *(p+1) > *p-10       // Blue
//	&& *(p+2) > *p-20 ;      // Green
//--------------

//--------------
//    if (!m_grayScaleInverted)
//      return *p < m_grayScaleThreshold    // Red
//	&& *(p+1) < m_grayScaleThreshold  // Blue
//	&& *(p+2) < m_grayScaleThreshold ; // Green
//    else
//      return *p > m_grayScaleThreshold    // Red
//	&& *(p+1) > m_grayScaleThreshold  // Blue
//	&& *(p+2) > m_grayScaleThreshold ; // Green
//--------------

//--------------
      if (!m_grayScaleInverted)
	return *p + *(p+1) + *(p+2) < m_grayScaleThreshold+m_grayScaleThreshold+m_grayScaleThreshold ;
      else
	return *p + *(p+1) + *(p+2) > m_grayScaleThreshold+m_grayScaleThreshold+m_grayScaleThreshold ;
//--------------

    }
    else { // Green detection
      return *(p+1)>70 && *(p+1)-m_whiteThreshold > *p && *(p+1)-m_whiteThreshold*.5 > *(p+2) ;
    }
  }


 private:

  //=========================
  //== MEMBERS private
  //=========================

  AssemblyWindow      *m_assemblyWindow ;
  ConfigurationWindow *m_configurationWindow ;

  unsigned int m_size ;
  unsigned int m_capacity ;

  vector<QPoint>     m_targets_position ;
  vector<bool>       m_targets_state ;
  vector<Cartouche*> m_cartouches ;

  int m_imageWidth ;
  int m_imageHeight ;

  double m_contrast ;
  double m_luminosity ;
  bool   m_applyContrast ;
  bool   m_applyLuminosity ;

  bool m_grayScaleDetection ;
  int  m_grayScaleThreshold ;
  bool m_grayScaleInverted ;
  int  m_whiteThreshold ;

  int  m_targetSize ;
  int  m_targetAreaSizeRatio ;
  bool m_areaTargetDetection ;
} ;

#endif /* TargetManager */


