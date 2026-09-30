#ifndef STEREO_HPP
#define STEREO_HPP

#include <irrlicht/irrlicht.h>

enum EPrimitives { PRIMITIVES_IRRLICHT , PRIMITIVES_OPENGL } ;

bool DrawAnaglyph( enum EPrimitives primitives,
                   irr::video::IVideoDriver *pDriver, 
                   irr::scene::ISceneManager *pSm, 
                   irr::scene::ICameraSceneNode *pCamera, 
                   irr::video::SColor colBackground, 
                   float fWidth, float fFocus, int nDriverType, 
                   unsigned long ulREyeKey, unsigned long ulLEyeKey ) ;

void DrawAnaglyph3 (
                   irr::video::IVideoDriver *pDriver, 
                   irr::scene::ISceneManager *pSm, 
                   irr::scene::ICameraSceneNode *pCamera, 
                   irr::video::SColor colBackground
                    );

#endif /* STEREO_HPP */
