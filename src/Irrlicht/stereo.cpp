/* File: stereo.cpp

Red/Cyan anaglyph stereovision using Irrlicht engine under Linux width video::EDT_OPENGL driver.

Directly inspired by:
http://irrlicht.sourceforge.net/forum/viewtopic.php?p=185215

And for Irrlich primitives by
http://irrlicht.sourceforge.net/forum/viewtopic.php?f=9&t=33463
http://irrlicht3d.org/wiki/index.php?n=Main.AnaglyphRendering

COMPILATION:
$ g++ -o irrsteroexample main.cpp stereo.cpp -I/usr/include/irrlicht/ -I/usr/include/GL/ -lIrrlicht -lGL

DOWNLOAD MEDIA NEEDED FOR RUNTIME:
$ mkdir ./media/
$ cd ./media/
$ wget https://github.com/zaki/irrlicht/blob/master/media/map-20kdm2.pk3
$ wget https://raw.githubusercontent.com/freeminer/irrlicht/master/media/sydney.bmp
$ wget https://raw.githubusercontent.com/freeminer/irrlicht/master/media/sydney.md2

USAGE:
$ ./irrsteroexample quake
$ ./irrsteroexample sydney

*/

#include <irrlicht/irrlicht.h>
#include <iostream>
#include <GL/gl.h>

#include "stereo.hpp"

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

/********************
 * Functions to prepare left and right eyes
 * and then restore when finished
 * with OPENGL primitives.
 ****/

void prepareLeftEye_OPENGL (unsigned long ulLEyeKey) {
  glClear( GL_DEPTH_BUFFER_BIT ); 
  glMatrixMode( GL_MODELVIEW ); 
  glLoadIdentity(); 
  glColorMask( 0 != ( ulLEyeKey & 0x00ff0000 ), 
               0 != ( ulLEyeKey & 0x0000ff00 ), 
               0 != ( ulLEyeKey & 0x000000ff ), 
               0 != ( ulLEyeKey & 0xff000000 ) ); 
}

void prepareRightEye_OPENGL (unsigned long ulREyeKey) {
  glMatrixMode( GL_MODELVIEW ); 
  glLoadIdentity(); 
  glColorMask( 0 != ( ulREyeKey & 0x00ff0000 ), 
               0 != ( ulREyeKey & 0x0000ff00 ), 
               0 != ( ulREyeKey & 0x000000ff ), 
               0 != ( ulREyeKey & 0xff000000 ) ); 
}

void restore_OPENGL () {
  glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ); 
}

/********************
 * Functions to prepare left and right eyes
 * and then restore when finished
 * with IRRLICHT primitives.
 ****/

void prepareRightEye_IRRLICHT (irr::video::IVideoDriver *pDriver) {
  pDriver->getOverrideMaterial().Material.ColorMask=ECP_GREEN+ECP_BLUE; // ECP_GREEN+ECP_BLUE
  pDriver->getOverrideMaterial().EnableFlags=EMF_COLOR_MASK;
  pDriver->getOverrideMaterial().EnablePasses=
    ESNRP_SKY_BOX
    +ESNRP_SOLID
    +ESNRP_TRANSPARENT
    +ESNRP_TRANSPARENT_EFFECT
    +ESNRP_SHADOW;
}

void prepareLeftEye_IRRLICHT (irr::video::IVideoDriver *pDriver) {
  pDriver->getOverrideMaterial().Material.ColorMask=ECP_RED;
  pDriver->getOverrideMaterial().EnableFlags=EMF_COLOR_MASK;
  pDriver->getOverrideMaterial().EnablePasses=
    ESNRP_SKY_BOX
    +ESNRP_SOLID
    +ESNRP_TRANSPARENT
    +ESNRP_TRANSPARENT_EFFECT
    +ESNRP_SHADOW;
}

void restore_IRRLICHT (irr::video::IVideoDriver *pDriver) {
  pDriver->getOverrideMaterial().Material.ColorMask=ECP_ALL;
  pDriver->getOverrideMaterial().EnableFlags=0;
  pDriver->getOverrideMaterial().EnablePasses=0;
}

bool DrawAnaglyph( enum EPrimitives primitives,
                   irr::video::IVideoDriver *pDriver, 
                   irr::scene::ISceneManager *pSm, 
                   irr::scene::ICameraSceneNode *pCamera, 
                   irr::video::SColor colBackground, 
                   float fWidth, float fFocus, int nDriverType, 
                   unsigned long ulREyeKey, unsigned long ulLEyeKey ) 
{ 
  // Right eye 
  irr::core::vector3df reye = pCamera->getPosition(); 

  // Left eye 
  irr::core::vector3df v( fWidth, 0, 0 ); 
  irr::core::matrix4 m; 
  m.setRotationDegrees( pCamera->getRotation() + irr::core::vector3df( 90.f, 0, 0 ) ); 
  m.transformVect( v ); 
  irr::core::vector3df leye = reye + v; 

  if (primitives == PRIMITIVES_OPENGL) {
    // Setup right eye 
    prepareRightEye_OPENGL (ulREyeKey) ;
  }

  //pDriver->beginScene( true, true, colBackground ); // Deprecated from Irrlicht 1.9
  pDriver->beginScene( video::ECBF_COLOR | video::ECBF_DEPTH, colBackground );

  if (primitives == PRIMITIVES_IRRLICHT) {
    // Setup right eye 
    prepareRightEye_IRRLICHT (pDriver) ;
  }

  // Tried here (but with no success) some code from: http://irrlicht.sourceforge.net/forum/viewtopic.php?t=28893
  //pDriver->setRenderTarget (video::ERT_STEREO_BOTH_BUFFERS, true, true, video::SColor(0, 0, 0, 1)) ;
  //pDriver->setRenderTarget (video::ERT_STEREO_RIGHT_BUFFER, true, true, video::SColor(0, 0, 0, 1)) ;

  // Eye target 
  irr::core::vector3df oldt = pCamera->getTarget(); 
  irr::core::vector3df eyet = ( oldt - reye ).normalize() * fFocus; 
  pCamera->setTarget( eyet ); 

  // pDriver->clearZBuffer(); // Deprecated from Irrlicht 1.9
  pDriver->clearBuffers(video::ECBF_DEPTH, colBackground);

  pSm->drawAll();  // right eye

  if (primitives == PRIMITIVES_OPENGL) {
    // Setup left Eye 
    prepareLeftEye_OPENGL (ulLEyeKey) ;
  }

  // pDriver->clearZBuffer(); // Deprecated from Irrlicht 1.9
  pDriver->clearBuffers(video::ECBF_DEPTH, colBackground);

  if (primitives == PRIMITIVES_IRRLICHT) {
    // Setup left Eye 
    prepareLeftEye_IRRLICHT (pDriver) ;
  }

  pCamera->setPosition( leye ); 
  pCamera->OnRegisterSceneNode(); 

  // Tried here (but with no success) some code from: http://irrlicht.sourceforge.net/forum/viewtopic.php?t=28893
  //pDriver->setRenderTarget (video::ERT_STEREO_LEFT_BUFFER, true, true, video::SColor(0, 0, 0, 1)) ;

  pSm->drawAll(); // left eye

  pDriver->endScene(); 

  if (primitives == PRIMITIVES_OPENGL) {
    restore_OPENGL () ;
  }
  else if (primitives == PRIMITIVES_IRRLICHT) {
    restore_IRRLICHT (pDriver) ;
  }

  // Restore original position 
  pCamera->setPosition( reye ); 
  pCamera->setTarget( oldt ); 

  return true; 
} 


void DrawAnaglyph3 (
                   irr::video::IVideoDriver *pDriver, 
                   irr::scene::ISceneManager *pSm, 
                   irr::scene::ICameraSceneNode *pCamera, 
                   irr::video::SColor colBackground
)
{
   vector3df oldPosition=pCamera->getPosition();
   vector3df oldTarget=pCamera->getTarget();
   
   pCamera->updateAbsolutePosition(); //== Added this in Irrlicht 1.8
   matrix4 startMatrix=pCamera->getAbsoluteTransformation();

   vector3df focusPoint=(pCamera->getTarget()-pCamera->getAbsolutePosition()).setLength(1) + pCamera->getAbsolutePosition() ;

   //Left eye move...
   vector3df leftEye;
   matrix4 leftMove;
   leftMove.setTranslation( vector3df(-0.01f,0.0f,0.0f) );
   leftEye=(startMatrix*leftMove).getTranslation();
   u8 leftMask = ECP_RED ;

   //Right eye move...
   vector3df rightEye;
   matrix4 rightMove;
   rightMove.setTranslation( vector3df(0.01f,0.0f,0.0f) );
   rightEye=(startMatrix*rightMove).getTranslation();
   u8 rightMask=ECP_GREEN+ECP_BLUE ;

   //clear the depth buffer, and color
   //pDriver->beginScene( true, true, colBackground ); // Deprecated from Irrlicht 1.9
   pDriver->beginScene( video::ECBF_COLOR | video::ECBF_DEPTH, colBackground );

   //Left eye...

   pCamera->setPosition( leftEye );
   pCamera->setTarget( focusPoint );

   pDriver->getOverrideMaterial().Material.ColorMask=leftMask;
   pDriver->getOverrideMaterial().EnableFlags=EMF_COLOR_MASK ;
   pDriver->getOverrideMaterial().EnablePasses=ESNRP_SKY_BOX+ESNRP_SOLID+ESNRP_TRANSPARENT+ESNRP_TRANSPARENT_EFFECT+ESNRP_SHADOW;

   pSm->drawAll();
   
   //clear the depth buffer
   //pDriver->clearZBuffer(); // Deprecated from Irrlicht 1.9
   pDriver->clearBuffers(video::ECBF_DEPTH, colBackground);

   //Right eye...

   pCamera->setPosition( rightEye );
   pCamera->setTarget( focusPoint );

   pDriver->getOverrideMaterial().Material.ColorMask=rightMask;
   pDriver->getOverrideMaterial().EnableFlags=EMF_COLOR_MASK ;
   pDriver->getOverrideMaterial().EnablePasses=ESNRP_SKY_BOX+ESNRP_SOLID+ESNRP_TRANSPARENT+ESNRP_TRANSPARENT_EFFECT+ESNRP_SHADOW;

   pSm->drawAll();

   pDriver->endScene();

   pDriver->getOverrideMaterial().Material.ColorMask=ECP_ALL;
   pDriver->getOverrideMaterial().EnableFlags=0;
   pDriver->getOverrideMaterial().EnablePasses=0;

   pCamera->setPosition( oldPosition );
   pCamera->setTarget( oldTarget );

}


