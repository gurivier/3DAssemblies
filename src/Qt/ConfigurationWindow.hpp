#ifndef WINDOW2_HPP
#define WINDOW2_HPP

#include <QtWidgets/QMainWindow>

#include "ui_ConfigurationWindowBase.h"

#include "Coordinates.hpp"

#include "IrrlichtController.hpp"

class Controller ;

class ConfigurationWindow : public QMainWindow, private Ui::ConfigurationWindowBase
{
    Q_OBJECT

public:
  ConfigurationWindow(QWidget *parent = 0, Controller *controller = 0);

  void displayObject1Coordinates (Coordinates *coord) ;
  void displayObject2Coordinates (Coordinates *coord) ;

  void setIrrlichtController (IrrlichtController *irrController) ;

  void setSpNavSensitivity (char id, int translate, int rotation) ;
  void setPolhemusSensitivity (char id, int translate, int rotation) ;

  void displaySpNavConnected (char id, bool connected) ;
  void displaySpNavTranslate (char id, bool active) ;
  void displaySpNavRotate (char id, bool active) ;
  void displayPolhemusClutch (char id, bool active) ;

  void displayCameraScale (float val) ;
  void displayCameraFOVangle (float val) ;
  void displayCameraNear (float val) ;
  void displayCameraFar (float val) ;
  void displayCameraFrustum (float near, float far) ;
  void displayCameraPosition (float x, float y, float z) ;

  void setAxes1 (bool checked) ;
  void setAxes2 (bool checked) ;
  void setSteroAnaglyph (bool checked) ;

public slots:

  void changedPosition1X (double val) ;
  void changedPosition1Y (double val) ;
  void changedPosition1Z (double val) ;
  void changedPosition2X (double val) ;
  void changedPosition2Y (double val) ;
  void changedPosition2Z (double val) ;
  void changedRotation1X (double val) ;
  void changedRotation1Y (double val) ;
  void changedRotation1Z (double val) ;
  void changedRotation2X (double val) ;
  void changedRotation2Y (double val) ;
  void changedRotation2Z (double val) ;

  void changedChkSpNavSameSensitivity () ;
  void changedChkPolhemusSameSensitivity () ;

  void changedSpinSpNavSensitivity (int value) ;
  void changedSpinPolhemusSensitivity (int value) ;

  void changedCameraScale (double val) ;
  void changedCameraFOVangle (double val) ;
  void changedCameraNear (double val) ;
  void changedCameraFar (double val) ;
  void changedCameraPosition (double val) ;
  void clickedCameraFit () ;
  void clickedCameraNormalize() ;

  void changedChkAxes1 () ;
  void changedChkAxes2 () ;
  void changedChkStereoAnaglyph() ;

  void clickedBtnObject1Reset () ;
  void clickedBtnObject2Reset () ;

  void clickedBtnObject1Remove () ;
  void clickedBtnObject2Remove () ;

  void clickedBtnSpNav1Translate () ;
  void clickedBtnSpNav2Translate () ;
  void clickedBtnSpNav1Rotate () ;
  void clickedBtnSpNav2Rotate () ;

  void clickedBtnPolhemus1Clutch () ;
  void clickedBtnPolhemus2Clutch () ;  

  void clickedGenerateThumbnailsAll () ;
  void clickedGenerateThumbnailsMissing () ;

private:
  void diplayLabelActive (bool active, QLabel *label) ;

  Controller *m_controller ;

  IrrlichtController *m_irrController ;

};

#endif /* WINDOW2_HPP */
