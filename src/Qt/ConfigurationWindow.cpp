
#include <iostream>

#include <QtGui>

#include "ConfigurationWindow.hpp"
#include "Controller.hpp"

ConfigurationWindow::ConfigurationWindow (QWidget *parent, Controller *controller)
: QMainWindow(parent),
  m_controller (controller)
{
  setupUi (this) ;

  /* Space Navigators */

  connect(spinBox_SpNav1_Translate, SIGNAL(valueChanged(int)), this, SLOT(changedSpinSpNavSensitivity(int)));
  connect(spinBox_SpNav2_Translate, SIGNAL(valueChanged(int)), this, SLOT(changedSpinSpNavSensitivity(int)));
  connect(spinBox_SpNav1_Rotate, SIGNAL(valueChanged(int)), this, SLOT(changedSpinSpNavSensitivity(int)));
  connect(spinBox_SpNav2_Rotate, SIGNAL(valueChanged(int)), this, SLOT(changedSpinSpNavSensitivity(int)));

  connect(pushButton_SpNav1_Translate, SIGNAL(clicked()), this, SLOT(clickedBtnSpNav1Translate()));
  connect(pushButton_SpNav2_Translate, SIGNAL(clicked()), this, SLOT(clickedBtnSpNav2Translate()));
  connect(pushButton_SpNav1_Rotate, SIGNAL(clicked()), this, SLOT(clickedBtnSpNav1Rotate()));
  connect(pushButton_SpNav2_Rotate, SIGNAL(clicked()), this, SLOT(clickedBtnSpNav2Rotate()));

  /* Polhemus */

  connect(spinBox_Polhemus1_Translate, SIGNAL(valueChanged(int)), this, SLOT(changedSpinPolhemusSensitivity(int)));
  connect(spinBox_Polhemus2_Translate, SIGNAL(valueChanged(int)), this, SLOT(changedSpinPolhemusSensitivity(int)));
  connect(spinBox_Polhemus1_Rotate, SIGNAL(valueChanged(int)), this, SLOT(changedSpinPolhemusSensitivity(int)));
  connect(spinBox_Polhemus2_Rotate, SIGNAL(valueChanged(int)), this, SLOT(changedSpinPolhemusSensitivity(int)));

  connect(pushButton_Polhemus1_Clutch, SIGNAL(clicked()), this, SLOT(clickedBtnPolhemus1Clutch()));
  connect(pushButton_Polhemus2_Clutch, SIGNAL(clicked()), this, SLOT(clickedBtnPolhemus2Clutch()));

  /* Objects */

  // Position 1
  connect(doubleSpinBox_position1X, SIGNAL(valueChanged(double)), this, SLOT(changedPosition1X(double)));
  connect(doubleSpinBox_position1Y, SIGNAL(valueChanged(double)), this, SLOT(changedPosition1Y(double)));
  connect(doubleSpinBox_position1Z, SIGNAL(valueChanged(double)), this, SLOT(changedPosition1Z(double)));

  // Rotation 1
  connect(doubleSpinBox_rotation1X, SIGNAL(valueChanged(double)), this, SLOT(changedRotation1X(double)));
  connect(doubleSpinBox_rotation1Y, SIGNAL(valueChanged(double)), this, SLOT(changedRotation1Y(double)));
  connect(doubleSpinBox_rotation1Z, SIGNAL(valueChanged(double)), this, SLOT(changedRotation1Z(double)));

  // Position 2
  connect(doubleSpinBox_position2X, SIGNAL(valueChanged(double)), this, SLOT(changedPosition2X(double)));
  connect(doubleSpinBox_position2Y, SIGNAL(valueChanged(double)), this, SLOT(changedPosition2Y(double)));
  connect(doubleSpinBox_position2Z, SIGNAL(valueChanged(double)), this, SLOT(changedPosition2Z(double)));

  // Rotation 1
  connect(doubleSpinBox_rotation2X, SIGNAL(valueChanged(double)), this, SLOT(changedRotation2X(double)));
  connect(doubleSpinBox_rotation2Y, SIGNAL(valueChanged(double)), this, SLOT(changedRotation2Y(double)));
  connect(doubleSpinBox_rotation2Z, SIGNAL(valueChanged(double)), this, SLOT(changedRotation2Z(double)));

  // Reset 1 & 2
  connect(pushButton_Object1_Reset, SIGNAL(clicked()), this, SLOT(clickedBtnObject1Reset()));
  connect(pushButton_Object2_Reset, SIGNAL(clicked()), this, SLOT(clickedBtnObject2Reset()));

  connect(pushButton_Object1_Remove, SIGNAL(clicked()), this, SLOT(clickedBtnObject1Remove()));
  connect(pushButton_Object2_Remove, SIGNAL(clicked()), this, SLOT(clickedBtnObject2Remove()));

  /* Camera / Scene */

  connect(checkBox_axes1, SIGNAL(clicked()), this, SLOT(changedChkAxes1()));
  connect(checkBox_axes2, SIGNAL(clicked()), this, SLOT(changedChkAxes2()));

  connect(checkBox_stereoAnaglyph, SIGNAL(clicked()), this, SLOT(changedChkStereoAnaglyph()));

  connect(doubleSpinBox_cameraScale, SIGNAL(valueChanged(double)), this, SLOT(changedCameraScale(double)));
  connect(doubleSpinBox_cameraFOV, SIGNAL(valueChanged(double)), this, SLOT(changedCameraFOVangle(double)));
  connect(doubleSpinBox_cameraNear, SIGNAL(valueChanged(double)), this, SLOT(changedCameraNear(double)));
  connect(doubleSpinBox_cameraFar, SIGNAL(valueChanged(double)), this, SLOT(changedCameraFar(double)));

  connect(pushButton_cameraFit, SIGNAL(clicked()), this, SLOT(clickedCameraFit()));
  connect(pushButton_cameraNormalize, SIGNAL(clicked()), this, SLOT(clickedCameraNormalize()));

  connect(doubleSpinBox_cameraPositionX, SIGNAL(valueChanged(double)), this, SLOT(changedCameraPosition(double)));
  connect(doubleSpinBox_cameraPositionY, SIGNAL(valueChanged(double)), this, SLOT(changedCameraPosition(double)));
  connect(doubleSpinBox_cameraPositionZ, SIGNAL(valueChanged(double)), this, SLOT(changedCameraPosition(double)));

  /* Thumbnails */

  connect(pushButton_generateThumbnailsAll, SIGNAL(clicked()), this, SLOT(clickedGenerateThumbnailsAll()));
  connect(pushButton_generateThumbnailsMissing, SIGNAL(clicked()), this, SLOT(clickedGenerateThumbnailsMissing()));

  /* Tool tips */

  QString tooltip ("Device sensitivity") ;
  spinBox_SpNav1_Translate->setToolTip (tooltip) ;
  spinBox_SpNav1_Rotate->setToolTip (tooltip) ;
  horizontalSlider_SpNav1_Translate->setToolTip (tooltip) ;
  horizontalSlider_SpNav1_Rotate->setToolTip (tooltip) ;
  spinBox_SpNav2_Translate->setToolTip (tooltip) ;
  spinBox_SpNav2_Rotate->setToolTip (tooltip) ;
  horizontalSlider_SpNav2_Translate->setToolTip (tooltip) ;
  horizontalSlider_SpNav2_Rotate->setToolTip (tooltip) ;
}

void ConfigurationWindow::setIrrlichtController (IrrlichtController *irrController)
{
  m_irrController = irrController ;
}

void ConfigurationWindow::displayObject1Coordinates (Coordinates *coord)
{
  /*
  label_px1->setText (QString("pX=%1").arg(coord->px, 9, 'f', 4)) ;
  label_py1->setText (QString("pY=%1").arg(coord->py, 9, 'f', 4)) ;
  label_pz1->setText (QString("pZ=%1").arg(coord->pz, 9, 'f', 4)) ;
  label_rx1->setText (QString("rX=%1").arg(coord->rx, 9, 'f', 4)) ;
  label_ry1->setText (QString("rY=%1").arg(coord->ry, 9, 'f', 4)) ;
  label_rz1->setText (QString("rZ=%1").arg(coord->rz, 9, 'f', 4)) ;
  */
  doubleSpinBox_position1X->setValue (coord->px) ;
  doubleSpinBox_position1Y->setValue (coord->py) ;
  doubleSpinBox_position1Z->setValue (coord->pz) ;

  doubleSpinBox_rotation1X->setValue (coord->rx) ;
  doubleSpinBox_rotation1Y->setValue (coord->ry) ;
  doubleSpinBox_rotation1Z->setValue (coord->rz) ;

  label_rotation1X->setText (QString("%1°").arg(coord->rx*core::RADTODEG+180.f, 6, 'f', 1)) ;
  label_rotation1Y->setText (QString("%1°").arg(coord->ry*core::RADTODEG+180.f, 6, 'f', 1)) ;
  label_rotation1Z->setText (QString("%1°").arg(coord->rz*core::RADTODEG+180.f, 6, 'f', 1)) ;
}

void ConfigurationWindow::displayObject2Coordinates (Coordinates *coord)
{
  /*
  label_px2->setText (QString("pX=%1").arg(coord->px, 9, 'f', 4)) ;
  label_py2->setText (QString("pY=%1").arg(coord->py, 9, 'f', 4)) ;
  label_pz2->setText (QString("pZ=%1").arg(coord->pz, 9, 'f', 4)) ;
  label_rx2->setText (QString("rX=%1").arg(coord->rx, 9, 'f', 4)) ;
  label_ry2->setText (QString("rY=%1").arg(coord->ry, 9, 'f', 4)) ;
  label_rz2->setText (QString("rZ=%1").arg(coord->rz, 9, 'f', 4)) ;
  */
  doubleSpinBox_position2X->setValue (coord->px) ;
  doubleSpinBox_position2Y->setValue (coord->py) ;
  doubleSpinBox_position2Z->setValue (coord->pz) ;
  doubleSpinBox_rotation2X->setValue (coord->rx) ;
  doubleSpinBox_rotation2Y->setValue (coord->ry) ;
  doubleSpinBox_rotation2Z->setValue (coord->rz) ;
}


void ConfigurationWindow::setSpNavSensitivity (char id, int translation, int rotation)
{
  QSpinBox *spbT, *spbR ;

  if (id == 1) {
    spbT = spinBox_SpNav1_Translate ;
    spbR = spinBox_SpNav1_Rotate ;
  }
  else {
    spbT = spinBox_SpNav2_Translate ;
    spbR = spinBox_SpNav2_Rotate ;
  }

  spbT->setValue (translation) ;
  spbR->setValue (rotation) ;
}

void ConfigurationWindow::displaySpNavConnected (char id, bool connected)
{
  QLabel *label ;

  if (id == 1) {
    label = label_SpNav1_Connected ;
  }
  else {  // id == 2
    label = label_SpNav2_Connected ;
  }

  if (connected) {
    label->setText ("Connected") ;
    label->setStyleSheet ("font-weight: bold; color: #9F0;") ;
  }
  else {
    label->setText ("Not connected") ;
    label->setStyleSheet ("font-weight: bold; color: #F03;") ;
  }
}

void ConfigurationWindow::setPolhemusSensitivity (char id, int translation, int rotation)
{
  QSpinBox *spbT, *spbR ;

  if (id == 1) {
    spbT = spinBox_Polhemus1_Translate ;
    spbR = spinBox_Polhemus1_Rotate ;
  }
  else {  // id == 2
    spbT = spinBox_Polhemus2_Translate ;
    spbR = spinBox_Polhemus2_Rotate ;
  }

  spbT->setValue (translation) ;
  spbR->setValue (rotation) ;
}

void ConfigurationWindow::diplayLabelActive (bool active, QLabel *label) // PRIVATE
{
  if (active) {
    label->setText ("ON ") ;
    label->setStyleSheet ("font-weight: bold; color: #9F0;") ;
  }
  else {
    label->setText ("OFF") ;
    label->setStyleSheet ("font-weight: bold; color: #F03;") ;
  }
}

void ConfigurationWindow::displaySpNavTranslate (char id, bool active)
{
  if (id == 1)
    this->diplayLabelActive (active, label_SpNav1_Translate) ;
  else // id == 2
    this->diplayLabelActive (active, label_SpNav2_Translate) ;
}

void ConfigurationWindow::displaySpNavRotate (char id, bool active)
{
  if (id == 1)
    this->diplayLabelActive (active, label_SpNav1_Rotate) ;
  else  // id == 2
    this->diplayLabelActive (active, label_SpNav2_Rotate) ;
}

void ConfigurationWindow::displayPolhemusClutch (char id, bool active)
{
  if (id == 1)
    this->diplayLabelActive (active, label_Polhemus1_Clutch) ;
  else  // id == 2
    this->diplayLabelActive (active, label_Polhemus2_Clutch) ;
}

void ConfigurationWindow::displayCameraScale (float val)
{
  doubleSpinBox_cameraScale->setValue (val) ;
}

void ConfigurationWindow::displayCameraFOVangle (float val)
{
  doubleSpinBox_cameraFOV->setValue (val) ;
  label_cameraFOVdegres->setText(QString("Field of view (%1°)").arg(val * 180.f / PI, 3, 'f', 1)) ;
}

void ConfigurationWindow::displayCameraNear (float val)
{
  doubleSpinBox_cameraNear->setValue (val) ;
}

void ConfigurationWindow::displayCameraFar (float val)
{
  doubleSpinBox_cameraFar->setValue (val) ;
}

void ConfigurationWindow::displayCameraFrustum (float near, float far)
{
  label_cameraFrustum->setText (QString("Frustum Near=%1 Far=%2").arg(near, 3, 'f', 1).arg(far, 3, 'f', 1)) ;
}

void ConfigurationWindow::displayCameraPosition (float x, float y, float z)
{
  //label_cameraPosition->setText (QString("Camera Position (%1, %2, %3)").arg(x, 6, 'f', 4).arg(y, 6, 'f', 4).arg(z, 6, 'f', 4)) ;
  doubleSpinBox_cameraPositionX->setValue (x) ;
  doubleSpinBox_cameraPositionY->setValue (y) ;
  doubleSpinBox_cameraPositionZ->setValue (z) ;
}

void ConfigurationWindow::setAxes1 (bool checked)
{
  checkBox_axes1->setChecked (checked) ;
}

void ConfigurationWindow::setAxes2 (bool checked)
{
  checkBox_axes2->setChecked (checked) ;
}

void ConfigurationWindow::changedPosition1X (double val)
{
  m_controller->setObjectPositionX (1, val) ;
}

void ConfigurationWindow::changedPosition1Y (double val)
{
  m_controller->setObjectPositionY (1, val) ;
}

void ConfigurationWindow::changedPosition1Z (double val)
{
  m_controller->setObjectPositionZ (1, val) ;
}

void ConfigurationWindow::changedPosition2X (double val)
{
  m_controller->setObjectPositionX (2, val) ;
}

void ConfigurationWindow::changedPosition2Y (double val)
{
  m_controller->setObjectPositionY (2, val) ;
}

void ConfigurationWindow::changedPosition2Z (double val)
{
  m_controller->setObjectPositionZ (2, val) ;
}

void ConfigurationWindow::changedRotation1X (double val)
{
  m_controller->setObjectRotationX (1, val) ;
}

void ConfigurationWindow::changedRotation1Y (double val)
{
  m_controller->setObjectRotationY (1, val) ;
}

void ConfigurationWindow::changedRotation1Z (double val)
{
  m_controller->setObjectRotationZ (1, val) ;
}

void ConfigurationWindow::changedRotation2X (double val)
{
  m_controller->setObjectRotationX (2, val) ;
}

void ConfigurationWindow::changedRotation2Y (double val)
{
  m_controller->setObjectRotationY (2, val) ;
}

void ConfigurationWindow::changedRotation2Z (double val)
{
  m_controller->setObjectRotationZ (2, val) ;
}

void ConfigurationWindow::changedChkSpNavSameSensitivity ()
{

}

void ConfigurationWindow::changedChkPolhemusSameSensitivity ()
{

}

void ConfigurationWindow::changedSpinSpNavSensitivity (int value)
{
  int tr1 = spinBox_SpNav1_Translate->value () ;
  int tr2 = spinBox_SpNav2_Translate->value () ;
  int ro1 = spinBox_SpNav1_Rotate->value () ;
  int ro2 = spinBox_SpNav2_Rotate->value () ;

  value = value ; // Pour eviter le warning "not used"

  this->m_controller->setSpNavSensitivity(tr1, ro1, tr2, ro2) ;
}

void ConfigurationWindow::changedSpinPolhemusSensitivity (int value)
{
  int tr1 = spinBox_Polhemus1_Translate->value () ;
  int tr2 = spinBox_Polhemus2_Translate->value () ;
  int ro1 = spinBox_Polhemus1_Rotate->value () ;
  int ro2 = spinBox_Polhemus2_Rotate->value () ;

  value = value ; // Pour eviter le warning "not used"

  this->m_controller->setPolhemusSensitivity(tr1, ro1, tr2, ro2) ;
}

void ConfigurationWindow::changedCameraScale (double val)
{
  std::cout << "ConfigurationWindow::changedCameraScale()" << std::endl ;
  m_controller->setCameraScale (val) ;
}

void ConfigurationWindow::changedCameraFOVangle (double val)
{
  m_controller->setCameraFOVangle (val) ;
}

void ConfigurationWindow::changedCameraNear (double val)
{
  m_controller->setCameraNear (val) ;
}

void ConfigurationWindow::changedCameraFar (double val)
{
  m_controller->setCameraFar (val) ;
}

void ConfigurationWindow::changedCameraPosition (double val)
{
  val = val ; // To avoid warning
  m_controller->setCameraPosition (doubleSpinBox_cameraPositionX->value (),
                                   doubleSpinBox_cameraPositionY->value (),
                                   doubleSpinBox_cameraPositionZ->value ()) ;
}

void ConfigurationWindow::clickedCameraFit()
{
  m_controller->cameraFitViewingFrustum () ;
}

void ConfigurationWindow::clickedCameraNormalize()
{
  m_controller->cameraNormalize () ;
}

void ConfigurationWindow::changedChkAxes1 ()
{
  m_controller->showAxes1 (checkBox_axes1->isChecked()) ;
}

void ConfigurationWindow::changedChkAxes2 ()
{
  m_controller->showAxes2 (checkBox_axes2->isChecked()) ;  
}

void ConfigurationWindow::changedChkStereoAnaglyph ()
{
  m_irrController->enableStereoAnaglyph (checkBox_stereoAnaglyph->isChecked()) ;
}

void ConfigurationWindow::clickedBtnObject1Reset ()
{
  this->m_controller->resetObjectPosition (1) ;
}

void ConfigurationWindow::clickedBtnObject2Reset ()
{
  this->m_controller->resetObjectPosition (2) ;
}


void ConfigurationWindow::clickedBtnObject1Remove ()
{
  m_irrController->removeMesh (1) ;
}

void ConfigurationWindow::clickedBtnObject2Remove ()
{
  m_irrController->removeMesh (2) ;
}

void ConfigurationWindow::clickedBtnSpNav1Translate ()
{
  this->m_controller->enableSpNavTranslate (1) ;
}

void ConfigurationWindow::clickedBtnSpNav2Translate ()
{
  this->m_controller->enableSpNavTranslate (2) ;
}

void ConfigurationWindow::clickedBtnSpNav1Rotate ()
{
  this->m_controller->enableSpNavRotate (1) ;
}

void ConfigurationWindow::clickedBtnSpNav2Rotate ()
{
  this->m_controller->enableSpNavRotate (2) ;
}

void ConfigurationWindow::clickedBtnPolhemus1Clutch ()
{
  this->m_controller->clutchPolhemus (1) ;
}

void ConfigurationWindow::clickedBtnPolhemus2Clutch ()
{
  this->m_controller->clutchPolhemus (2) ;
}

void ConfigurationWindow::clickedGenerateThumbnailsAll ()
{
  this->m_controller->generateMediasScreenshots (Controller::THUMBNAILS_ALL) ;
}

void ConfigurationWindow::clickedGenerateThumbnailsMissing ()
{
  this->m_controller->generateMediasScreenshots (Controller::THUMBNAILS_MISSING) ;
}

// Include the extra Qt file for signals and slots
//#include "moc_ConfigurationWindow.cpp"
