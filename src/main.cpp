/* g++ main.cpp Irrlicht/IrrlichtController.cpp Irrlicht/AxesSceneNode.cpp SpaceNavigator/SpaceNavigatorServer.cpp -lIrrlicht -lpthread */

#include <iostream>
//#include <ctype.h>

#include <stdio.h>

#include <signal.h>

#include <QtWidgets/QApplication>
#include "SpaceNavigator/SpaceNavigatorServer.hpp"
#include <QStyleFactory>

#include "Controller.hpp"
#include "MediaList.hpp"

SpaceNavigatorServer *SPSRV ;
QApplication *app ;
bool first_sig = true ;

void sig (int s)
{
  std::cout << std::endl << "Interruption. (" << (s == SIGKILL ? "SIGKILL" : "SIGINT") << ")" << std::endl  ;

  if (first_sig) {
    std::cout << "FIRST ATTEMPT TO KILL: Attempting to exit gracefully." << std::endl  ;
    first_sig = false ;
    app->quit () ;
  }
  else {
    std::cout << "SECOND ATTEMPT TO KILL: Forcing to stop." << std::endl  ;
    SPSRV->freeRessources() ;
    delete SPSRV ;
    //delete app ;
    std::cout << "Bye (sig)." << std::endl  ;
    exit(0);
  }
}

void usage (char *program)
{
  std::cerr << program << " [-p port] -l left_model -r right_model" << std::endl ;
}

void loadStyleSheet (QApplication *app, const char *qssFilename)
{
  QFile File(qssFilename);
  File.open(QFile::ReadOnly);
  QString StyleSheet = QLatin1String(File.readAll());
  app->setStyleSheet(StyleSheet);
  File.close () ;
}

int main(int argc, char *argv[])
{
  char default_port[] = "10222" ;
  char default_model1_filename[] = "../media/Lettres2015b_SKP/lettreFdock.obj" ;
  char default_model2_filename[] = "../media/Lettres2015b_SKP/lettreF.obj" ;  

  char *port_s = default_port ;
  char *model1_filename = default_model1_filename ;
  char *model2_filename = default_model2_filename ;

  /* === PARMETERS ================================== */

  std::cout << "+++ PARMETERS +++" << std::endl ;

  for (int i=1 ; i < argc ; i++) {

    if (!strcmp(argv[i], "-p")) { // port
      if (++i >= argc) {

        exit (EXIT_FAILURE) ;
      }

      char *c = argv[i] ;
      while (*c != '\0' && std::isdigit (*c))
        c++ ;
      if (*c != '\0') {
        std::cerr << "Port must be a number. (" << argv[i] << " is not a number)" << std::endl ;
        usage(argv[0]) ;
        exit (EXIT_FAILURE) ;
      }

      port_s = argv[i] ;
    }
    else if (!strcmp(argv[i], "-l")) { // Left object
      if (++i >= argc) {
        usage(argv[0]) ;
        exit (EXIT_FAILURE) ;
      }

      model1_filename = argv[i] ;
    }
    else if (!strcmp(argv[i], "-r")) { // Right object
      if (++i >= argc) {
        usage(argv[0]) ;
        exit (EXIT_FAILURE) ;
      }
      model2_filename = argv[i] ;
    }
  }

  if (port_s == default_port) {
    std::cerr << "[MAIN] Taking default port '" << default_port << "'" << std::endl ;
  }

  if (model1_filename == default_model1_filename) {
    std::cerr << "[MAIN] Taking default left model '" << default_model1_filename << "'" << std::endl ;
  }

  if (model2_filename == default_model2_filename) {
    std::cerr << "[MAIN] Taking default right model '" << default_model2_filename << "'" << std::endl ;
  }

  /* === MEDIA LIST ================================== */

  std::cout << "+++ MEDIA LIST +++" << std::endl ;

  MediaList medias ("../media/") ;

  //medias.display () ;

  /* === SPACE NAVIGATOR SERVER ====================== */

  std::cout << "+++ SPACE NAVIGATOR SERVER +++" << std::endl ;

  SPSRV = new SpaceNavigatorServer (port_s) ;

  /* Free resources if interrupted */
  signal(SIGKILL, sig);
  signal(SIGINT, sig);

  /* === Qt ========================================== */

  std::cout << "+++ Qt +++" << std::endl ;

  app = new QApplication (argc, argv);

  //app->setStyleSheet () ;

  Controller controller (SPSRV, &medias) ;

  /* === MAIN LOOP =================================== */

  std::cout << "+++ MAIN LOOP +++" << std::endl ;

  std::cout << "main: controller.init()" << std::endl  ;
  controller.init(model1_filename, model2_filename) ;

  std::cout << "main: controller.start()" << std::endl  ;
  controller.start() ;

  loadStyleSheet (app, "stylesheet.qss") ;
  //loadStyleSheet (app, "qss/QTDark.qss") ;


  app->setStyle(QStyleFactory::create("Fusion"));


   QPalette p = app->palette();

   p.setColor(QPalette::Light, QColor("#999999"));
   p.setColor(QPalette::Dark, QColor("#343434"));
   //p.setColor(QPalette::Highlight, QColor("#0000FF"));

   /*
  p.setColor(QPalette::Window, QColor(53,53,53));
  p.setColor(QPalette::Button, QColor(53,53,53));
  p.setColor(QPalette::Highlight, QColor(142,45,197));
  p.setColor(QPalette::ButtonText, QColor(255,255,255));
  p.setColor(QPalette::WindowText, QColor(255,255,255));
   */

  /*
    // Dark purple
  p.setColor(QPalette::Window, QColor(53,53,53));
  p.setColor(QPalette::WindowText, Qt::white);
  p.setColor(QPalette::Base, QColor(15,15,15));
  p.setColor(QPalette::AlternateBase, QColor(53,53,53));
  p.setColor(QPalette::ToolTipBase, Qt::white);
  p.setColor(QPalette::ToolTipText, Qt::white);
  p.setColor(QPalette::Text, Qt::white);
  p.setColor(QPalette::Button, QColor(53,53,53));
  p.setColor(QPalette::ButtonText, Qt::white);
  p.setColor(QPalette::BrightText, Qt::red);

  p.setColor(QPalette::Highlight, QColor(142,45,197).lighter());
  p.setColor(QPalette::HighlightedText, Qt::black);

  p.setColor(QPalette::Disabled, QPalette::Text, Qt::darkGray);
  p.setColor(QPalette::Disabled, QPalette::ButtonText, Qt::darkGray);
  */

//    p.setColor(QPalette::Window, QColor(53,53,53));
//    p.setColor(QPalette::WindowText, Qt::white);
//    p.setColor(QPalette::Base, QColor(25,25,25));
//    p.setColor(QPalette::AlternateBase, QColor(53,53,53));
//    p.setColor(QPalette::ToolTipBase, Qt::white);
//    p.setColor(QPalette::ToolTipText, Qt::white);
//    p.setColor(QPalette::Text, Qt::white);
//    p.setColor(QPalette::Button, QColor(53,53,53));
//    p.setColor(QPalette::ButtonText, Qt::white);
//    p.setColor(QPalette::BrightText, Qt::red);
//    p.setColor(QPalette::Link, QColor(42, 130, 218));
//    
//    p.setColor(QPalette::Highlight, QColor(42, 130, 218));
//    p.setColor(QPalette::HighlightedText, Qt::black);

  app->setPalette(p);

  loadStyleSheet (app, "qss/darkorange/darkorange.qss") ;

  std::cout << "main: app->exec()" << std::endl  ;

  int status = app->exec() ;

  controller.stop() ;

  //SPSRV->freeRessources () ;

  printf ("%p\n", SPSRV) ;

  std::cout << "DELETE SPSRV avant" << std::endl  ;

  delete SPSRV ;

  std::cout << "DELETE SPSRV apres" << std::endl  ;

  //delete app ;

  std::cout << "Bye. (main)" << std::endl  ;

  return status ;
}
