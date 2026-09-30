
#include <iostream>
#include <stdio.h> // sprintf
#include <unistd.h> // access

#include "Controller.hpp"

#include "../Coordinates.hpp"

const unsigned int Controller::PING_LIMIT = 400 ;
const char Controller::THUMBNAILS_ALL = 0 ;
const char Controller::THUMBNAILS_MISSING = 1 ;

Controller::Controller (SpaceNavigatorServer *SPSRV, MediaList *medias)
  : m_medias (medias),
    m_irrController (NULL),
    m_manipulationWindow (this),
    m_selectionWindow (NULL, this, medias), 
    m_configurationWindow (NULL, this),
    m_coord1 (0.0, 0.0, 0.0, 0.0, 0.0, 0.0),
    m_coord2 (0.0,  0.0, 0.0, 0.0, 0.0, 0.0),
    m_enabledSpaceNavigator (true),
    m_enabledPolhemus (true),
    m_SPSRV (SPSRV),
    m_spnav1_ping_counter (0),
    m_spnav2_ping_counter (0),
    m_spnav1_active_translation (true),
    m_spnav2_active_translation (true),
    m_spnav1_active_rotation (true),
    m_spnav2_active_rotation (true),
    m_spnav1_coeff_rot (1.0f),
    m_spnav1_coeff_pos (1.0f),
    m_spnav2_coeff_rot (1.0f),
    m_spnav2_coeff_pos (1.0f),
    //m_spnav_coord_max (0.0, 0.0, 0.0, 0.0, 0.0, 0.0),
    //m_spnav_coord_min (0.0, 0.0, 0.0, 0.0, 0.0, 0.0),
    m_polReader (),
    m_pol1_clutched (false),
    m_pol2_clutched (false)
{
  m_irrController = new IrrlichtController (m_manipulationWindow.getIrrlichtWidget(), &m_configurationWindow) ;

  m_manipulationWindow.setIrrlichtController (m_irrController) ;
  m_configurationWindow.setIrrlichtController (m_irrController) ;

  m_configurationWindow.setSpNavSensitivity (1, 2, 4) ; // Device, Translation, Rotation
  m_configurationWindow.setSpNavSensitivity (2, 2, 4) ; // Device, Translation, Rotation

  m_configurationWindow.displaySpNavTranslate (1, m_spnav1_active_translation) ;
  m_configurationWindow.displaySpNavTranslate (2, m_spnav2_active_translation) ;
  m_configurationWindow.displaySpNavRotate (1, m_spnav1_active_rotation) ;
  m_configurationWindow.displaySpNavRotate (2, m_spnav2_active_rotation) ;

  m_polReader.init() ;

  std::cout << "Controller() === Ok1" << std::endl ;
  m_irrController->updateDisplay () ;
  std::cout << "Controller() === Ok2" << std::endl ;

  //m_configurationWindow.setVisible (false) ;
  //m_selectionWindow.setVisible (false) ;
  
}

Controller::~Controller ()
{
  m_polReader.stop() ;

  //std::cout << "Controller::~Controller()" << std::endl ;
  //std::cout << "SpNav Maximum : " ;
  //m_spnav_coord_max.display () ;
  //std::cout << "SpNav Minimum : " ;
  //m_spnav_coord_min.display () ;
}

void Controller::init (char *model1_filename, char *model2_filename)
{
  m_irrController->initMesh (model1_filename, model2_filename) ;
}

void Controller::start ()
{
  m_manipulationWindow.timerStart(5) ; // interval in milliseconds
  m_manipulationWindow.show();
  m_configurationWindow.show() ;
  m_selectionWindow.show();
}

void Controller::stop ()
{
  m_manipulationWindow.timerStop() ;
}

bool Controller::readCoordsFromSpaceNavigators (bool readCoords)
{
  bool posChanged = false ;
  bool continuer = true ;
  int i, max_it=50 ;

  /* Considerer (nombre d'iterations depuis le dernier ping) si la connexion
     avec le client 1 qui ecoute un SpaceNavigator est active */
  if (m_spnav1_ping_counter++ > Controller::PING_LIMIT) { 
    m_configurationWindow.displaySpNavConnected (1, false) ;
  }

  /* Considerer (nombre d'iterations depuis le dernier ping) si la connexion
     avec le client 2 qui ecoute un SpaceNavigator est active */
  if (m_spnav2_ping_counter++ > Controller::PING_LIMIT) {
    m_configurationWindow.displaySpNavConnected (2, false) ;
  }

  /* Lire au plus max_it messages dans la file du SpaceNavigator */
  for (i=0 ; i < max_it && continuer ; i++) {

    /* Lire le message suivant dans la file du SpaceNavigator */
    if ((continuer = m_SPSRV->idle ())) {

      //std::cout << "sp1 px=" << m_SPSRV->sp1.coord.px << " sp2 px=" << m_SPSRV->sp2.coord.px << (m_SPSRV->sp1.ping ? " ping1" : "") << (m_SPSRV->sp2.ping ? " ping2" : "") << std::endl ;

      if (m_SPSRV->sp1.ping) {
        m_SPSRV->sp1.ping = false ;
        m_spnav1_ping_counter = 0 ;
        m_configurationWindow.displaySpNavConnected (1, true) ;
      }

      if (m_SPSRV->sp2.ping) {
        m_SPSRV->sp2.ping = false ;
        m_spnav2_ping_counter = 0 ;
        m_configurationWindow.displaySpNavConnected (2, true) ;
      }

      if (readCoords) {

        if (m_SPSRV->sp1.read) {
          m_SPSRV->sp1.read = false ;

          //std::cout << "sp1 read " << m_SPSRV->sp1.coord.px << " " << m_SPSRV->sp1.coord.py << " " << m_SPSRV->sp1.coord.pz << std::endl ;

          if (m_spnav1_active_translation) {
            //m_spnav_coord_max.maximumPosition(&m_SPSRV->sp1.coord) ;
            //m_spnav_coord_min.minimumPosition(&m_SPSRV->sp1.coord) ;
            m_coord1.addPosition (&(m_SPSRV->sp1.coord), m_spnav1_coeff_pos) ;
            posChanged = true ;
          }

          if (m_spnav1_active_rotation) {
            //m_spnav_coord_max.maximumRotation(&m_SPSRV->sp1.coord) ;
            //m_spnav_coord_min.minimumRotation(&m_SPSRV->sp1.coord) ;
            m_coord1.addRotation (&(m_SPSRV->sp1.coord), -m_spnav1_coeff_rot) ;
            posChanged = true ;
          }

        }

        if (m_SPSRV->sp2.read) {
          m_SPSRV->sp2.read = false ;

          //std::cout << "sp2 read " << m_SPSRV->sp2.px << " " << m_SPSRV->sp2.py << " " << m_SPSRV->sp2.pz << std::endl ;

          if (m_spnav2_active_translation) {
            //m_spnav_coord_max.maximumPosition(&m_SPSRV->sp2.coord) ;
            //m_spnav_coord_min.minimumPosition(&m_SPSRV->sp2.coord) ;
            m_coord2.addPosition (&m_SPSRV->sp2.coord, m_spnav2_coeff_pos) ;
            posChanged = true ;
          }

          if (m_spnav2_active_rotation) {
            //m_spnav_coord_max.maximumRotation(&m_SPSRV->sp2.coord) ;
            //m_spnav_coord_min.minimumRotation(&m_SPSRV->sp2.coord) ;
            m_coord2.addRotation (&m_SPSRV->sp2.coord, -m_spnav2_coeff_rot) ;
            posChanged = true ;
          }

        }
      } /* end if readCoords */

      if (m_SPSRV->sp1.leftButtonChanged) {
        m_SPSRV->sp1.leftButtonChanged = false ;
        continuer = false ;

        if (m_SPSRV->sp1.leftButtonPosition == SPNAV_RELEASED) {
          this->enableSpNavTranslate (1) ;
        }
      }
      if (m_SPSRV->sp1.rightButtonChanged) {
        m_SPSRV->sp1.rightButtonChanged = false ;
        continuer = false ;

        if (m_SPSRV->sp1.rightButtonPosition == SPNAV_RELEASED) {
          this->enableSpNavRotate (1) ;
        }
      }

      if (m_SPSRV->sp2.leftButtonChanged) {
        m_SPSRV->sp2.leftButtonChanged = false ;
        continuer = false ;

        if (m_SPSRV->sp2.leftButtonPosition == SPNAV_RELEASED) {
          this->enableSpNavTranslate (2) ;
        }
      }

      if (m_SPSRV->sp2.rightButtonChanged) {
        m_SPSRV->sp2.rightButtonChanged = false ;
        continuer = false ;

        if (m_SPSRV->sp2.rightButtonPosition == SPNAV_RELEASED) {
          this->enableSpNavRotate (2) ;
        }
      }

    }
  } /* End for */

  if (i == max_it) {
    std::cerr << std::endl << std::endl << "[MAIN] ** INFORMATION : buffer limit was temporarly reached ** " << std::endl << std::endl ;
  }

  return posChanged ;
}

bool Controller::readCoordsFromPolhemus ()
{
  bool posChanged = false ;
  m_pol1_clutched = true ;
  m_pol2_clutched = true ;
  float coef = 1.0f ;

  if (m_polReader.idle()) {

    if (m_pol1_clutched && m_polReader.pol1.read) {
      m_polReader.pol1.read = false ;

      //-- Position
      m_coord1.setPosition (&(m_polReader.pol1.coord), coef) ;

      //-- Orientation
      m_coord1.setQuaternion (&(m_polReader.pol1.coord)) ;

      //m_polReader.pol1.coord.display();
      posChanged = true ;
    }

    if (m_pol2_clutched && m_polReader.pol2.read) {
      m_polReader.pol2.read = false ;

      //-- Position
      m_coord2.setPosition (&(m_polReader.pol2.coord), coef) ;

      //-- Orientation
      m_coord2.setQuaternion (&(m_polReader.pol2.coord)) ;

      //m_polReader.pol2.coord.display();
      posChanged = true ;
    }

  }

  return posChanged ;
}

void Controller::idle ()
{
  static bool first = true ;
  bool posChanged=first ;

  //m_configurationWindow.setVisible (false) ;
  //m_selectionWindow.setVisible (false) ;


  if (first) {
    first = false ;
    //setObjectPosition (1, m_coord1.x, m_coord1.y, m_coord1.z) ;
    //setObjectPosition (2, m_coord2.x, m_coord2.y, m_coord2.z) ;
    m_irrController->setNodeCoordinates (1, &m_coord1) ;
    m_irrController->setNodeCoordinates (2, &m_coord2) ;
  }

  // std::cout << "SP1 " << m_SPSRV->sp1.coord.px << " " << m_SPSRV->sp1.coord.py << " " << m_SPSRV->sp1.coord.pz << std::endl ;

  
  if (m_irrController->idle()) {      

    /* Remettre rx=0, ry=0 et rz=0 (pour que ne soit applique dans Irrlicht que la nouvelle rotation
     * relative locale relativement a l'orientation courante */
    m_coord1.resetRotation () ;
    m_coord2.resetRotation () ;

    /* Modifier m_coord1 et m_coord2, en lisant les coordonnees fournies par les SpaceNavigator */ 
    posChanged = posChanged || this->readCoordsFromSpaceNavigators (m_enabledSpaceNavigator) ;

    /* Modifier m_coord1 et m_coord2, en lisant les coordonnees fournies par les Polhemus */ 
    if (m_enabledPolhemus && m_polReader.isConnected()) {
      posChanged = posChanged || this->readCoordsFromPolhemus () ;
    }

    /* Update View */
    if (posChanged) {
      //m_coord1.display() ;
      //m_coord2.display() ;
      m_configurationWindow.displayObject1Coordinates (&m_coord1) ;
      m_configurationWindow.displayObject2Coordinates (&m_coord2) ;
      m_irrController->setNodeCoordinates (1, &m_coord1) ;
      m_irrController->setNodeCoordinates (2, &m_coord2) ;
    }
  }
}

//void Controller::setInputMode ()
//{
//
//}

void Controller::setMesh (int id, const char *mesh_model_filename)
{
  m_irrController->setMesh (id, mesh_model_filename) ;
  //m_irrController->moveCameraToFitObjectInsideViewingFrustum () ;
  m_irrController->askUpdateCameraFitting () ;
}

void Controller::setSpNavSensitivity (int tr1, int ro1, int tr2, int ro2)
{
  //std::cout << "Controller::setSpNavSensitivity() : [" << tr1 << "; " << ro1 << "] [" << tr2 << "; " << ro2 << "]" << std::endl ;

  m_spnav1_coeff_pos = tr1 / 2.0f ;
  m_spnav1_coeff_rot = ro1 / 2.0f ;
  m_spnav2_coeff_pos = tr2 / 2.0f ;
  m_spnav2_coeff_rot = ro2 / 2.0f ;

  //std::cout << "  coef =>[" << m_spnav1_coeff_pos << "; " << m_spnav1_coeff_rot << "] [" << m_spnav2_coeff_pos << "; " << m_spnav2_coeff_rot << "]" << std::endl ;

}

void Controller::setPolhemusSensitivity (int tr1, int ro1, int tr2, int ro2)
{
  std::cout << "Controller::setPolhemusSensitivity() : [" << tr1 << "; " << ro1 << "] [" << tr2 << "; " << ro2 << "]" << std::endl ;
}

void Controller::setObjectPositionX (int id, float val)
{
  if (id == 1) {
    m_coord1.px = val ;
    m_irrController->setNodeCoordinates (1, &m_coord1) ;
    //m_configurationWindow.displayObject1Coordinates (&m_coord1) ;
  }
  else { // id==2
    m_coord2.px = val ;
    m_irrController->setNodeCoordinates (2, &m_coord2) ;
    //m_configurationWindow.displayObject2Coordinates (&m_coord2) ;
  }
}

void Controller::setObjectPositionY (int id, float val)
{
  if (id == 1) {
    m_coord1.py = val ;
    m_irrController->setNodeCoordinates (1, &m_coord1) ;
    //m_configurationWindow.displayObject1Coordinates (&m_coord1) ;
  }
  else { // id==2
    m_coord2.py = val ;
    m_irrController->setNodeCoordinates (2, &m_coord2) ;
    //m_configurationWindow.displayObject2Coordinates (&m_coord2) ;
  }
}

void Controller::setObjectPositionZ (int id, float val)
{
  if (id == 1) {
    m_coord1.pz = val ;
    m_irrController->setNodeCoordinates (1, &m_coord1) ;
    //m_configurationWindow.displayObject1Coordinates (&m_coord1) ;
  }
  else { // id==2
    m_coord2.pz = val ;
    m_irrController->setNodeCoordinates (2, &m_coord2) ;
    //m_configurationWindow.displayObject2Coordinates (&m_coord2) ;
  }
}

void Controller::setObjectRotationX (int id, float val)
{
  if (id == 1) {
    m_coord1.rx = val ;
    m_irrController->setNodeCoordinates (1, &m_coord1) ;
    //m_configurationWindow.displayObject1Coordinates (&m_coord1) ;
  }
  else { // id==2
    m_coord2.rx = val ;
    m_irrController->setNodeCoordinates (2, &m_coord2) ;
    //m_configurationWindow.displayObject2Coordinates (&m_coord2) ;
  }
}

void Controller::setObjectRotationY (int id, float val)
{
  if (id == 1) {
    m_coord1.ry = val ;
    m_irrController->setNodeCoordinates (1, &m_coord1) ;
    //m_configurationWindow.displayObject1Coordinates (&m_coord1) ;
  }
  else { // id==2
    m_coord2.ry = val ;
    m_irrController->setNodeCoordinates (2, &m_coord2) ;
    //m_configurationWindow.displayObject2Coordinates (&m_coord2) ;
  }
}

void Controller::setObjectRotationZ (int id, float val)
{
  if (id == 1) {
    m_coord1.rz = val ;
    m_irrController->setNodeCoordinates (1, &m_coord1) ;
    //m_configurationWindow.displayObject1Coordinates (&m_coord1) ;
  }
  else { // id==2
    m_coord2.rz = val ;
    m_irrController->setNodeCoordinates (2, &m_coord2) ;
    //m_configurationWindow.displayObject2Coordinates (&m_coord2) ;
  }
}

void Controller::setCameraScale (double val)
{
  m_irrController->setCameraScale (val) ;
  m_configurationWindow.displayCameraScale (val) ;
}

void Controller::setCameraFOVangle (double val)
{
  m_irrController->setCameraFOVangle (val) ;
  m_configurationWindow.displayCameraFOVangle (val) ;  
}

void Controller::setCameraNear (double val)
{
  m_irrController->setCameraNear (val) ;
  m_configurationWindow.displayCameraNear (val) ;    
}

void Controller::setCameraFar (double val)
{
  m_irrController->setCameraFar (val) ;
  m_configurationWindow.displayCameraFar (val) ;  
}

void Controller::setCameraPosition (double x, double y, double z)
{
  m_irrController->setCameraPosition (x, y, z) ;
}

void Controller::enableSpaceNavigator (bool enable)
{
  m_enabledSpaceNavigator = enable ;
}

void Controller::enablePolhemusLiberty (bool enable)
{
  m_enabledPolhemus = enable ;
}

void Controller::resetObjectPosition (char id)
{
  if (id == 1) {
    m_coord1.resetPosition () ;
    m_irrController->setNodeCoordinates (1, &m_coord1) ;
  }
  else { // id == 2
    m_coord2.resetPosition () ;
    m_irrController->setNodeCoordinates (2, &m_coord2) ;
  }
}

void Controller::enableSpNavTranslate (char id)
{
  if (id == 1) {
    m_spnav1_active_translation = !m_spnav1_active_translation ;
    m_configurationWindow.displaySpNavTranslate (1, m_spnav1_active_translation) ;
  }
  else { // id == 2
    m_spnav2_active_translation = !m_spnav2_active_translation ;
    m_configurationWindow.displaySpNavTranslate (2, m_spnav2_active_translation) ;
  }
}

void Controller::enableSpNavRotate (char id)
{
  if (id == 1) {
    m_spnav1_active_rotation = !m_spnav1_active_rotation ;
    m_configurationWindow.displaySpNavRotate (1, m_spnav1_active_rotation) ;
  }
  else { // id == 2
    m_spnav2_active_rotation = !m_spnav2_active_rotation ;
    m_configurationWindow.displaySpNavRotate (2, m_spnav2_active_rotation) ;
  }
}

void Controller::clutchPolhemus (char id)
{
  id = id ; // To avoid compiltation warning
}

void Controller::showAxes1 (bool enable)
{
  m_irrController->showAxes1 (enable) ;
}

void Controller::showAxes2 (bool enable)
{
  m_irrController->showAxes2 (enable) ;
}

void Controller::cameraFitViewingFrustum ()
{
  m_irrController->askUpdateCameraFitting () ;
}

void Controller::cameraNormalize ()
{
  m_irrController->askNormalizeCamera () ;
}

void Controller::generateMediasScreenshots (char thumbnails_mode)
{
  bool generateAll = (thumbnails_mode == Controller::THUMBNAILS_ALL) ;
  char file_png[512] = "" ;
  bool removed = false ;
  m_irrController->setFittingMode (IrrlichtController::FITTING_CLOSER) ;

  for (int i=0 ; i < m_medias->size() ; i++) {
    for (int j=0 ; j < m_medias->getElement(i).size () ; j++) {

      // Filename
      sprintf (file_png, "%s.png", m_medias->getElement(i).getElementFullName(j)) ;
      bool fileExists = (access(file_png, F_OK) != -1) ;

      // Generate all files or if file don't exist
      if (generateAll || !fileExists) {

        if (!removed) {
          m_irrController->removeMesh (1) ;
          m_irrController->removeMesh (2) ;
        }

        // Set mesh
        this->setMesh (1, m_medias->getElement(i).getElementFullName(j)) ;

        // Idle
        m_irrController->idle() ;

        // Adjust camera
        m_irrController->askUpdateCameraFitting () ;

        // Idle
        m_irrController->idle() ;

        // Take Screenshot
        std::cout << "Taking screenshot for " << file_png << std::endl ;
        m_irrController->takeScreenshot (file_png) ;
        std::cout << "Taking screenshot Done"  << std::endl ;
      }
    }
  }

  m_irrController->setFittingMode (IrrlichtController::FITTING_NORMAL) ;
}
