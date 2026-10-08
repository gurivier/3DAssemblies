
/**
 * To rotate / revolve node in world space & in local space
 * Postby smso » Wed Dec 26, 2012 4:03 pm 
 * http://irrlicht.sourceforge.net/forum/viewtopic.php?f=9&t=47924
 */

#include "utils.hpp"

/* Author: SMSO */
void rotateNodeInWorldSpace(scene::ISceneNode* node, f32 degs, const core::vector3df& axis)
{
  core::quaternion q;
  q.fromAngleAxis(degs*core::DEGTORAD, axis);
  core::matrix4 m1 = q.getMatrix();
 
  node->updateAbsolutePosition();
  core::matrix4 m2 = node->getAbsoluteTransformation();
   
  core::matrix4 m = m1*m2;
  node->setRotation(m.getRotationDegrees()); 
}

/* Author: GR */
void rotateNodeInWorldSpaceAbsolute(scene::ISceneNode* node, f32 qX, f32 qY, f32 qZ, f32 qW)
{
  node->setRotation(core::quaternion(qX,qY,qZ,qW).getMatrix().getRotationDegrees()); 
}

/* Author: GR */
void rotateNodeInWorldSpace3(scene::ISceneNode* node, f32 degsX, f32 degsY, f32 degsZ)
{
  static core::quaternion qx, qy, qz;
  static const core::vector3df axisX(1.0f, 0.0f, 0.0f) ;
  static const core::vector3df axisY(0.0f, 1.0f, 0.0f) ;
  static const core::vector3df axisZ(0.0f, 0.0f, 1.0f) ;

  qx.fromAngleAxis(degsX*core::DEGTORAD, axisX);
  qy.fromAngleAxis(degsY*core::DEGTORAD, axisY);
  qz.fromAngleAxis(degsZ*core::DEGTORAD, axisZ);

  core::matrix4 m1x = qx.getMatrix();
  core::matrix4 m1y = qy.getMatrix();
  core::matrix4 m1z = qz.getMatrix();
  
  node->updateAbsolutePosition();
  core::matrix4 m2 = node->getAbsoluteTransformation();
  
  core::matrix4 m = m1x * m1y * m1z * m2 ;
  node->setRotation(m.getRotationDegrees());
}

/* Author: SMSO */
void rotateNodeInLocalSpace(scene::ISceneNode* node, f32 degs, const core::vector3df& axis)
{
    node->updateAbsolutePosition();
    core::matrix4 m2 = node->getAbsoluteTransformation();
    core::vector3df a = axis;
    m2.rotateVect(a);
    a.normalize();
 
    core::quaternion q;
    q.fromAngleAxis(degs*core::DEGTORAD, a);
    core::matrix4 m1 = q.getMatrix();
 
    core::matrix4 m = m1*m2;
    node->setRotation(m.getRotationDegrees());
}

/* Author: SMSO */
//both axis and pivot are in world space
void revolveNodeInWorldSpace(scene::ISceneNode* node, f32 degs, const core::vector3df& axis, const core::vector3df& pivot)
{
    node->updateAbsolutePosition();
    core::vector3df p1 = node->getAbsolutePosition();
    core::vector3df p2 = getClosestPointOnLine(axis, pivot, p1);
    core::vector3df vect = p1 - p2;
   
    core::quaternion q;
    q.fromAngleAxis(degs*core::DEGTORAD, axis);
    q.getMatrix().rotateVect(vect);
   
    node->setPosition(p2 + vect);
}

/* Author: SMSO */
//both axis and pivot are in local space
void revolveNodeInLocalSpace(scene::ISceneNode* node, f32 degs, const core::vector3df& axis, const core::vector3df& pivot)
{
    moveNodeInLocalSpace(node, pivot);
    rotateNodeInLocalSpace(node, degs, axis);
    moveNodeInLocalSpace(node, -pivot);
}

/* Author: SMSO */
//axis is in local space and pivot in world space
void revolveNodeAboutLocalAxis(scene::ISceneNode* node, f32 degs, const core::vector3df& axis, const core::vector3df& pivot)
{
    node->updateAbsolutePosition();
    core::matrix4 m = node->getAbsoluteTransformation();
    core::vector3df a = axis;
    m.rotateVect(a);
    a.normalize();
 
    core::vector3df p1 = node->getAbsolutePosition();
    core::vector3df p2 = getClosestPointOnLine(a, pivot, p1);
    core::vector3df vect = p1 - p2;
 
    core::quaternion q;
    q.fromAngleAxis(degs*core::DEGTORAD, a);
    q.getMatrix().rotateVect(vect);
   
 
    node->setPosition(p2 + vect);
   
}

/* Author: SMSO */
void moveNodeInLocalSpace(scene::ISceneNode* node, const core::vector3df& distVect)
{
    node->updateAbsolutePosition();
    core::matrix4 m = node->getAbsoluteTransformation();
    core::vector3df d = distVect;
    m.rotateVect(d);
       
    core::vector3df pos = node->getAbsolutePosition() + d;
    node->setPosition(pos);
}

/* Author: SMSO */
void moveNodeInLocalSpace(scene::ISceneNode* node, const core::vector3df& dir, f32 dist)
{
    node->updateAbsolutePosition();
    core::matrix4 m = node->getAbsoluteTransformation();
    core::vector3df d = dir;
    m.rotateVect(d);
    d.normalize();
   
    core::vector3df pos = node->getAbsolutePosition() + d * dist;
    node->setPosition(pos);
}

/* Author: SMSO */
// the line is defined by axis direction passing through the pivot
// 3rd argument "point" is the external point
core::vector3df getClosestPointOnLine(const core::vector3df& axis, const core::vector3df& pivot, const core::vector3df& point)
{
    core::vector3df c = point - pivot;
    f32 t = axis.dotProduct(c);
    return pivot + axis*t;
}


