#include <QtGui>
#include <iostream>

#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QAction>


#include "ConfigurationWindow.h"
#include "CameraArea.h"
#include "v4l2grab.h"

//=========================
//== CONSTRUCTORS public
//=========================

ConfigurationWindow::ConfigurationWindow (TargetManager *targetManager)
  : m_targetManager (targetManager),
    m_imageWidth (targetManager->getImageWidth()),
    m_imageHeight (targetManager->getImageHeight()),
    m_viewNone (false),
    m_viewTreatedImage (false),
    m_viewFullTreatedImage (false)
{
    QSpacerItem *spacer = new QSpacerItem (0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    QFrame * horizontalSeparatorFrame = new QFrame(this);
    horizontalSeparatorFrame->setFrameShape(QFrame::HLine);
    horizontalSeparatorFrame->setFrameShadow(QFrame::Sunken);

    QFrame * horizontalSeparatorFrame2 = new QFrame(this);
    horizontalSeparatorFrame2->setFrameShape(QFrame::HLine);
    horizontalSeparatorFrame2->setFrameShadow(QFrame::Sunken);

    // -- Viewing Modes
    m_viewNoneRadiobutton = new QRadioButton (tr("No image"));
    m_viewRawImageRadiobutton = new QRadioButton (tr("Raw image"));
    m_viewFullTreatedImageRadiobutton = new QRadioButton (tr("Treated: Full"));
    m_viewTargetsTreatedImageRadiobutton = new QRadioButton (tr("Treated: Targets"));
    m_viewRawImageRadiobutton->setChecked(true);
    onImageViewModeRadioButtonClicked();

    // -- Viewing Modes Layout
    QVBoxLayout *viewingModesLayout = new QVBoxLayout;
    viewingModesLayout->addWidget(m_viewNoneRadiobutton);
    viewingModesLayout->addWidget(m_viewRawImageRadiobutton);
    viewingModesLayout->addWidget(m_viewFullTreatedImageRadiobutton) ;
    viewingModesLayout->addWidget(m_viewTargetsTreatedImageRadiobutton) ;  

    // -- Viewing Modes GroupBox
    QGroupBox *viewingModesGroupBox = new QGroupBox(tr("Viewing modes"));
    viewingModesGroupBox->setLayout(viewingModesLayout);

    // -- Configuration
    QPushButton *resetConfigurationButton = new QPushButton (tr("Reset"));
    QPushButton *saveConfigurationButton = new QPushButton (tr("Save"));

    // -- Configuration Layout
    QVBoxLayout *configurationLayout = new QVBoxLayout;
    configurationLayout->addWidget(resetConfigurationButton);
    configurationLayout->addWidget(saveConfigurationButton);

    // -- Configuration GroupBox
    QGroupBox *configurationGroupBox = new QGroupBox(tr("Configuration"));
    configurationGroupBox->setLayout(configurationLayout);

    // -- Viewing Setup Layout
    QVBoxLayout *viewingSetupLayout = new QVBoxLayout;
    viewingSetupLayout->addWidget(viewingModesGroupBox);
    viewingSetupLayout->addWidget(configurationGroupBox);
    viewingSetupLayout->addItem(spacer);

    // -- Viewing Modes Area
    QWidget *viewingModesArea = new QWidget;
    viewingModesArea->setLayout(viewingSetupLayout);    

    // -- Camera Area
    m_cameraArea = new CameraArea(this, targetManager);

    // -- Viewing Layout
    QHBoxLayout *viewingLayout = new QHBoxLayout;
    viewingLayout->addWidget(m_cameraArea, 1);
    viewingLayout->addWidget(viewingModesArea);

    // -- Viewing Area
    QWidget *viewingArea = new QWidget;
    viewingArea->setLayout(viewingLayout);



    // -- Pre-treatment : Contrast
    m_contrastCheckBox = new QCheckBox (tr("Contrast:   "));
    m_contrastSpinBox = new QSpinBox() ;
    m_contrastSpinBox->setRange(0, 50);
    m_contrastSlider = new QSlider (Qt::Horizontal, this) ;
    m_contrastSlider->setRange(0, 50);
    m_contrastSlider->setTickPosition(QSlider::TicksBelow);
    m_contrastSlider->setTickInterval (10);

    // -- Contrast Layout
    QHBoxLayout *contrastLayout = new QHBoxLayout();
    contrastLayout->addWidget(m_contrastCheckBox);
    contrastLayout->addWidget(m_contrastSpinBox);
    contrastLayout->addWidget(new QLabel(tr("  0")));
    contrastLayout->addWidget(m_contrastSlider) ;
    contrastLayout->addWidget(new QLabel(tr("50")));

    // -- Pre-treatment : Luminosity
    m_luminosityCheckBox = new QCheckBox (tr("Luminosity:"));
    m_luminositySpinBox = new QSpinBox() ;
    m_luminositySpinBox->setRange(0, 50);
    m_luminositySlider = new QSlider (Qt::Horizontal, this) ;
    m_luminositySlider->setRange(0, 50);
    m_luminositySlider->setTickPosition(QSlider::TicksBelow);
    m_luminositySlider->setTickInterval (10);

    // -- Luminosity Layout
    QHBoxLayout *luminosityLayout = new QHBoxLayout();
    luminosityLayout->addWidget(m_luminosityCheckBox);
    luminosityLayout->addWidget(m_luminositySpinBox);
    luminosityLayout->addWidget(new QLabel(tr("  0")));
    luminosityLayout->addWidget(m_luminositySlider) ;
    luminosityLayout->addWidget(new QLabel(tr("50")));

    // -- Pre-treatment Layout
    QVBoxLayout *pretreatmentLayout = new QVBoxLayout();
    pretreatmentLayout->addLayout(contrastLayout);
    pretreatmentLayout->addWidget(horizontalSeparatorFrame2);
    pretreatmentLayout->addLayout(luminosityLayout);

    // -- Pre-treatment Area
    QGroupBox *pretreatmentArea = new QGroupBox(tr("Pre-treatment"));
    pretreatmentArea->setLayout(pretreatmentLayout);


    
    // -- Dection method: Gray scale
    m_grayscaleRadioButton = new QRadioButton (tr("Gray scale"));
    m_grayscaleLabel = new QLabel(tr("Threshold: ")) ;
    m_grayscaleSpinBox = new QSpinBox() ;
    m_grayscaleSpinBox->setRange(0, 255);
    m_grayscaleSlider = new QSlider (Qt::Horizontal, this) ;
    m_grayscaleSlider->setRange(0, 255);
    m_grayscaleSlider->setTickPosition(QSlider::TicksBelow);
    m_grayscaleSlider->setTickInterval (32);
    m_grayscaleInvertCheckBox = new QCheckBox (tr("Invert"));

    // -- Gray Scale Layout
    QHBoxLayout *grayscaleLayout = new QHBoxLayout();
    grayscaleLayout->addWidget(m_grayscaleRadioButton);
    grayscaleLayout->addItem(new QSpacerItem(16, 0, QSizePolicy::Minimum, QSizePolicy::Minimum));
    grayscaleLayout->addWidget(m_grayscaleInvertCheckBox);
    grayscaleLayout->addItem(new QSpacerItem(16, 0, QSizePolicy::Minimum, QSizePolicy::Minimum));
    grayscaleLayout->addWidget(m_grayscaleLabel);
    grayscaleLayout->addWidget(m_grayscaleSpinBox);
    grayscaleLayout->addItem(new QSpacerItem(16, 0, QSizePolicy::Minimum, QSizePolicy::Minimum));
    grayscaleLayout->addWidget(new QLabel(tr("0")));
    grayscaleLayout->addWidget(m_grayscaleSlider) ;
    grayscaleLayout->addWidget(new QLabel(tr("255")));

    // -- Dection method: Green extraction
    m_extractGreenRadioButton = new QRadioButton (tr("Extract green"));
    m_extractGreenLabel = new QLabel(tr("White threshold: ")) ;
    m_extractGreenSpinBox = new QSpinBox() ;
    m_extractGreenSpinBox->setRange(0, 255);
    m_extractGreenSlider = new QSlider (Qt::Horizontal, this) ;
    m_extractGreenSlider->setRange(0, 255);
    m_extractGreenSlider->setTickPosition(QSlider::TicksBelow);
    m_extractGreenSlider->setTickInterval (32);

    // -- Extract Green Layout
    QHBoxLayout *extractGreenLayout = new QHBoxLayout();
    extractGreenLayout->addWidget(m_extractGreenRadioButton);
    extractGreenLayout->addItem(new QSpacerItem(45, 0, QSizePolicy::Minimum, QSizePolicy::Minimum));
    extractGreenLayout->addWidget(m_extractGreenLabel);
    extractGreenLayout->addWidget(m_extractGreenSpinBox);
    extractGreenLayout->addItem(new QSpacerItem(16, 0, QSizePolicy::Minimum, QSizePolicy::Minimum));
    extractGreenLayout->addWidget(new QLabel(tr("0")));
    extractGreenLayout->addWidget(m_extractGreenSlider) ;
    extractGreenLayout->addWidget(new QLabel(tr("255")));

    // -- Detection Method Layout
    QVBoxLayout *detectionMethodLayout = new QVBoxLayout();
    detectionMethodLayout->addLayout(grayscaleLayout);
    detectionMethodLayout->addWidget(horizontalSeparatorFrame);
    detectionMethodLayout->addLayout(extractGreenLayout);

    // -- Detection Method Area
    QGroupBox *detectionMethodArea = new QGroupBox(tr("Detection method"));
    detectionMethodArea->setLayout(detectionMethodLayout);


    // -- Targets Setup
    QLabel *targetSizeLabel = new QLabel(tr("Size: ")) ;
    m_targetSizeSpinBox = new QSpinBox() ;
    m_targetSizeSpinBox->setRange(0, 32);
    m_targetSizeSpinBox->setSuffix(tr(" px"));
    m_targetSizeSpinBox->setSingleStep(2);
    QLabel *targetCapacityLabel = new QLabel(tr("Capacity:")) ;
    m_targetCapacityValueLabel = new QLabel(QString::number(0)) ;
    QLabel *targetDetectionLabel = new QLabel(tr("Target detection:")) ;
    m_areaTargetDetectionRadiobutton = new QRadioButton (tr("area"));
    m_pointsTargetDetectionRadiobutton = new QRadioButton (tr("5 points"));    
    m_areaTargetDetectionSpinBox = new QSpinBox() ;
    m_areaTargetDetectionSpinBox->setRange(0, 100);
    m_areaTargetDetectionSpinBox->setSuffix(tr(" %"));
    m_areaTargetDetectionSpinBox->setSingleStep(10);


    // -- Targets Setup Layout
    QHBoxLayout *targetSetupLayout = new QHBoxLayout;
    targetSetupLayout->addWidget(targetSizeLabel);
    targetSetupLayout->addWidget(m_targetSizeSpinBox);
    targetSetupLayout->addItem(new QSpacerItem(16, 0, QSizePolicy::Minimum, QSizePolicy::Minimum));
    targetSetupLayout->addWidget(targetCapacityLabel);
    targetSetupLayout->addWidget(m_targetCapacityValueLabel);
    targetSetupLayout->addItem(new QSpacerItem(16, 0, QSizePolicy::Minimum, QSizePolicy::Minimum));
    targetSetupLayout->addWidget(targetDetectionLabel);
    targetSetupLayout->addWidget(m_areaTargetDetectionRadiobutton);
    targetSetupLayout->addWidget(m_areaTargetDetectionSpinBox);
    targetSetupLayout->addWidget(m_pointsTargetDetectionRadiobutton);
    targetSetupLayout->addItem(spacer);

    // -- Targets Setup Area
    QGroupBox *targetSetupArea = new QGroupBox(tr("Targets"));
    targetSetupArea->setLayout(targetSetupLayout);


    
    // -- Central Layout
    QVBoxLayout *centralLayout = new QVBoxLayout;
    centralLayout->addWidget(viewingArea, 1);
    centralLayout->addWidget(pretreatmentArea);
    centralLayout->addWidget(detectionMethodArea);
    centralLayout->addWidget(targetSetupArea);

    // -- Central Area
    QWidget *centralArea = new QWidget;
    centralArea->setLayout(centralLayout);
    setCentralWidget(centralArea);


    // -- Connect Contrast
    connect(m_contrastSpinBox, SIGNAL(valueChanged(int)),
            m_contrastSlider, SLOT(setValue(int)));
    connect(m_contrastSlider, SIGNAL(valueChanged(int)),
            m_contrastSpinBox, SLOT(setValue(int)));
    connect(m_contrastSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(onContrastChanged(int)));

    connect(m_contrastCheckBox, SIGNAL(clicked(bool)),
            this, SLOT(onApplyContrastClicked(bool)));
    

    // -- Connect Luminosity
    connect(m_luminositySpinBox, SIGNAL(valueChanged(int)),
            m_luminositySlider, SLOT(setValue(int)));
    connect(m_luminositySlider, SIGNAL(valueChanged(int)),
            m_luminositySpinBox, SLOT(setValue(int)));
    connect(m_luminositySpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(onLuminosityChanged(int)));

    connect(m_luminosityCheckBox, SIGNAL(clicked(bool)),
            this, SLOT(onApplyLuminosityClicked(bool)));



    // -- Connect Gray Scale Method
    connect(m_grayscaleSpinBox, SIGNAL(valueChanged(int)),
            m_grayscaleSlider, SLOT(setValue(int)));
    connect(m_grayscaleSlider, SIGNAL(valueChanged(int)),
            m_grayscaleSpinBox, SLOT(setValue(int)));
    connect(m_grayscaleSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(onGrayscaleThresholdChanged(int)));

    connect(m_grayscaleInvertCheckBox, SIGNAL(clicked(bool)),
            this, SLOT(onGrayscaleInvertClicked(bool)));
    

    // -- Connect Extract Green Method
    connect(m_extractGreenSpinBox, SIGNAL(valueChanged(int)),
            m_extractGreenSlider, SLOT(setValue(int)));
    connect(m_extractGreenSlider, SIGNAL(valueChanged(int)),
            m_extractGreenSpinBox, SLOT(setValue(int)));
    connect(m_extractGreenSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(onWhiteThresholdChanged(int)));

    // -- Connect Detection Methods
    connect(m_grayscaleRadioButton, SIGNAL(toggled(bool)),
            this, SLOT(onGrayscaleRadioButtonToggled(bool)));


    // -- Connect Targets Setup / Size
    connect(m_targetSizeSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(onTargetSizeChanged(int)));

    // -- Connect Targets Setup / Target Detection Mode
    connect(m_areaTargetDetectionRadiobutton, SIGNAL(toggled(bool)),
            this, SLOT(onAreaTargetDetectionRadiobuttonToggled(bool)));

    connect(m_areaTargetDetectionSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(onAreaTargetDetectionChanged(int)));


    // -- Connect Configuration Buttons
    connect(resetConfigurationButton, SIGNAL(clicked(bool)),
            this, SLOT(onResetConfigurationButtonClicked()));
    connect(saveConfigurationButton, SIGNAL(clicked(bool)),
            this, SLOT(onSaveConfigurationButtonClicked()));

    // -- Connect Viewing Modes
    connect(m_viewNoneRadiobutton, SIGNAL(clicked(bool)),
            this, SLOT(onImageViewModeRadioButtonClicked()));
    connect(m_viewRawImageRadiobutton, SIGNAL(clicked(bool)),
            this, SLOT(onImageViewModeRadioButtonClicked()));
    connect(m_viewFullTreatedImageRadiobutton, SIGNAL(clicked(bool)),
            this, SLOT(onImageViewModeRadioButtonClicked()));
    connect(m_viewTargetsTreatedImageRadiobutton, SIGNAL(clicked(bool)),
            this, SLOT(onImageViewModeRadioButtonClicked()));

    // -- Connect Shortcuts
    QAction *exitAct = new QAction(this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    connect(exitAct, SIGNAL(triggered()),
	    this, SLOT(close()));


    //-- Set up window
    initialization () ;
    setWindowTitle(tr("Target Managers"));
    resize(870, 855);
}

//=========================
//== METHODS public
//=========================

void ConfigurationWindow::timerStart (int msec) {
  m_timer = new QTimer(this);
  m_timer->setInterval(msec); // interval in milliseconds
  connect(m_timer, SIGNAL(timeout()), this, SLOT(timerFunction()));
  m_timer->start();
}

void ConfigurationWindow::timerStop () {
  m_timer->stop();
  //  disconnect(SIGNAL(timeout()), this, SLOT(timerFunction()));
}

void ConfigurationWindow::initialization ()
{
  // -- Pre-treatment : Contrast
  setContrastChecked (m_targetManager->getApplyContrast()) ;
  setContrastValue (m_targetManager->getContrast()) ;

  // -- Pre-treatment : Luminosity
  setLuminosityChecked (m_targetManager->getApplyLuminosity()) ;
  setLuminosityValue (m_targetManager->getLuminosity()) ;

  // -- Dection method: Gray scale
  setGrayscaleChecked (m_targetManager->getGrayScaleDetection()) ;
  setGrayscaleThresholdValue (m_targetManager->getGrayScaleThreshold()) ;
  setGrayscaleInvertChecked (m_targetManager->getGrayScaleInverted()) ;

  // -- Dection method: Green extraction
  setExtractGreenChecked (!m_targetManager->getGrayScaleDetection()) ;
  setExtractGreenWhiteThresholdValue (m_targetManager->getWhiteThreshold()) ;

  // -- Targets Setup
  setTargetSizeValue (m_targetManager->getTargetSize()) ;
  setCapacityValue (m_targetManager->capacity()) ;
  setDetectionOfTargetAreaChecked (m_targetManager->getTargetAreaDetection()) ;
  setDetectionOfTargetAreaValue (m_targetManager->getTargetDetectionAreaSize()) ;
  setDetectionOfPointsChecked (!m_targetManager->getTargetAreaDetection()) ;
}

void ConfigurationWindow::setContrastChecked (bool checked)
{
  m_contrastCheckBox->setChecked (checked) ;
  m_contrastSpinBox->setEnabled (checked) ;
  m_contrastSlider->setEnabled (checked) ;
}

void ConfigurationWindow::setContrastValue (int value)
{
  m_contrastSpinBox->setValue (value) ;
  m_contrastSlider->setValue (value) ;
}

void ConfigurationWindow::setLuminosityChecked (bool checked)
{
  m_luminosityCheckBox->setChecked (checked) ;
  m_luminositySpinBox->setEnabled (checked) ;
  m_luminositySlider->setEnabled (checked) ;
}

void ConfigurationWindow::setLuminosityValue (int value)
{
  m_luminositySpinBox->setValue (value) ;
  m_luminositySlider->setValue (value) ;
}

void ConfigurationWindow::setGrayscaleChecked (bool checked)
{
  m_grayscaleRadioButton->setChecked (checked) ;
  setGrayscaleEnabled (checked) ;
  setExtractGreenEnabled (!checked) ;
}

void ConfigurationWindow::setGrayscaleThresholdValue (int value)
{
  m_grayscaleSpinBox->setValue(value) ;
  m_grayscaleSlider->setValue(value) ;
}

void ConfigurationWindow::setGrayscaleInvertChecked (bool checked)
{
  m_grayscaleInvertCheckBox->setChecked (checked) ;
}

void ConfigurationWindow::setExtractGreenChecked (bool checked)
{
  m_extractGreenRadioButton->setChecked(checked);
  setGrayscaleEnabled (!checked) ;
  setExtractGreenEnabled (checked) ;
}

void ConfigurationWindow::setExtractGreenWhiteThresholdValue (int value)
{
  m_extractGreenSpinBox->setValue(value) ;
  m_extractGreenSlider->setValue(value) ;
}

void ConfigurationWindow::setTargetSizeValue (int value)
{
  m_targetSizeSpinBox->setValue(value) ;
}

void ConfigurationWindow::setCapacityValue (int value)
{
  m_targetCapacityValueLabel->setText (QString::number(value)) ;
}

void ConfigurationWindow::setDetectionOfTargetAreaChecked (bool checked)
{
  m_areaTargetDetectionRadiobutton->setChecked(checked) ;
  m_areaTargetDetectionSpinBox->setEnabled(checked) ;  
  m_pointsTargetDetectionRadiobutton->setChecked(!checked) ;
}

void ConfigurationWindow::setDetectionOfTargetAreaValue (int value)
{
  m_areaTargetDetectionSpinBox->setValue(value) ;
}

void ConfigurationWindow::setDetectionOfPointsChecked (bool checked)
{
  m_areaTargetDetectionRadiobutton->setChecked(!checked) ;
  m_areaTargetDetectionSpinBox->setEnabled(!checked) ;  
  m_pointsTargetDetectionRadiobutton->setChecked(checked) ;
}


//=========================
//== METHODS protected
//=========================

void ConfigurationWindow::closeEvent (QCloseEvent *event)
{
  m_targetManager->quit () ;

  event->accept () ;
}


//=========================
//== SLOTS public
//=========================

void ConfigurationWindow::timerFunction () {

  m_targetManager->idleFunction(); // Acquires a frame and then the V4L2 global variable is set (v4l2_frame_RGB888).


  /* Display the captured image */
  if (!m_viewNone) {

    /* Process the captured image */
    if (m_viewTreatedImage) { // view mode
      m_targetManager->processTreatedImage(v4l2_frame_RGB888, m_viewFullTreatedImage);
      
      const QPoint* mouseMovingPoint = m_cameraArea->getMovingPoint();

      if (!m_viewFullTreatedImage && mouseMovingPoint != NULL)
	m_targetManager->processTreatedImageTarget(v4l2_frame_RGB888, mouseMovingPoint->x(), mouseMovingPoint->y());
    }

    //-- Load the frame in QImage
    m_cameraArea->openImage(v4l2_frame_RGB888, m_imageWidth, m_imageHeight);

    //-- Draw the targets on QImage
    m_cameraArea->drawTargets();
  }
}

void ConfigurationWindow::onContrastChanged (int contrast)
{
  // cout << "ConfigurationWindow::contrastChanged" << endl ;
  m_targetManager->setContrast(contrast);
}

void ConfigurationWindow::onLuminosityChanged(int luminosity)
{
  // cout << "ConfigurationWindow::luminosityChanged" << endl ;
  m_targetManager->setLuminosity(luminosity);
}

void ConfigurationWindow::onGrayscaleThresholdChanged (int threshold)
{
  // cout << "ConfigurationWindow::thresholdChanged" << endl ;
  m_targetManager->setGrayScaleThreshold(threshold);
}

void ConfigurationWindow::onWhiteThresholdChanged (int threshold)
{
  // cout << "ConfigurationWindow::thresholdChanged" << endl ;
  m_targetManager->setWhiteThreshold(threshold);
}

void ConfigurationWindow::onTargetSizeChanged (int size)
{
  //  cout << "ConfigurationWindow::targetSizeChanged" << endl ;
  m_targetManager->setTargetSize(size);
  m_cameraArea->setTargetSize(size);
}

void ConfigurationWindow::onAreaTargetDetectionChanged (int sizeRatio)
{
  m_targetManager->setTargetDetectionAreaSize(sizeRatio);
}

//=========================
//== SLOTS private
//=========================

void ConfigurationWindow::onImageViewModeRadioButtonClicked ()
{
  m_viewNone = m_viewNoneRadiobutton->isChecked();
  m_viewTreatedImage = !m_viewRawImageRadiobutton->isChecked();
  m_viewFullTreatedImage = m_viewFullTreatedImageRadiobutton->isChecked();

  if (m_viewNone)
    m_cameraArea->clearImage();
}

void ConfigurationWindow::onResetConfigurationButtonClicked ()
{
  m_targetManager->loadConfiguration ("setup") ;
}

void ConfigurationWindow::onSaveConfigurationButtonClicked ()
{
  m_targetManager->saveConfiguration ("setup") ;
}

void ConfigurationWindow::onApplyContrastClicked (bool checked)
{
  m_targetManager->setApplyContrast (checked) ;
  setContrastChecked (checked) ;
}

void ConfigurationWindow::onApplyLuminosityClicked (bool checked)
{
  m_targetManager->setApplyLuminosity (checked) ;
  setLuminosityChecked (checked) ;
}

void ConfigurationWindow::onGrayscaleRadioButtonToggled (bool checked)
{
  m_targetManager->setGreenDetection(!checked);
  setGrayscaleEnabled (checked) ;
  setExtractGreenEnabled (!checked) ;
}

void ConfigurationWindow::onGrayscaleInvertClicked (bool clicked)
{
  m_targetManager->setGrayScaleInverted (clicked);
}

void ConfigurationWindow::onAreaTargetDetectionRadiobuttonToggled (bool checked)
{
  m_targetManager->setTargetAreaDetection(checked) ;
  setDetectionOfTargetAreaChecked (checked) ;
}

//=========================
//== METHODS private
//=========================

void ConfigurationWindow::setGrayscaleEnabled (bool checked)
{
  m_grayscaleSpinBox->setEnabled (checked) ;
  m_grayscaleSlider->setEnabled (checked) ;
  m_grayscaleInvertCheckBox->setEnabled (checked) ;
  m_grayscaleLabel->setEnabled (checked) ;
}

void ConfigurationWindow::setExtractGreenEnabled (bool checked)
{
  m_extractGreenSpinBox->setEnabled (checked) ;
  m_extractGreenLabel->setEnabled (checked) ;
  m_extractGreenSlider->setEnabled (checked) ;
}
