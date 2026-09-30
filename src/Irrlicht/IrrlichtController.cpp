#include <iostream>
#include <stdlib.h>

#include "IrrlichtController.hpp"
#include "AxesSceneNode.hpp"

#include "../Qt/ConfigurationWindow.hpp"

#include "colormod.hpp" // namespace Color
#include "maximum.hpp"
#include "utils.hpp"
#include "stereo.hpp"

const char IrrlichtController::PARENT_THREE_NODES = 0 ;
const char IrrlichtController::PARENT_ROOT_NODE   = 1 ;

const char IrrlichtController::FITTING_NORMAL = 0 ;
const char IrrlichtController::FITTING_CLOSER = 1 ;

IrrlichtController::IrrlichtController (QIrrlichtWidget *irrWidget, ConfigurationWindow *configurationWindow)
  : m_configurationWindow (configurationWindow),
    m_irrWidget (irrWidget),
    m_mesh1 (NULL),
    m_mesh2 (NULL),
    m_mesh3 (NULL),
    m_node1 (NULL),
    m_node2 (NULL),
    m_node3 (NULL),
    m_parentMode (IrrlichtController::PARENT_ROOT_NODE),
    m_updateNode1 (false),
    m_updateNode2 (false),
    m_updateCameraFitting (false),
    m_updateCameraNormalize (false),
    m_fittingMode (IrrlichtController::FITTING_NORMAL),
    m_scale (1.0),
    m_axes1 (false),
    m_axes2 (false),
    m_stereo (false)
{

  m_configurationWindow->setAxes1 (m_axes1) ;
  m_configurationWindow->setAxes2 (m_axes2) ;

  m_device = irrWidget->getIrrlichtDevice() ;

  /*
  m_device = createDevice(video::EDT_SOFTWARE,        // deviceType
                              dimension2d<u32>(640, 480), // windowSize
                              16,         // Bits
                              false,      // fullscreen
                              false,      // stencilbuffer
                              false,      // vsync
                              0);         // IEventReceiver*
  */

  if (!m_device) {
    std::cerr << "[IRRLICHT CONTROLLER] Error while creating device" << std::endl ;
    exit (1) ;
  }

  m_device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");

  m_driver = m_device->getVideoDriver();
  m_smgr = m_device->getSceneManager();
  m_guienv = m_device->getGUIEnvironment();

  //this->guienv->addStaticText(L"Hello World! This is the Irrlicht Software renderer!", rect<s32>(10,10,260,22), true);

  /* AXES (1) */
  if (m_axes1) {
    this->addAxes(m_device,1);
  }

  //m_smgr->addCameraSceneNode(0, vector3df(0, 200, -100), vector3df(0,0,0)); // (0,5,0)
  //m_smgr->addCameraSceneNode(0, vector3df(100, 100, -100), vector3df(0,0,0)); // (0,5,0)

  // Ambiant Light
  //m_smgr->setAmbientLight(video::SColorf(0.3,0.3,0.3,1));

  ILightSceneNode *sun_node;
  SLight sun_data;
  sun_node = m_smgr->addLightSceneNode();
  sun_data.Direction=vector3df(0,0,0);
  sun_data.Type=video::ELT_DIRECTIONAL;
  sun_data.AmbientColor=video::SColorf(0.1f,0.1f,0.1f,1);
  sun_data.SpecularColor=video::SColorf(0.4f,0.4f,0.4f,1);
  sun_data.DiffuseColor=video::SColorf(1.0f,1.0f,1.0f,1);
  sun_data.CastShadows=false;
  sun_node->setLightData(sun_data);
  sun_node->setPosition(vector3df(0,0,0));
  sun_node->setRotation(vector3df(0,0,0));


  // Add three empty nodes in the scene
  // one for each rotation
  // (FOR POLHEMUS NEEDS)
  const core::vector3df axisX(1.0f, 0.0f, 0.0f) ;
  const core::vector3df axisY(0.0f, 1.0f, 0.0f) ;
  const core::vector3df axisZ(0.0f, 0.0f, 1.0f) ;
  core::quaternion qx, qy, qz;
  qx.fromAngleAxis(  90.f * core::DEGTORAD, axisX);
  qy.fromAngleAxis(-180.f * core::DEGTORAD, axisY);
  qz.fromAngleAxis(-180.f * core::DEGTORAD, axisZ);
  // Polhemus 1
  m_node1_rot1 = m_smgr->addEmptySceneNode(m_smgr->getRootSceneNode()) ;
  m_node1_rot2 = m_smgr->addEmptySceneNode(m_node1_rot1) ;
  m_node1_rot3 = m_smgr->addEmptySceneNode(m_node1_rot2) ;
  rotateNodeInWorldSpaceAbsolute (m_node1_rot1, qx.X, qx.Y, qx.Z, qx.W) ;
  rotateNodeInWorldSpaceAbsolute (m_node1_rot2, qy.X, qy.Y, qy.Z, qy.W) ;
  rotateNodeInWorldSpaceAbsolute (m_node1_rot3, qz.X, qz.Y, qz.Z, qz.W) ;
  // Polhemus 2
  m_node2_rot1 = m_smgr->addEmptySceneNode(m_smgr->getRootSceneNode()) ;
  m_node2_rot2 = m_smgr->addEmptySceneNode(m_node2_rot1) ;
  m_node2_rot3 = m_smgr->addEmptySceneNode(m_node2_rot2) ;
  rotateNodeInWorldSpaceAbsolute (m_node2_rot1, qx.X, qx.Y, qx.Z, qx.W) ;
  rotateNodeInWorldSpaceAbsolute (m_node2_rot2, qy.X, qy.Y, qy.Z, qy.W) ;
  rotateNodeInWorldSpaceAbsolute (m_node2_rot3, qz.X, qz.Y, qz.Z, qz.W) ;
}

IrrlichtController::~IrrlichtController ()
{
  std::cerr << "[IRRLICHT CONTROLLER] Closing Irrlicht properly" << std::endl ;
  m_device->drop();
}

void IrrlichtController::setConfigurationWindow (ConfigurationWindow *configurationWindow)
{
  m_configurationWindow = configurationWindow ;
}

void IrrlichtController::removeMesh (IMesh *mesh)
{
  m_smgr->getMeshCache()->removeMesh (mesh) ;
}

void IrrlichtController::removeMesh (int node)
{
  if (node == 1 && m_node1) {
    m_smgr->addToDeletionQueue (m_node1) ;
    this->removeMesh (m_mesh1) ;
    m_node1 = NULL ;
  }
  else if (node == 2 && m_node2) {
    m_smgr->addToDeletionQueue (m_node2) ;
    this->removeMesh (m_mesh2) ;
    m_node2 = NULL ;
  }
}


void IrrlichtController::setMesh (int node, const char *mesh_model_filename)
{

  IAnimatedMesh *mesh = m_smgr->getMesh(mesh_model_filename);

  if (!mesh) {
    m_device->drop();
    exit (1);
  }

  IAnimatedMeshSceneNode *n = m_smgr->addAnimatedMeshSceneNode(mesh) ;

  //ILightSceneNode* light1 = smgr->addLightSceneNode(0, core::vector3df(0,400,-200), video::SColorf(0.3f,0.3f,0.3f), 1.0f, 1 ); 
  //n->setMaterialFlag(EMF_LIGHTING, true); 

  n->getMaterial(0).Shininess = 20.0f ; // Making it looks like metal
  n->getMaterial(0).SpecularColor.set(255,255,255,255);
  n->getMaterial(0).AmbientColor.set(255,255,255,255);
  n->getMaterial(0).DiffuseColor.set(255,255,255,255);
  n->getMaterial(0).EmissiveColor.set(0,0,0,0);
  
  n->getMaterial(0).ColorMaterial = video::ECM_NONE ;

  for (u32 i = 0; i < n->getMaterialCount(); i++) {
    n->getMaterial(i).EmissiveColor = video::SColor(255,150,150,150);
  }

  if (n) {
    n->setPosition(core::vector3df(0,0,0));
    //n->setScale(core::vector3df(200.0, 200.0, 200.0));
    n->setScale(core::vector3df(m_scale, m_scale, m_scale));
    // DOC : enum irr::video::E_MATERIAL_FLAG ===> http://irrlicht.sourceforge.net/docu/namespaceirr_1_1video.html#a8a3bc00ae8137535b9fbc5f40add70d3
    n->setMaterialFlag(EMF_LIGHTING, false);

    /* Fil de fer */
    //n->setMaterialFlag(EMF_WIREFRAME, !n->getMaterial(0).Wireframe);
    //n->setMaterialFlag(video::EMF_POINTCLOUD, false);

    n->setMaterialFlag(EMF_BACK_FACE_CULLING, true) ;
    //n->setMaterialFlag(EMF_FRONT_FACE_CULLING, true); // CAUTION: do not active if driver is EDT_OPENGL

    //n->setMD2Animation(scene::EMAT_ATTACK);
    //n->setMaterialTexture( 0, driver->getTexture("sydney.bmp") );
    //n->setMaterialTexture(0, driver->getTexture("media/purple.bmp") );
  }

  n->getMesh()->getMeshBuffer(0)->recalculateBoundingBox();

  if (node == 1) {
    m_smgr->addToDeletionQueue (m_node1) ;
    m_mesh1 = mesh ;
    m_node1 = n ;
  }
  else { // id == 2
    m_smgr->addToDeletionQueue (m_node2) ;
    m_mesh2 = mesh ;
    m_node2 = n ;
  }

  this->reattachNodesToParent () ;

  std::cout << "IrrlichtController::setMesh() ok9" << std::endl  ;

  this->scaleNormalize () ;

  std::cout << "IrrlichtController::setMesh() FIN" << std::endl  ;
}

void IrrlichtController::initMesh (char *mesh1_model_filename, char *mesh2_model_filename)
{
  std::cout << "IrrlichtController::initMesh() ok1" << std::endl  ;
  this->setMesh (1, mesh1_model_filename) ;
  std::cout << "IrrlichtController::initMesh() ok2" << std::endl  ;
  this->setMesh (2, mesh2_model_filename) ;
  std::cout << "IrrlichtController::initMesh() ok3" << std::endl  ;
}

void IrrlichtController::reattachNodesToParent ()
{
  if (m_parentMode == IrrlichtController::PARENT_THREE_NODES) {
    if (m_node1)
      m_node1->setParent (m_node1_rot3) ;
    if (m_node2)
      m_node2->setParent (m_node2_rot3) ;
  }
  else { // IrrlichtController::PARENT_ROOT_NODE
    if (m_node1)
      m_node1->setParent (m_smgr->getRootSceneNode()) ;
    if (m_node2)
      m_node2->setParent (m_smgr->getRootSceneNode()) ;
  }
}

void IrrlichtController::setParentMode (char parentMode)
{
  m_parentMode = parentMode ;
}

void IrrlichtController::setFittingMode (char fittingMode) {
  if (fittingMode == IrrlichtController::FITTING_NORMAL
      || fittingMode == IrrlichtController::FITTING_CLOSER) {
    m_fittingMode = fittingMode ;
  }
}

void IrrlichtController::merge (bool merge)
{
  if (merge)
    m_node1->addChild (m_node2) ;
  else
    m_node2->setParent(NULL) ;
}

void IrrlichtController::setNodeCoordinates (int node, Coordinates *coord)
{
  if (node == 1) {
    m_updateNode1 = true ;
    m_coord1 = *coord ;
  }
  else if (node == 2) {
    m_updateNode2 = true ;
    m_coord2 = *coord ;
  }
  else {
    std::cerr << "[IRRLICHT CONTROLLER] Error setMeshPositionRotation node null" << std::endl ;
  }
}

void IrrlichtController::askUpdateNode (int node)
{
  if (node == 1) {
    m_updateNode1 = true ;
  }
  else if (node == 2) {
    m_updateNode2 = true ;
  }
}

void IrrlichtController::askUpdateCameraFitting ()
{
  m_updateCameraFitting = true ;
  m_frameCount = 0 ;
}

void IrrlichtController::askNormalizeCamera ()
{
  m_updateCameraNormalize = true ;
}

bool IrrlichtController::idle () {

  if (m_device->run()) {

    //std::cout << "Repaint" << std::endl ;

    /* AXES (2) */
    if (m_axes2) {
      AxesSceneNode* axis = new AxesSceneNode(m_smgr->getRootSceneNode(), m_smgr, -1);
      axis->setAxesScale(1); //  for the length of the axes
      axis->drop();
    }

    if (m_updateCameraNormalize) {
      m_updateCameraNormalize = false ;
      this->scaleNormalize () ;
    }

    if (m_node1 && m_updateNode1) {
      m_updateNode1 = false ;

      if (m_parentMode == IrrlichtController::PARENT_THREE_NODES) {
        // Pohemus Liberty
        //-- Orientation
        // CAUTION !!! Quaternion orders
        // Polhemus : (q0,q1,q2,q3) => q0 + q1*i + q2*j + q3*k => (qr,qi,qj,qk)
        // Irrlicht : (X, Y, Z, W)  => W + X*i + Y*j + Z*k
        rotateNodeInWorldSpaceAbsolute (m_node1, m_coord1.qi, m_coord1.qj, m_coord1.qk, m_coord1.qr) ;
        //-- Position
        m_node1_rot1->setPosition(core::vector3df(m_coord1.px, m_coord1.py, m_coord1.pz));
      }
      else { // IrrlichtController::PARENT_ROOT_NODE
        // SpaceNavigator
        //-- Orientation
        rotateNodeInWorldSpace3 (m_node1, m_coord1.rx, m_coord1.ry, m_coord1.rz) ;
        //-- Position
        m_node1->setPosition(core::vector3df(m_coord1.px, m_coord1.py, m_coord1.pz));
      }

    }

    if (m_node2 && m_updateNode2) {
      m_updateNode2 = false ;

      if (m_parentMode == IrrlichtController::PARENT_THREE_NODES) {
        // Pohemus Liberty
        //-- Orientation
        // CAUTION !!! Quaternion orders
        // Polhemus : (q0,q1,q2,q3) => q0 + q1*i + q2*j + q3*k => (qr,qi,qj,qk)
        // Irrlicht : (X, Y, Z, W)  => W + X*i + Y*j + Z*k
        rotateNodeInWorldSpaceAbsolute (m_node2, m_coord2.qi, m_coord2.qj, m_coord2.qk, m_coord2.qr) ;
        //-- Position
        m_node2_rot1->setPosition(core::vector3df(m_coord2.px, m_coord2.py, m_coord2.pz));
      }
      else { // IrrlichtController::PARENT_ROOT_NODE
        // SpaceNavigator
        //-- Orientation
        rotateNodeInWorldSpace3 (m_node2, m_coord2.rx, m_coord2.ry, m_coord2.rz) ;
        //-- Position
        m_node2->setPosition(core::vector3df(m_coord2.px, m_coord2.py, m_coord2.pz));
      }
    }

    if (m_updateCameraFitting && m_frameCount++ >= 1) {
      m_updateCameraFitting = false ;
      this->moveCameraToFitObjectInsideViewingFrustum () ;
    }

    static video::SColor colBackground (255,100,101,140) ;

    if (m_stereo) {

      
      DrawAnaglyph(
                   PRIMITIVES_OPENGL,
                   m_driver, m_smgr, m_smgr->getActiveCamera(),
                   colBackground,
                   2, // fWidth
                   1000, // fFocus
                   video::EDT_OPENGL,
                   0x000000ff, // ulREyeKey
                   0x00ffff00 // ulLEyeKey
                   );
      

      //DrawAnaglyph3( m_driver, m_smgr, m_smgr->getActiveCamera(), colBackground );
    }
    else {
      //m_driver->beginScene(true, true, SColor(255,100,101,140)); // Deprecated since Irrlicht 1.9
      m_driver->beginScene( video::ECBF_COLOR | video::ECBF_DEPTH, colBackground );
      m_smgr->drawAll();
      //m_guienv->drawAll();
      m_driver->endScene();
    }

    return true ;
  }
  else {
    return false ;
  }

}

void IrrlichtController::showInfo (int node)
{
  //  SViewFrustum* frustrum ;

  if (node == 1) {
    //frustrum = m_node1->getViewFrustum() ;
  }
  else { // node == 2
    //frustrum = m_node2->getViewFrustum() ;
  }
}

void IrrlichtController::setZoom (int i)
{
  float dist = i / 10.f ;

  ICameraSceneNode *cam = m_smgr->getActiveCamera() ;
  
  if (cam != 0) {
    //cam->setAspectRatio((f32)widgetSize.Height / (f32)widgetSize.Width);
    core::vector3df pos = cam->getPosition () ;
    pos.Z = dist ;
    cam->setPosition (pos) ;
    //std::cout << "setZoom() : New camera position (" << pos.X << ", " << pos.Y << ", " << pos.Z << ")" << std::endl ;
    m_configurationWindow->displayCameraPosition (pos.X, pos.Y, pos.Z) ;
  }

}

void IrrlichtController::setCameraScale (float scale)
{
  m_scale = (scale > 0 ? scale : -scale) ;

  m_configurationWindow->displayCameraScale (m_scale) ;

  if (m_node1) {
    m_node1->setScale(core::vector3df(m_scale, m_scale, m_scale));
  }

  if (m_node2) {
    m_node2->setScale(core::vector3df(m_scale, m_scale, m_scale));
  }
  
}

void IrrlichtController::setCameraFOVangle (float val)
{
  ICameraSceneNode *cam = m_smgr->getActiveCamera() ;
  if (cam != 0) {
    cam->setFOV (val) ;
    updateDisplay () ;
  }
}

void IrrlichtController::setCameraNear (float val)
{
  ICameraSceneNode *cam = m_smgr->getActiveCamera() ;
  if (cam != 0) {
    cam->setNearValue (val) ;
    updateDisplay () ;
  }
}

void IrrlichtController::setCameraFar (float val)
{
  ICameraSceneNode *cam = m_smgr->getActiveCamera() ;
  if (cam != 0) {
    cam->setFarValue (val) ;
    updateDisplay () ;
  }
}

void IrrlichtController::setCameraPosition (double x, double y, double z)
{
  ICameraSceneNode *cam = m_smgr->getActiveCamera() ;
  if (cam != 0) {
    cam->setPosition (core::vector3df (x, y, z)) ;
  }
  updateDisplay() ;
}


void IrrlichtController::showAxes1 (bool enable)
{
  m_axes1 = enable ;
}

void IrrlichtController::showAxes2 (bool enable)
{
  m_axes2 = enable ;
}

void IrrlichtController::enableStereoAnaglyph (bool enable)
{
  m_stereo = enable ;
}

void IrrlichtController::updateDisplay ()
{
  ICameraSceneNode *cam = m_smgr->getActiveCamera() ;

  /* PB : Segmentation error !!!!!!!!!!!! */
  //std::cout << "updateDisplay() === Ok1" << std::endl ;
  //m_configurationWindow->displayCameraScale(m_scale) ;
  //std::cout << "updateDisplay() === Ok2" << std::endl ;

  if (cam != 0) {
    core::vector3df pos = cam->getPosition () ;
    const SViewFrustum* frustum = cam->getViewFrustum() ;

    m_configurationWindow->displayCameraFOVangle(cam->getFOV()) ;
    m_configurationWindow->displayCameraNear(cam->getNearValue()) ;
    m_configurationWindow->displayCameraFar(cam->getFarValue()) ;
    m_configurationWindow->displayCameraFrustum(frustum->getNearLeftDown().Z, frustum->getFarLeftDown().Z) ;
    m_configurationWindow->displayCameraPosition(pos.X, pos.Y, pos.Z) ;
  }

  //m_configurationWindow.displayObject1Coordinates (&m_coord1) ;
  //m_configurationWindow.displayObject2Coordinates (&m_coord2) ;
}

/*
 Aide : 
 http://stackoverflow.com/questions/2866350/move-camera-to-fit-3d-scene
 http://irrlicht.sourceforge.net/forum/viewtopic.php?p=193161
 http://gamedev.stackexchange.com/questions/79834/fit-a-bounding-box-in-the-scene-modifying-fov

              frustum      ------            
                     ------    *****          -  
                -----          *   *          |
            -===     ) FOV a   *bounding box  | BB size s
         camera -----          *   *          |
                     ------    *****          -
                           ------
         
           |-------------------|
                 distance d


     tan (a/2) = (s/2) / d    <=>     d = (s/2) / tan(a/2)

 */
void IrrlichtController::moveCameraToFitObjectInsideViewingFrustum ()
{
  ICameraSceneNode *cam = m_smgr->getActiveCamera() ;
  float d_h, d_l, d, a = cam->getFOV () ; /* The field of view of the camera in radians */
  core::aabbox3df bb ;

  std::cout << "scale is x" << m_scale << std::endl ;

  if (m_node1 != NULL) {
    //m_node1->recalculateBoundingBox () ;
    m_node1->getMesh()->getMeshBuffer(0)->recalculateBoundingBox();
    core::aabbox3df bb1 = m_node1->getTransformedBoundingBox () ;
    bb.addInternalBox (bb1) ;

    /*
         /3--------/7
        / |       / |
       /  |      /  |          Y _  __
      1---------5   |           /|\  /| Z
      |  /2- - -|- -6            |  /
      | /       |  /             | /
      |/        | /              |/
      0---------4/               -----------> X
    */
    std::cout << Color::FG_BLUE << std::endl ;
    core::vector3df edges1[8] ;
    bb1.getEdges (edges1) ;
    std::cout << "Min X " << edges1[0].X << std::endl ;
    std::cout << "Max X " << edges1[4].X << std::endl ;
    std::cout << "Min Y " << edges1[0].Y << std::endl ;
    std::cout << "Max Y " << edges1[1].Y << std::endl ;
    std::cout << Color::FG_DEFAULT << std::endl ;
  }

  if (m_node2 != NULL) {
    //m_node2->recalculateBoundingBox () ;
    m_node2->getMesh()->getMeshBuffer(0)->recalculateBoundingBox();
    core::aabbox3df bb2 = m_node2->getTransformedBoundingBox () ;
    bb.addInternalBox (bb2) ;
  }

  if (m_node1 != NULL || m_node2 != NULL) {

    /*
      Edges are stored in this way:
      Hey, am I an ascii artist, or what? :) niko.
         /3--------/7
        / |       / |
       /  |      /  |          Y _  __
      1---------5   |           /|\  /| Z
      |  /2- - -|- -6            |  /
      | /       |  /             | /
      |/        | /              |/
      0---------4/               -----------> X
    */

    /*
    core::vector3df edges[8] ;
    bb.getEdges (edges) ;

    float h = fabs (edges1[1].Y - edges1[0].Y) ;
    float l = fabs (edges1[4].X - edges1[0].X) ;
    */

    std::cout << Color::FG_GREEN << std::endl ;
    core::vector3df edges[8] ;
    bb.getEdges (edges) ;
    std::cout << "Min X " << edges[0].X << std::endl ;
    std::cout << "Max X " << edges[4].X << std::endl ;
    std::cout << "Min Y " << edges[0].Y << std::endl ;
    std::cout << "Max Y " << edges[1].Y << std::endl ;
    std::cout << "Min Z " << edges[0].Z << std::endl ;
    std::cout << "Max Z " << edges[2].Z << std::endl ;
    std::cout << Color::FG_DEFAULT << std::endl ;

    core::vector3df extent = bb.getExtent() ;
    float h = extent.Y ;
    float l = extent.X ;
    float p = extent.Z ;

    std::cout << Color::FG_MAGENTA << std::endl ;
    std::cout << "h=" << h << " l=" << l << " p=" << p << std::endl ;
    std::cout << Color::FG_DEFAULT << std::endl ;

    d_h = (h / 2.f) / tan (a / 2.f) ;
    d_l = (l / 2.f) / tan (a / 2.f) ;

    d = (d_h > d_l ? d_h : d_l) ; // Take the longest distance of the camera needed to fit the entire object in the screen

    /*
    std::cout << "d_h=" << d_h << std::endl ;
    std::cout << "d_l=" << d_l << std::endl ;
    std::cout << "d=" << d << std::endl ;
    */
    
    if (d < cam->getNearValue()) {
      std::cout << Color::FG_RED << "Cutting d to Camera Near Value " << cam->getNearValue() << Color::FG_DEFAULT << std::endl ;
      d = cam->getNearValue() ;
    }

    if (d > cam->getFarValue()) {
      std::cout << Color::FG_RED << "Cutting d to Camera Far Value " << cam->getFarValue() << Color::FG_DEFAULT  << std::endl ;
      d = cam->getFarValue() ;
    }
    
    core::vector3df pos = cam->getPosition () ;

    //std::cout  << "AVANT " ;
    //m_irrWidget->printCameraInfo () ;

    float bbCenterX = (edges[0].X + edges[4].X) / 2.f ;
    float bbCenterY = (edges[0].Y + edges[1].Y) / 2.f ;
    float dist = d + p ; // FITTING_CLOSER

    if (m_fittingMode == IrrlichtController::FITTING_NORMAL) {
      dist += cam->getNearValue() ;
    }

    pos.X = bbCenterX ;
    pos.Y = bbCenterY ;
    pos.Z = -dist ; // For screenshots remove + cam->getNearValue()
    cam->setPosition (pos) ;

    //m_configurationWindow->displayCameraPosition (pos.X, pos.Y, pos.Z) ;

    //std::cout  << "APRES " ;
    //m_irrWidget->printCameraInfo () ;

    cam->setTarget(vector3df(bbCenterX, bbCenterY, 0));

    updateDisplay () ;

    this->idle() ;
  }
}

void IrrlichtController::scaleNormalize ()
{
  const float sizeNormalized = 10.f ; // Normalized size
  float factor1=0.f, factor2=0.f ;

  std::cout << "IrrlichtController::scaleNormalize() BEGIN" << std::endl ;

  if (m_node1 != NULL) {
    m_node1->getMesh()->getMeshBuffer(0)->recalculateBoundingBox();
    core::aabbox3df bb1 = m_node1->getTransformedBoundingBox () ;
    core::vector3df extent1 = bb1.getExtent() ;
    factor1 = sizeNormalized / maximum (extent1.X, extent1.Y, extent1.Z) ;
  }

  if (m_node2 != NULL) {
    m_node2->getMesh()->getMeshBuffer(0)->recalculateBoundingBox();
    core::aabbox3df bb2 = m_node2->getTransformedBoundingBox () ;
    core::vector3df extent2 = bb2.getExtent() ;
    factor2 = sizeNormalized / maximum (extent2.X, extent2.Y, extent2.Z) ;
  }

  if (m_node1 != NULL || m_node2 != NULL) {
    setCameraScale (maximum(factor1, factor2)) ;
  }

  std::cout << "IrrlichtController::scaleNormalize() END" << std::endl ;
}

// http://irrlicht3d.org/wiki/index.php?n=Main.TakingAScreenShot
void IrrlichtController::takeScreenshot(const char *filename_png) 
{ 
  //irr::video::IVideoDriver* const driver = device->getVideoDriver(); 

   //get image from the last rendered frame 
   irr::video::IImage* const image = m_driver->createScreenShot(); 

   if (image) //should always be true, but you never know. ;) 
   { 
      // Construct a filename, consisting of local time and file extension 
      //irr::c8 filename[64]; 
      //snprintf(filename, 64, "screenshot_%u.png", device->getTimer()->getRealTime()); 

      //write screenshot to file 
      if (!m_driver->writeImageToFile(image, filename_png)) 
         m_device->getLogger()->log(L"Failed to take screenshot.", irr::ELL_WARNING); 

      //Don't forget to drop image since we don't need it anymore. 
      image->drop(); 
   } 
}

void IrrlichtController::addAxes(IrrlichtDevice *device, float scale)
{
  ISceneNode *node;
  IGUIFont   *font=device->getGUIEnvironment()->getFont("font.png");

  // X : Rouge
  node=device->getSceneManager()->addMeshSceneNode(device->getSceneManager()->addArrowMesh("AddAxesMeshX",SColor(255,255,0,0),SColor(255,255,0,0),4,8,scale,scale*0.8f,scale*0.02f,scale*0.05f));
  node->setMaterialFlag(EMF_NORMALIZE_NORMALS,true);
  node->setMaterialFlag(EMF_LIGHTING,false);
  node->setMaterialFlag(EMF_GOURAUD_SHADING,true);
  node->setRotation(vector3df(0,0,-90));
  device->getSceneManager()->addTextSceneNode(font,L"+X",SColor(255,255,255,255),0,vector3df(scale,0,0));
  
  // Y : Vert
  node=device->getSceneManager()->addMeshSceneNode(device->getSceneManager()->addArrowMesh("AddAxesMeshY",SColor(255,0,255,0),SColor(255,0,255,0),4,8,scale,scale*0.8f,scale*0.02f,scale*0.05f));
  node->setMaterialFlag(EMF_NORMALIZE_NORMALS,true);
  node->setMaterialFlag(EMF_LIGHTING,false);
  node->setMaterialFlag(EMF_GOURAUD_SHADING,true);
  node->setRotation(vector3df(0,0,0));
  device->getSceneManager()->addTextSceneNode(font,L"+Y",SColor(255,255,255,255),0,vector3df(0,scale,0));

  // Z : Bleu
  node=device->getSceneManager()->addMeshSceneNode(device->getSceneManager()->addArrowMesh("AddAxesMeshZ",SColor(255,0,0,255),SColor(255,0,0,255),4,8,scale,scale*0.8f,scale*0.02f,scale*0.05f));
  node->setMaterialFlag(EMF_NORMALIZE_NORMALS,true);
  node->setMaterialFlag(EMF_LIGHTING,false);
  node->setMaterialFlag(EMF_GOURAUD_SHADING,true);
  node->setRotation(vector3df(90,0,0));
  device->getSceneManager()->addTextSceneNode(font,L"+Z",SColor(255,255,255,255),0,vector3df(0,0,scale));
}
