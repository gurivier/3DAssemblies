#ifndef MANIPULATIONWINDOW_HPP
#define MANIPULATIONWINDOW_HPP

//#include <QtGui>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QtCore/QTimer>

#include "QIrrlichtWidget.hpp"
#include "../Irrlicht/IrrlichtController.hpp"
#include "../SpaceNavigator/SpaceNavigatorServer.hpp"

#include "ui_ManipulationWindowBase.h"

class Controller ;
 
class ManipulationWindow : public QMainWindow, private Ui::ManipulationWindowBase
{
  Q_OBJECT
 
public:

  ManipulationWindow(Controller *controller);

  ~ManipulationWindow();

  void setIrrlichtController (IrrlichtController *irrController) ;

  inline QIrrlichtWidget* getIrrlichtWidget() { return &m_irrWidget ; }

  void init () ;


  void timerStart (int msec) ;

  void timerStop () ;

  //inline void setSpaceNavigatorServer (SpaceNavigatorServer *SPSRV) { this->m_SPSRV = SPSRV ; }
 
  //inline void setIrrlichtController (IrrlichtController *irrController) { this->m_irrController = irrController ; }

public slots:

  void changedZoomSlider (int i) ;
  void changedScale (int i) ;

  void timerFunction () ;

private:

  Controller *m_controller ;

  IrrlichtController *m_irrController ;

  QTimer *m_timer ;

  QIrrlichtWidget m_irrWidget;

  //IrrlichtController *m_irrController ;

  //SpaceNavigatorServer *m_SPSRV ;

};
 
#endif /* MANIPULATIONWINDOW_HPP */
