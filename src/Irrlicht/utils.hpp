#ifndef UTILS_HPP
#define UTILS_HPP

#include <irrlicht/irrlicht.h>

using namespace irr;

void rotateNodeInWorldSpace(scene::ISceneNode* node, f32 degs, const core::vector3df& axis) ;
void rotateNodeInWorldSpaceAbsolute(scene::ISceneNode* node, f32 q0, f32 q1, f32 q2, f32 q3) ;
void rotateNodeInWorldSpace3(scene::ISceneNode* node, f32 degsX, f32 degsY, f32 degsZ) ;

void rotateNodeInLocalSpace(scene::ISceneNode* node, f32 degs, const core::vector3df& axis) ;

void revolveNodeInWorldSpace(scene::ISceneNode* node, f32 degs, const core::vector3df& axis, const core::vector3df& pivot) ;
void revolveNodeInLocalSpace(scene::ISceneNode* node, f32 degs, const core::vector3df& axis, const core::vector3df& pivot) ;
void revolveNodeAboutLocalAxis(scene::ISceneNode* node, f32 degs, const core::vector3df& axis, const core::vector3df& pivot) ;

void moveNodeInLocalSpace(scene::ISceneNode* node, const core::vector3df& distVect) ;
void moveNodeInLocalSpace(scene::ISceneNode* node, const core::vector3df& dir, f32 dist) ;

core::vector3df getClosestPointOnLine(const core::vector3df& axis, const core::vector3df& pivot, const core::vector3df& point) ;

#endif /* UTILS_HPP */
