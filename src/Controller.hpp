#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "MediaList.hpp"

#include "ManipulationWindow.hpp"
#include "SelectionWindow.hpp"
#include "ConfigurationWindow.hpp"

#include "Coordinates.hpp"
#include "PolhemusReader.hpp"

class Controller {

public:

  static const unsigned int PING_LIMIT ;
  static const char THUMBNAILS_ALL ;
  static const char THUMBNAILS_MISSING ;

public:

  Controller (SpaceNavigatorServer *SPSRV, MediaList *medias) ;

  ~Controller () ;
  
  void init (char *model1_filename, char *model2_filename) ;
  void start () ;
  void stop () ;
  bool readCoordsFromSpaceNavigators (bool readCoords) ;
  bool readCoordsFromPolhemus () ;
  void idle () ;

  void loadMediaList (const char *dirpath) ;

  void setMediaList (MediaList *medias) ;

  void setMedia (int index) ;

  void setLeftHand (int index) ;

  void setRightHand (int index) ;

  void merge () ;

  void unmerge () ;

  void clutch (int hand) ;

  void declutch (int hand) ;

  void setMesh (int id, const char *mesh_model_filename) ;

  void setObjectPositionX (int id, float val) ;
  void setObjectPositionY (int id, float val) ;
  void setObjectPositionZ (int id, float val) ;
  void setObjectRotationX (int id, float val) ;
  void setObjectRotationY (int id, float val) ;
  void setObjectRotationZ (int id, float val) ;

  void setCameraScale (double val) ;
  void setCameraFOVangle (double val) ;
  void setCameraNear (double val) ;
  void setCameraFar (double val) ;
  void setCameraPosition (double x, double y, double z) ;

  void showAxes1 (bool enable) ;
  void showAxes2 (bool enable) ;

  void cameraFitViewingFrustum () ;
  void cameraNormalize () ;

  void generateMediasScreenshots (char thumbnails_mode) ;

public:

  // -- UI Window 2

  void setSpNavSensitivity (int tr1, int ro1, int tr2, int ro2) ;
  void setPolhemusSensitivity (int tr1, int ro1, int tr2, int ro2) ;
  void resetObjectPosition (char id) ;

  void enableSpaceNavigator (bool enable) ;
  void enablePolhemusLiberty (bool enable) ;

  void enableSpNavTranslate (char id) ;
  void enableSpNavRotate (char id) ;
  void clutchPolhemus (char id) ;

  // -- User Experiments

  void expTrialStart () ;

  void expTrialEnd () ;

  void expTrialNext () ;

private:

  MediaList *m_medias ;

  IrrlichtController *m_irrController ;

  ManipulationWindow m_manipulationWindow ;
  SelectionWindow m_selectionWindow ;
  ConfigurationWindow m_configurationWindow ;

  Coordinates m_coord1 ;
  Coordinates m_coord2 ;

  bool m_enabledSpaceNavigator ;
  bool m_enabledPolhemus ;

  SpaceNavigatorServer *m_SPSRV ;
  bool m_spnav_active ;
  unsigned int m_spnav1_ping_counter ;
  unsigned int m_spnav2_ping_counter ;
  bool m_spnav1_active_translation ;
  bool m_spnav2_active_translation ;
  bool m_spnav1_active_rotation ;
  bool m_spnav2_active_rotation ;
  float m_spnav1_coeff_rot ;
  float m_spnav1_coeff_pos ;
  float m_spnav2_coeff_rot ;
  float m_spnav2_coeff_pos ;
  //Coordinates m_spnav_coord_max ;
  //Coordinates m_spnav_coord_min ;

  PolhemusReader m_polReader ;
  bool m_pol1_clutched ;
  bool m_pol2_clutched ;

  bool m_screenshot ;
  bool m_screenshot_first ;
} ;

#endif /* CONTROLLER_HPP */
