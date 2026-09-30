#ifndef CAMERAAREA_H
#define CAMERAAREA_H

#include <QtGui/QColor>
#include <QtGui/QImage>
#include <QtCore/QPoint>
#include <QtWidgets/QWidget>

#include "TargetManager.h"

class CameraArea : public QWidget
{
  Q_OBJECT

 public:

  //=========================
  //== CONSTRUCTORS public
  //=========================

  CameraArea(QWidget *parent = 0, TargetManager *targetManager = 0) ;


 public:

  //=========================
  //== ACCESSORS public
  //=========================

  inline void setTargetManager (TargetManager *targetManager) {
    m_targetManager = targetManager ;
    m_targetSize = targetManager->getTargetSize() ;
  }

  inline void setCurrentTargetNumber (int num) { m_currentTargetNum = num % m_targetManager->size() ; }
  inline int  getCurrentTargetNumber () { return m_currentTargetNum ; }

  inline void setTargetSize(int size) { m_targetSize = size ; }

  inline const QPoint* getMovingPoint () { return m_drawingMouseTarget ? &m_mouseTargetPosition : NULL ; }

 public:

  //=========================
  //== METHODS public
  //=========================

  bool openImage(const QString &fileName) ;
  void openImage(const uchar *data, int width, int height) ;
  bool saveImage(const QString &fileName, const char *fileFormat) ;

  void drawTargets () ;

  inline void nextTarget() { m_currentTargetNum = (m_currentTargetNum+1) % m_targetManager->size() ; }


 public slots:

  //=========================
  //== SLOTS public
  //=========================

  void clearImage() ;

 protected:

  //=========================
  //== METHODS protected
  //=========================

  void mousePressEvent(QMouseEvent *event) ;
  void mouseMoveEvent(QMouseEvent *event) ;
  void mouseReleaseEvent(QMouseEvent *event) ;
  void paintEvent(QPaintEvent *event) ;
  void resizeEvent(QResizeEvent *event) ;

 private:

  //=========================
  //== METHODS private
  //=========================

  void drawTarget(const QPoint &point, const int targetNum, const bool targetIsActive, const bool move) ;
  void resizeImage(QImage *image, const QSize &newSize) ;

 private:

  //=========================
  //== MEMBERS private
  //=========================

  int m_penWidth ;
  QImage m_image ;

  TargetManager *m_targetManager ;
  int    m_currentTargetNum ;
  QPoint m_mouseTargetPosition ;
  bool   m_drawingMouseTarget ;
  int    m_targetSize ;
  int    m_imageWidth ;
  int    m_imageHeight ;
};

#endif /* CAMERAAREA_H */
