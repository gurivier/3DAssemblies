
#include <iostream>

#include "Coordinates.hpp"

Coordinates::Coordinates ()
: px (0.f),
  py (0.f),
  pz (0.f),
  rx (0.f),
  ry (0.f),
  rz (0.f)
{
  // Nothing
}

Coordinates::Coordinates (float px, float py, float pz,
                float rx, float ry, float rz)
: px (px),
  py (py),
  pz (pz),
  rx (rx),
  ry (ry),
  rz (rz)
{
  // Nothing
}

Coordinates::~Coordinates ()
{
  // Nothing
}

void Coordinates::reset ()
{
  px = py = pz = rx = ry = rz = 0.0f ;
}

void Coordinates::resetPosition ()
{
  px = py = pz = 0.0f ;
}

void Coordinates::resetRotation ()
{
  rx = ry = rz = 0.0f ;
}

void Coordinates::display () 
{
  display (false);
}

void Coordinates::display (bool quaternion) 
{
  if (!quaternion) {
    std::cout << "P(x=" << px
              << ", \ty=" << py
              << ", \tz=" << pz
              << ")\t\t"
              << "R(x=" << rx
              << ", \ty=" << ry
              << ", \tz=" << rz
              << ")" << std::endl ;
  }
  else {
    std::cout << "P(x=" << px
              << ", \ty=" << py
              << ", \tz=" << pz
              << ")\t\t"
              << "Q(qr=" << qr
              << ", \tqi=" << qi
              << ", \tqj=" << qj
              << ", \tqk=" << qk
              << ")" << std::endl ;
  }
}

void Coordinates::maximumPosition (Coordinates *coord)
{
  if (px < coord->px)
    px = coord->px ;

  if (py < coord->py)
    py = coord->py ;

  if (pz < coord->pz)
    pz = coord->pz ;
}

void Coordinates::maximumRotation (Coordinates *coord)
{
  if (rx < coord->rx)
    rx = coord->rx ;

  if (ry < coord->ry)
    ry = coord->ry ;

  if (rz < coord->rz)
    rz = coord->rz ;
}

void Coordinates::minimumPosition (Coordinates *coord)
{
  if (px > coord->px)
    px = coord->px ;

  if (py > coord->py)
    py = coord->py ;

  if (pz > coord->pz)
    pz = coord->pz ;
}

void Coordinates::minimumRotation (Coordinates *coord)
{
  if (rx > coord->rx)
    rx = coord->rx ;

  if (ry > coord->ry)
    ry = coord->ry ;

  if (rz > coord->rz)
    rz = coord->rz ;
}
