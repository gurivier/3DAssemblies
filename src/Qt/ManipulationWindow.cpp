#include <iostream>

#include "ManipulationWindow.hpp"
#include "Controller.hpp"
 
ManipulationWindow::ManipulationWindow(Controller *controller)
  : QMainWindow(NULL),
    m_controller (controller),
    m_irrWidget (this)
{
  setupUi (this) ;

  // -- Central Area
  //QWidget *centralArea = new QWidget(this) ;
  //QMainWindow::setCentralWidget(centralArea) ;

  // -- Central Layout
  QVBoxLayout *centralLayout = new QVBoxLayout(widgetView) ;
  centralLayout->addWidget ((QWidget *)&(m_irrWidget)) ;
  widgetView->setLayout(centralLayout) ;

  // -- Irrlicht Widget
  m_irrWidget.setParent((QWidget *)(widgetView));
  m_irrWidget.setGeometry(0, 0, 1600, 1200);
  m_irrWidget.init () ;

  connect(verticalSlider, SIGNAL(valueChanged(int)), this, SLOT(changedZoomSlider(int)));
  connect(spinBox, SIGNAL(valueChanged(int)), this, SLOT(changedScale(int)));

  //-- Set up window
  //setWindowTitle(tr("First Window")) ;
  //resize(1200, 800) ;
}
 
ManipulationWindow::~ManipulationWindow()
{

}

void ManipulationWindow::setIrrlichtController (IrrlichtController *irrController)
{
  m_irrController = irrController ;
}

void ManipulationWindow::init ()
{

}

void ManipulationWindow::changedZoomSlider (int i)
{
  m_irrController->setZoom (i) ;
}

void ManipulationWindow::changedScale (int i)
{
  m_irrController->setCameraScale (i) ;
  //m_irrController->moveCameraToFitObjectInsideViewingFrustum () ;
  m_irrController->askUpdateCameraFitting () ;
}

void ManipulationWindow::timerStart (int msec)
{
  m_timer = new QTimer(this);
  m_timer->setInterval(msec); // interval in milliseconds
  connect(m_timer, SIGNAL(timeout()), this, SLOT(timerFunction()));
  m_timer->start();
}

void ManipulationWindow::timerStop ()
{
  m_timer->stop();
  //  disconnect(SIGNAL(timeout()), this, SLOT(timerFunction()));
}

void ManipulationWindow::timerFunction ()
{
  m_controller->idle() ;
}

// Include the extra Qt file for signals and slots
//#include "moc_ManipulationWindow.cpp"
