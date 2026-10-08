#ifndef IRRLICHTCONTROLLER_HPP
#define IRRLICHTCONTROLLER_HPP

#include <irrlicht/irrlicht.h>
#include "QIrrlichtWidget.hpp"
#include "../Coordinates.hpp"


using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class ConfigurationWindow ;

class IrrlichtController {

public:

  static const char PARENT_THREE_NODES ; // Nodes are attached to 3 empty nodes and rotated by quaternion
  static const char PARENT_ROOT_NODE ;   // Nodes are attached to root node and rotated by three angles

  static const char FITTING_NORMAL ;
  static const char FITTING_CLOSER ;

public:

  IrrlichtController (QIrrlichtWidget *irrWidget, ConfigurationWindow *configurationWindow) ;
  ~IrrlichtController () ;

  void setConfigurationWindow (ConfigurationWindow *configurationWindow) ;

  void removeMesh (IMesh *mesh) ;
  void removeMesh (int node) ;
  void setMesh (int node, const char *mesh_model_filename) ;
  void initMesh (char *mesh1_model_filename, char *mesh2_model_filename) ;
  void reattachNodesToParent () ;
  void setParentMode (char parentMode) ;
  void setFittingMode (char fittingMode) ;
  void merge (bool merge) ;
  bool idle () ;
  //void setNodePositionRotation (int node, float px, float py, float pz, float rx, float ry, float rz) ;
  void setNodeCoordinates (int node, Coordinates *coord) ;
  void askUpdateNode (int node) ;
  void askUpdateCameraFitting () ;
  void askNormalizeCamera () ;

  void showInfo (int node) ;

  void setZoom (int i) ;

  void setCameraScale (float val) ;
  void setCameraFOVangle (float val) ;
  void setCameraNear (float val) ;
  void setCameraFar (float val) ;
  void setCameraPosition (double x, double y, double z) ;

  void showAxes1 (bool enable) ;
  void showAxes2 (bool enable) ;
  void enableStereoAnaglyph (bool enable) ;

  void updateDisplay () ;

  void moveCameraToFitObjectInsideViewingFrustum () ;

  void scaleNormalize () ;

  void takeScreenshot(const char *filename_png) ;

private:

  void addAxes (IrrlichtDevice *device, float scale) ;

private:

  ConfigurationWindow *m_configurationWindow ;

  QIrrlichtWidget *m_irrWidget ;

  IrrlichtDevice  *m_device ;
  IVideoDriver    *m_driver ;
  ISceneManager   *m_smgr ;
  IGUIEnvironment *m_guienv ;

  IAnimatedMesh *m_mesh1, *m_mesh2, *m_mesh3 ;
  IAnimatedMeshSceneNode *m_node1, *m_node2, *m_node3 ;

  char m_parentMode ;
  ISceneNode *m_node1_rot1, *m_node1_rot2, *m_node1_rot3 ;
  ISceneNode *m_node2_rot1, *m_node2_rot2, *m_node2_rot3 ;

  Coordinates m_coord1, m_coord2 ;

  bool m_updateNode1, m_updateNode2 ;
  bool m_updateCameraFitting ;
  bool m_updateCameraNormalize ;

  int m_frameCount ;
  char m_fittingMode ;


  float m_scale ;  

  bool m_axes1, m_axes2 ;
  bool m_stereo ;
} ;

#endif /* IRRLICHTCONTROLLER_HPP */
