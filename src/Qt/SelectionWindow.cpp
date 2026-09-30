#include <iostream>

#include <stdio.h> // sprintf
#include <unistd.h> // access

#include <QtCore/QList>
#include <QtCore/QSignalMapper>

#include "SelectionWindow.hpp"
 
#include "../Controller.hpp"

SelectionWindow::SelectionWindow(QWidget *parent, Controller *controller, MediaList *medias)
  : QMainWindow(parent),
    m_controller (controller),
    m_medias (medias),
    m_frames (100),
    m_labels (100),
    m_images (100)
{
  setupUi (this) ;

  // -- List View
  
  this->m_model = new QStandardItemModel (listView_Directories);

  char file_png[512] ;
  QPixmap pixmapDefault ("obj.png") ;
  QPixmap *pixmap ;
  bool pixmapAllocated = false ;

  for (int i=0 ; i < m_medias->size() ; i++) {

    // Pixmap with the first object of the collection (if the collection has at least one element)
    if (m_medias->getElement(i).size() > 0) {
      sprintf (file_png, "%s.png", m_medias->getElement(i).getElementFullName(0)) ;
      pixmap = new QPixmap (file_png) ;
      pixmapAllocated = true ;
    }
    else {
      pixmap = &pixmapDefault ;
    }
    
    // Create item
    QStandardItem *item = new QStandardItem (QIcon (*pixmap), m_medias->getElement(i).getName()) ;
    this->m_model->setItem(i, item);

    // Free allocated memory 
    if (pixmapAllocated) {
      pixmapAllocated = false ;
      delete pixmap ;
    }
  }

  listView_Directories->setModel (this->m_model) ;

  connect(listView_Directories->selectionModel(), 
          SIGNAL(selectionChanged(QItemSelection, QItemSelection)), 
          this,
          SLOT(handleSelectionChanged(QItemSelection)));

  //connect(this->m_model,
  //          SIGNAL(itemChanged(QStandardItem*)),
  //          this,
  //          SLOT(ReceiveChange(QStandardItem*)));

  // -- 


  m_gridFiles = new QGridLayout (widget_Files) ;
  widget_Files->setLayout (m_gridFiles) ;

  QSignalMapper *signalMapperLeft = new QSignalMapper (this);
  connect(signalMapperLeft, SIGNAL(mapped(int)), this, SLOT(objectClickedForLeftHand(int)));

  QSignalMapper *signalMapperRight = new QSignalMapper (this);
  connect(signalMapperRight, SIGNAL(mapped(int)), this, SLOT(objectClickedForRightHand(int)));


  for (int i=0 ; i<100 ; i++) {
    // Frame
    QFrame *frame = new QFrame (this) ;
    frame->setVisible (false) ;

    QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
    frame->setSizePolicy (sizePolicy) ;

    // Label Image
    QLabel *labelImage = new QLabel(frame);
    labelImage->setPixmap(QPixmap("obj2.png"));

    // Label Text
    QLabel *labelText = new QLabel (frame) ;

    // Left Button
    QToolButton *bLeft = new QToolButton (frame) ;
    bLeft->setText ("  L  ") ;
    signalMapperLeft->setMapping(bLeft, i);
    connect(bLeft, SIGNAL(clicked()), signalMapperLeft, SLOT(map()));

    // Right Button
    QToolButton *bRight = new QToolButton (frame) ;
    bRight->setText ("  R  ") ;
    signalMapperRight->setMapping(bRight, i);
    connect(bRight, SIGNAL(clicked()), signalMapperRight, SLOT(map()));

    // Style
    frame->setStyleSheet ("background-color: #646464;") ;
    bLeft->setStyleSheet ("background-color: #777;") ;
    bRight->setStyleSheet ("background-color: #777;") ;

    // Add
    QVBoxLayout *vLayout = new QVBoxLayout(frame) ;
    QWidget *empty = new QWidget (frame) ;
    QHBoxLayout *hLayout = new QHBoxLayout(empty) ;

    vLayout->setAlignment (Qt::AlignHCenter) ;

    frame->setLayout (vLayout) ;
    vLayout->addWidget (labelImage, Qt::AlignHCenter) ;
    vLayout->addWidget (labelText, Qt::AlignHCenter) ;
    vLayout->addWidget (empty) ;

    empty->setLayout (hLayout) ;
    hLayout->addWidget (bLeft) ;
    hLayout->addWidget (bRight) ;

    m_gridFiles->addWidget (frame, i/4, i%4) ;

    m_frames[i] = frame ;
    m_labels[i] = labelText ;
    m_images[i] = labelImage ;
  }

  //-- Set up window
  //setWindowTitle(tr("Selection Window")) ;
  //resize(800, 1800) ;
}
 
SelectionWindow::~SelectionWindow()
{

}
 
void SelectionWindow::handleSelectionChanged(const QItemSelection& selection)
{
  std::cout << "Selection Changed" << std::endl ;

  QList<QModelIndex> indexes = selection.indexes() ;

   if (indexes.isEmpty()) {
     std::cout << " empty" << std::endl ;
   }
   else {
     std::cout << " NOT empty" << std::endl ;

     QStandardItem *item = this->m_model->itemFromIndex(indexes.first()) ;

     std::cout << " range of indexes [" << indexes.first().row() << "] " << item->text().toStdString() << std::endl ;

     this->loadDirectory (indexes.first().row()) ;

   }
}

void SelectionWindow::loadDirectory (int i)
{
  int j ;

  m_curDir = i ;

  // ui->horizontalLayout->removeWidget(button1);

  char file_png[512] ;

  for (j=0 ; j < m_medias->getElement(m_curDir).size () ; j++) {
    sprintf (file_png, "%s.png", m_medias->getElement(m_curDir).getElementFullName(j)) ;

    //std::cout << "** " << m_medias->getElement(m_curDir).getElementFullName(j) << std::endl ;
    //std::cout << "Icon for " << j << "  will be " << file_png << std::endl ;

    m_frames[j]->setVisible (true) ;
    m_labels[j]->setText (m_medias->getElement(m_curDir).getElementName(j)) ;

    if (access(file_png, F_OK) != -1) {
      QPixmap pixmap(file_png);
      m_images[j]->setPixmap(pixmap.scaled(QSize(100,100), Qt::KeepAspectRatio));
    }
  }
  
  while (j < 100) {
    m_frames[j]->setVisible (false) ;
    j++ ;
  }

}

void SelectionWindow::objectClickedForLeftHand (int j)
{
  std::cout << "Selection for Left hand: " << j << std::endl ;

  m_controller->setMesh (1, m_medias->getElement(m_curDir).getElementFullName(j)) ;

}

void SelectionWindow::objectClickedForRightHand (int j)
{
  std::cout << "Selection for Right hand: " << j << std::endl ;

  m_controller->setMesh (2, m_medias->getElement(m_curDir).getElementFullName(j)) ;

}


// Include the extra Qt file for signals and slots
//#include "moc_SelectionWindow.cpp"
