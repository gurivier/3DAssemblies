#ifndef CONFIGURATIONWINDOW_H
#define CONFIGURATIONWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QSlider>
#include <QtWidgets/QLabel>
#include <QtCore/QTimer>

#include "TargetManager.h"

class CameraArea;

class ConfigurationWindow : public QMainWindow
{
  Q_OBJECT

 public:

  //=========================
  //== CONSTRUCTORS public
  //=========================

  ConfigurationWindow (TargetManager *targetManager) ;


 public:

  //=========================
  //== METHODS public
  //=========================

  void timerStart (int msec) ;
  void timerStop () ;

  void initialization () ;

  void setContrastChecked (bool checked) ;
  void setContrastValue (int value) ;

  void setLuminosityChecked (bool checked) ;
  void setLuminosityValue (int value) ;

  void setGrayscaleChecked (bool checked) ;
  void setGrayscaleThresholdValue (int value) ;
  void setGrayscaleInvertChecked (bool checked) ;

  void setExtractGreenChecked (bool checked) ;
  void setExtractGreenWhiteThresholdValue (int value) ;

  void setTargetSizeValue (int value) ;
  void setCapacityValue (int value) ;
  void setDetectionOfTargetAreaChecked (bool checked) ;
  void setDetectionOfTargetAreaValue (int value) ;
  void setDetectionOfPointsChecked (bool checked) ;


 protected:

  //=========================
  //== METHODS protected
  //=========================

  void closeEvent (QCloseEvent *event) ;


 public slots:

  //=========================
  //== SLOTS public
  //=========================

  void timerFunction () ;

  void onContrastChanged (int contrast) ;
  void onLuminosityChanged (int luminosity) ;

  void onGrayscaleThresholdChanged (int threshold) ;
  void onWhiteThresholdChanged (int threshold) ;

  void onTargetSizeChanged (int size) ;
  void onAreaTargetDetectionChanged (int sizeRatio) ;


 private slots:

  //=========================
  //== SLOTS private
  //=========================

  void onImageViewModeRadioButtonClicked () ;

  void onResetConfigurationButtonClicked () ;
  void onSaveConfigurationButtonClicked () ;

  void onApplyContrastClicked (bool checked) ;
  void onApplyLuminosityClicked (bool checked) ;

  void onGrayscaleRadioButtonToggled (bool checked) ;
  void onGrayscaleInvertClicked (bool clicked) ;

  void onAreaTargetDetectionRadiobuttonToggled (bool checked) ;


 private:

  //=========================
  //== METHODS private
  //=========================

  void createActions () ;
  void createMenus () ;
  bool maybeSave () ;
  bool saveFile (const QByteArray &fileFormat) ;

  void setGrayscaleEnabled (bool checked) ;
  void setExtractGreenEnabled (bool checked) ;


 private:

  //===============================
  //== MEMBERS private (widgets)
  //===============================

  // -- Viewing Modes
  QRadioButton *m_viewNoneRadiobutton;
  QRadioButton *m_viewRawImageRadiobutton;
  QRadioButton *m_viewFullTreatedImageRadiobutton;
  QRadioButton *m_viewTargetsTreatedImageRadiobutton;

  // -- Pre-treatment : Contrast
  QCheckBox *m_contrastCheckBox ;
  QSpinBox  *m_contrastSpinBox ;
  QSlider   *m_contrastSlider ;

  // -- Pre-treatment : Luminosity
  QCheckBox *m_luminosityCheckBox ;
  QSpinBox  *m_luminositySpinBox ;
  QSlider   *m_luminositySlider ;

  // -- Dection method: Gray scale
  QRadioButton *m_grayscaleRadioButton ;
  QLabel       *m_grayscaleLabel ;
  QSpinBox     *m_grayscaleSpinBox ;
  QSlider      *m_grayscaleSlider ;
  QCheckBox    *m_grayscaleInvertCheckBox ;

  // -- Dection method: Green extraction
  QRadioButton *m_extractGreenRadioButton ;
  QLabel       *m_extractGreenLabel ;
  QSpinBox     *m_extractGreenSpinBox ;
  QSlider      *m_extractGreenSlider ;

  // -- Targets Setup
  QSpinBox     *m_targetSizeSpinBox ;
  QLabel       *m_targetCapacityValueLabel;
  QRadioButton *m_areaTargetDetectionRadiobutton ;
  QRadioButton *m_pointsTargetDetectionRadiobutton ;
  QSpinBox     *m_areaTargetDetectionSpinBox;


  //===============================
  //== MEMBERS private (attributes)
  //===============================

  QTimer *m_timer ;

  CameraArea    *m_cameraArea;
  TargetManager *m_targetManager;

  int m_imageWidth;
  int m_imageHeight;

  bool m_viewNone;
  bool m_viewTreatedImage;
  bool m_viewFullTreatedImage;
};

#endif /* CONFIGURATIONWINDOW_H */
