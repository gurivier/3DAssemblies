
#include <QtGui>
#include <QtWidgets/QLabel>

#include "AssemblyWindow.h"
#include "Cartouche.h"

//=========================
//== CONSTRUCTORS public
//=========================

AssemblyWindow::AssemblyWindow()
  : m_numFragment (0)
{
  
  // -- Central Layout
  centralLayout = new QHBoxLayout ;
  
  // -- Central Area
  QWidget *centralArea = new QWidget ;
  centralArea->setLayout(centralLayout) ;
  setCentralWidget(centralArea) ;
  
  //-- Set up window
  setWindowTitle(tr("Assembly Space")) ;
  resize(1000, 200) ;
}

//=========================
//== METHODS public
//=========================

bool AssemblyWindow::loadFragment(const QString &fileName)
{
  
  // -- Image
  QImage *loadedImage = new QImage (QSize(128, 128), QImage::Format_RGB32) ;
  if (!loadedImage->load(fileName))
    return false ;

  //  QSize newSize = loadedImage.size().expandedTo(size()) ;
  //  resizeImage(&loadedImage, newSize) ;
  m_images.push_back(loadedImage) ;

  // -- Image Label
  QLabel *imageLabel = new QLabel ;
  imageLabel->setBackgroundRole(QPalette::Base) ;
  imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored) ;
  imageLabel->setScaledContents(true) ;
  centralLayout->addWidget(imageLabel, 1) ;
  m_labels.push_back(imageLabel) ;
  
  // -- Cartouche
  Cartouche *cartouche = new Cartouche (m_numFragment, fileName, this) ;
  m_targetManager->addCartouche(cartouche) ;
  m_numFragment++ ;

  update() ;
  return true ;
}

void AssemblyWindow::showFragment(const int i)
{
  m_labels[i]->setPixmap(QPixmap::fromImage(*m_images[i])) ;
}

void AssemblyWindow::hideFragment(const int i)
{
  QPixmap m_clearImagePixmap ;
  m_clearImagePixmap.fill(Qt::white) ;
  m_labels[i]->setPixmap(m_clearImagePixmap) ;
}

//=========================
//== METHODS protected
//=========================

void AssemblyWindow::closeEvent (QCloseEvent *event)
{
  m_targetManager->quit () ;

  event->accept () ;
}
