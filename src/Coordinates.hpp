#ifndef COORDINATES_HPP
#define COORDINATES_HPP

#include <math.h>

class Coordinates {

public:

  float px, py, pz ;
  float rx, ry, rz ;
  float qr, qi, qj, qk ;

public:

  Coordinates () ;

  Coordinates (float px, float py, float pz,
               float rx, float ry, float rz) ;

  ~Coordinates () ;

  void reset () ;
  void resetPosition () ;
  void resetRotation () ;

  inline void set (float px, float py, float pz,
                   float rx, float ry, float rz)
  {
    this->px = px ;
    this->py = py ;
    this->pz = pz ;

    this->rx = rx ;
    this->ry = ry ;
    this->rz = rz ;
  }

  inline void set (Coordinates *coord)
  {
    this->px = coord->px ;
    this->py = coord->py ;
    this->pz = coord->pz ;

    this->rx = coord->rx ;
    this->ry = coord->ry ;
    this->rz = coord->rz ;
  }

  inline void set (Coordinates *coord, float coef)
  {
    this->px = coord->px * coef ;
    this->py = coord->py * coef ;
    this->pz = coord->pz * coef ;

    this->rx = coord->rx * coef ;
    this->ry = coord->ry * coef ;
    this->rz = coord->rz * coef ;
  }

  inline void setPosition (float px, float py, float pz)
  {
    this->px = px ;
    this->py = py ;
    this->pz = pz ;
  }

  inline void setRotation (float rx, float ry, float rz)
  {
    this->rx = rx ;
    this->ry = ry ;
    this->rz = rz ;
  }

  inline void setQuaternion (float qr, float qi, float qj, float qk)
  {
    this->qr = qr ;
    this->qi = qi ;
    this->qj = qj ;
    this->qk = qk ;
  }

  inline void add (Coordinates *coord, float coef)
  {
    this->px += coord->px * coef ;
    this->py += coord->py * coef ;
    this->pz += coord->pz * coef ;
    this->rx += coord->rx * coef ;
    this->ry += coord->ry * coef ;
    this->rz += coord->rz * coef ;
  }

  inline void setPosition (Coordinates *coord)
  {
    this->px = coord->px ;
    this->py = coord->py ;
    this->pz = coord->pz ;
  }

  inline void setPosition (Coordinates *coord, float coef)
  {
    this->px = coord->px * coef ;
    this->py = coord->py * coef ;
    this->pz = coord->pz * coef ;
  }

  inline void setRotation (Coordinates *coord)
  {
    this->rx = coord->rx ;
    this->ry = coord->ry ;
    this->rz = coord->rz ;
  }

  inline void setRotation (Coordinates *coord, float coef)
  {
    this->rx = coord->rx * coef ;
    this->ry = coord->ry * coef ;
    this->rz = coord->rz * coef ;
  }

  inline void setQuaternion (Coordinates *coord)
  {
    this->qr = coord->qr ;
    this->qi = coord->qi ;
    this->qj = coord->qj ;
    this->qk = coord->qk ;
  }

  inline void addPosition (Coordinates *coord, float coef)
  {
    this->px += coord->px * coef ;
    this->py += coord->py * coef ;
    this->pz += coord->pz * coef ;
  }

  inline void addRotation (Coordinates *coord, float coef)
  {
    this->rx += coord->rx * coef ;
    this->ry += coord->ry * coef ;
    this->rz += coord->rz * coef ;
  }

  void display () ;
  void display (bool quaternion) ;

  void maximumPosition (Coordinates *coord) ;
  void maximumRotation (Coordinates *coord) ;
  void minimumPosition (Coordinates *coord) ;
  void minimumRotation (Coordinates *coord) ;

  inline bool equals (Coordinates *coord)
  {
    return px == coord->px && py == coord->py && pz == coord->pz
      && rx == coord->rx && ry == coord->ry && rz == coord->rz ;
  }

  inline bool equalsPosition (Coordinates *coord)
  {
    return px == coord->px && py == coord->py && pz == coord->pz ;
  }

  inline bool equalsRotation (Coordinates *coord)
  {
    return rx == coord->rx && ry == coord->ry && rz == coord->rz ;
  }

  inline bool equalsEpsilon (Coordinates *coord, float eplison)
  {
    return fabs(px - coord->px) < eplison
      && fabs(py - coord->py) < eplison
      && fabs(pz - coord->pz) < eplison
      && fabs(rx - coord->rx) < eplison
      && fabs(ry - coord->ry) < eplison
      && fabs(rz - coord->rz) < eplison ;
  }

  inline bool equalsPositionEpsilon (Coordinates *coord, float eplison)
  {
    return fabs(px - coord->px) < eplison
      && fabs(py - coord->py) < eplison
      && fabs(pz - coord->pz) < eplison ;
  }

  inline bool equalsRotationEpsilon (Coordinates *coord, float eplison)
  {
    return fabs(rx - coord->rx) < eplison
      && fabs(ry - coord->ry) < eplison
      && fabs(rz - coord->rz) < eplison ;
  }

} ;

#endif /* COORDINATES_HPP */
