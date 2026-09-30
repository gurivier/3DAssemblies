#include <iostream>
#include <QtWidgets/QApplication>

#include "ConfigurationWindow.h"
#include "AssemblyWindow.h"

#include "v4l2grab.h"

using namespace std ;

int main(int argc, char *argv[])
{

  /* ==== V4L2 init ==== */

  init_v4l2 (argc, argv) ;

  // -- open and initialize device
  deviceOpen() ;
  deviceInit() ;

  // -- start capturing
  captureStart() ;

  /* ==== Cartouches ==== */

  // TargetManager *targetManager = new TargetManager (12, 640, 480) ;
  TargetManager *targetManager = new TargetManager ("setup", 640, 480) ;

  /* ==== QT ==== */

  QApplication app(argc, argv) ;

  // -- configuration window for TargetManager -- Mandatory --
  ConfigurationWindow configurationWindow (targetManager) ;
  configurationWindow.timerStart(5) ; // interval in milliseconds
  configurationWindow.show() ; 

  // -- assembly window
  AssemblyWindow assemblyWindow ;
  assemblyWindow.setTargetManager(targetManager) ; // -- Mandatory --
  assemblyWindow.show() ;

  // assemblyWindow.loadFragment("images/triangle.png") ;
  // assemblyWindow.loadFragment("images/square.png") ;
  // assemblyWindow.loadFragment("images/circle.png") ;
  // assemblyWindow.loadFragment("images/star.png") ; 

  assemblyWindow.loadFragment("images/A.png") ;
  assemblyWindow.loadFragment("images/B.png") ;
  assemblyWindow.loadFragment("images/C.png") ;
  assemblyWindow.loadFragment("images/D.png") ;
  assemblyWindow.loadFragment("images/E.png") ;
  assemblyWindow.loadFragment("images/F.png") ;
  assemblyWindow.loadFragment("images/G.png") ;
  assemblyWindow.loadFragment("images/H.png") ;
  assemblyWindow.loadFragment("images/I.png") ;
  assemblyWindow.loadFragment("images/J.png") ;
  assemblyWindow.loadFragment("images/K.png") ;
  assemblyWindow.loadFragment("images/L.png") ;

  targetManager->setAssemblyWindow (&assemblyWindow) ;
  targetManager->setConfigurationWindow (&configurationWindow) ;

  int status = app.exec() ;

  configurationWindow.timerStop() ;

  /* ==== V4L2 close ==== */

  // -- stop capturing
  captureStop() ;

  // -- close device
  deviceUninit() ;
  deviceClose() ;

  cout << "Bye." << endl ;

  return status ;
}
